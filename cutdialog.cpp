#include "cutdialog.h"
#include "ui_cutdialog.h"
#include <QApplication>
#include <QScreen>
#include <QPixmap>
#include <QLabel>
#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QClipboard>
#include <QFileDialog>
#include <QStandardPaths>

const int lineWidth = 3;
const int gripSize = 4; // 鼠标触发蓝框边缘的阈值

CutDialog::CutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CutDialog)
{
    qDebug() <<  __LINE__ << __FUNCTION__ ;
    ui->setupUi(this);
    this->setObjectName("CutDialog");
    // 移除窗口装饰
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    // 设置窗口背景为透明
    setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);
    // 捕捉当前屏幕的画面
    updateScreenToPixmap();
    m_blueRect = this->rect();

#ifdef QT_DEBUG
    m_frameTime.start();
#endif
}

CutDialog::~CutDialog()
{
    delete ui;
}

void CutDialog::reset()
{
    m_blueRect = this->rect();
    updateScreenToPixmap();
}


void CutDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        // 如果检测到右键点击，关闭对话框
        close();
    }
    else {
        // 这里实现一个  简单的鼠标单机拖动 更改蓝框的功能
        int ret = mouseUnderBuleBorder(event->pos());
        m_mouseMode = ret;
        m_lastPoint = event->pos();
    }
    QDialog::mousePressEvent(event);
}

void CutDialog::mouseReleaseEvent(QMouseEvent *event)
{
    m_mouseMode = 0;
}

void CutDialog::mouseMoveEvent(QMouseEvent *event)
{
    if(!this->isVisible())
        return;
    mouseUnderBuleBorder(event->pos());
    if (m_mouseMode != 0) {
        mouseMoveEventForAdjustBlueRect(event);
    }else{
        mouseMoveEventForResizeBlueRect(event);
    }
}

void CutDialog::mouseMoveEventForAdjustBlueRect(QMouseEvent *event)
{
    if (m_mouseMode != (Qt::TopEdge | Qt::LeftEdge | Qt::RightEdge | Qt::BottomEdge)){
        int dx = event->pos().x() - m_lastPoint.x();
        int dy = event->pos().y() - m_lastPoint.y();

        // 根据鼠标移动的方向，更改矩形的大小
        if (m_lastPoint.x() <= m_blueRect.left() + gripSize) {
            m_blueRect.setLeft(m_blueRect.left() + dx);
        } else if (m_lastPoint.x() >= m_blueRect.right() - gripSize) {
            m_blueRect.setRight(m_blueRect.right() + dx);
        }

        if (m_lastPoint.y() <= m_blueRect.top() + gripSize) {
            m_blueRect.setTop(m_blueRect.top() + dy);
        } else if (m_lastPoint.y() >= m_blueRect.bottom() - gripSize) {
            m_blueRect.setBottom(m_blueRect.bottom() + dy);
        }

        m_lastPoint = event->pos();
        update();
    } else {
        int dx = event->pos().x() - m_lastPoint.x();
        int dy = event->pos().y() - m_lastPoint.y();
        // 移动前的蓝框矩形位置预测
        QRect expectedRect = m_blueRect.translated(dx, dy);

        // 保持蓝框在对话框内部的边界
        if (expectedRect.left() < 0) {
            dx = -m_blueRect.left();
        } else if (expectedRect.right() > width()) {
            dx = width() - m_blueRect.right();
        }

        if (expectedRect.top() < 0) {
            dy = -m_blueRect.top();
        } else if (expectedRect.bottom() > height()) {
            dy = height() - m_blueRect.bottom();
        }
        m_blueRect.translate(dx, dy);
        m_lastPoint = event->pos();
        update();
    }
}

void CutDialog::mouseMoveEventForResizeBlueRect(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton){
        auto topLeft = m_lastPoint;
        auto bottomRight = event->pos();
        if (topLeft.x() < bottomRight.x()){
            m_blueRect.setTopLeft(topLeft);
            m_blueRect.setBottomRight(bottomRight);
        }else{
            m_blueRect.setTopLeft(bottomRight);
            m_blueRect.setBottomRight(topLeft);
        }
        update();
    }
}

void CutDialog::mouseDoubleClickEvent(QMouseEvent *event)
{
    savePixmapToClipBoard();
    close();
}

int CutDialog::mouseUnderBuleBorder(QPoint pos)
{
    int ret = 0;
    QRect topRect = m_blueRect.adjusted(-gripSize, -gripSize, gripSize, -m_blueRect.height() + gripSize);
    QRect leftRect = m_blueRect.adjusted(-gripSize, -gripSize, -m_blueRect.width() + gripSize, gripSize);
    QRect rightRect = m_blueRect.adjusted(-gripSize + m_blueRect.width(), -gripSize, gripSize, gripSize);
    QRect bottomRect = m_blueRect.adjusted(- gripSize, m_blueRect.height() - gripSize, gripSize, gripSize);
    if (topRect.contains(pos)){
        ret = Qt::TopEdge | ret;
    }
    if (leftRect.contains(pos)){
        ret = Qt::LeftEdge | ret;
    }
    if (rightRect.contains(pos)){
        ret = Qt::RightEdge | ret;
    }
    if (bottomRect.contains(pos)){
        ret = Qt::BottomEdge| ret;
    }
    if (ret == 0){
        // 判断 在蓝框里面or外面
        if(m_blueRect.contains(pos)){
            ret = Qt::TopEdge | Qt::LeftEdge | Qt::RightEdge | Qt::BottomEdge;
        }
    }

    if (ret == Qt::TopEdge || ret == Qt::BottomEdge){
        setCursor(Qt::SizeVerCursor);
    }else if (ret == Qt::LeftEdge || ret == Qt::RightEdge){
        setCursor(Qt::SizeHorCursor);
    }else if (ret == (Qt::TopEdge| Qt::LeftEdge) || ret == (Qt::BottomEdge| Qt::RightEdge)){
        setCursor(Qt::SizeFDiagCursor);
    }else if (ret == (Qt::TopEdge| Qt::RightEdge) || ret == (Qt::BottomEdge| Qt::LeftEdge)){
        setCursor(Qt::SizeBDiagCursor);
    } else if (ret == (Qt::TopEdge | Qt::LeftEdge | Qt::RightEdge | Qt::BottomEdge)){
        setCursor(Qt::SizeAllCursor);
    }
    else{
        setCursor(Qt::ArrowCursor);
    }

    return ret ;
}

void CutDialog::updateScreenToPixmap()
{
    auto currentScreen = QApplication::screenAt(QCursor::pos());
    QSize desktopSize = currentScreen->size();
    m_screenPixmap = currentScreen->grabWindow(0, 0, 0, desktopSize.width(), desktopSize.height());
    this->resize(desktopSize);
    emit screenPixmapChange();
}

void CutDialog::savePixmapToClipBoard()
{
    QRect rect = m_blueRect.adjusted(lineWidth, lineWidth, -lineWidth, -lineWidth);
    QPixmap pixmap(rect.size());
    this->render(&pixmap, QPoint(), QRegion(rect));
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setPixmap(pixmap);
}

bool CutDialog::savePixmapToLoacl()
{
    QRect rect = m_blueRect.adjusted(lineWidth, lineWidth, -lineWidth, -lineWidth);
    QPixmap pixmap(rect.size());
    this->render(&pixmap, QPoint(), QRegion(rect));
    auto filePath =  QFileDialog::getSaveFileName(nullptr, "save Image","", "*.png,*.jpg");
    if(!filePath.isEmpty()){
        return pixmap.save(filePath);
    }
    return false;
}

void CutDialog::showEvent(QShowEvent *)
{
}

void CutDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    paintScreenPixMap(painter);
    paintMask(painter);
    paintBlueRect(painter);

#ifdef QT_DEBUG
    if (m_frameTime.elapsed() >= 1000) { // 每秒更新一次帧率
        m_frameRate = m_frameCount * 1000.0f / m_frameTime.restart();
        m_frameCount = 0; // 重置计数
    }
    m_frameCount++; // 每次重绘时帧数加1
    // 将帧率显示到窗口
    painter.setFont(QFont("",40));
    painter.drawText(rect(), Qt::AlignTop | Qt::AlignRight, QString("FPS: %1").arg(m_frameRate, 0, 'f', 0));
#endif
}

void CutDialog::paintScreenPixMap(QPainter &painter)
{
    // 绘制截取的图片
    painter.drawPixmap(this->rect(), m_screenPixmap);
}

void CutDialog::paintMask(QPainter &painter)
{
    // 绘制半透明蒙层
    QBrush brush(QColor(0, 0, 0, 128));
    // 将绘制区域限制在蓝框外
    QPainterPath path;
    QList<QPointF> list;
    QRectF dialogRectF = this->rect();
    list << dialogRectF.topLeft() << dialogRectF.topRight() << dialogRectF.bottomRight() << dialogRectF.bottomLeft();
    list << dialogRectF.topLeft();
    list << m_blueRect.topLeft() << m_blueRect.topRight() << m_blueRect.bottomRight() << m_blueRect.bottomLeft();
    list << m_blueRect.topLeft() << dialogRectF.topLeft();
    path.addPolygon(list);
    painter.fillPath(path, brush);
}

void CutDialog::paintBlueRect(QPainter &painter)
{
    // 然后画蓝框
    painter.fillRect(m_blueRect, Qt::transparent);
    painter.setPen(QPen(Qt::blue, lineWidth));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(m_blueRect);
}





