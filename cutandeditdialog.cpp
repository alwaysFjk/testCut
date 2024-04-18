#include "cutandeditdialog.h"
#include "toolbarwidget.h"
#include "customdrawinfos.h"

#include <QPainter>

CutAndEditDialog::CutAndEditDialog(QWidget *parent)
    : CutDialog{parent}
{
    initToolBar();
}

void CutAndEditDialog::reset()
{
    if (m_toolBarWidget)
        m_toolBarWidget->reset();
    if (m_customDrawInfos)
        m_customDrawInfos->reset();
    CutDialog::reset();
}

void CutAndEditDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        CutDialog::mousePressEvent(event);
    }
    else if (m_toolBarWidget->getToolBarModel() == ToolBarWidget::DrawPoint ){

    }else if (m_toolBarWidget->getToolBarModel() == ToolBarWidget::DrawLine ||
               m_toolBarWidget->getToolBarModel() == ToolBarWidget::DrawRect){
        m_drawStartPos = event->pos();
    }else if (m_toolBarWidget->getToolBarModel() == ToolBarWidget::Erase ){

    }else {
        CutDialog::mousePressEvent(event);
    }
}

void CutAndEditDialog::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_toolBarWidget->getToolBarModel() == ToolBarWidget::DrawPoint ||
        m_toolBarWidget->getToolBarModel() == ToolBarWidget::DrawLine){
        m_customDrawInfos->endCurrentDrawPoint(QPen(m_toolBarWidget->penColor(), m_toolBarWidget->penWidth()),QBrush(m_toolBarWidget->brushColor()));
    }else if (m_toolBarWidget->getToolBarModel() == ToolBarWidget::DrawRect ){
        auto pen = QPen(m_toolBarWidget->penColor(), m_toolBarWidget->penWidth());
        if (m_drawStartPos.x() < event->pos().x())
            m_customDrawInfos->addCurrentDrawRect({m_drawStartPos, event->pos()}, pen, QBrush(m_toolBarWidget->brushColor()));
        else
            m_customDrawInfos->addCurrentDrawRect({event->pos(), m_drawStartPos}, pen, QBrush(m_toolBarWidget->brushColor()));
    }else if (m_toolBarWidget->getToolBarModel() == ToolBarWidget::Erase ){
        m_customDrawInfos->endCurrentErasePoint(m_toolBarWidget->penWidth());
    }
    return CutDialog::mouseReleaseEvent(event);
}

void CutAndEditDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (m_toolBarWidget->getToolBarModel() == ToolBarWidget::DrawLine){
        if (event->buttons() == Qt::LeftButton){
            m_customDrawInfos->clearCurrenCache();
            m_customDrawInfos->addCurrentDrawPoint(m_drawStartPos);
            m_customDrawInfos->addCurrentDrawPoint(event->pos());
        }
    }else if (m_toolBarWidget->getToolBarModel() == ToolBarWidget::DrawPoint){
        if (event->buttons() == Qt::LeftButton){
            m_customDrawInfos->addCurrentDrawPoint(event->pos());
        }
    }
    else if (m_toolBarWidget->getToolBarModel() == ToolBarWidget::DrawRect ){
        if (event->buttons() == Qt::LeftButton){
            if (m_drawStartPos.x() < event->pos().x())
                m_customDrawInfos->setCurrentDrawRect({m_drawStartPos, event->pos()});
            else
                m_customDrawInfos->setCurrentDrawRect({event->pos(), m_drawStartPos});
        }
    }else if (m_toolBarWidget->getToolBarModel() == ToolBarWidget::Erase ){
        if (event->buttons() == Qt::LeftButton){
            m_customDrawInfos->addCurrentErasePoint(event->pos(), m_toolBarWidget->penWidth());
        }
    }else{
        CutDialog::mouseMoveEvent(event);
        updateTooBarPosition();
        return;
    }
}

void CutAndEditDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    paintScreenPixMap(painter);
    // 绘制用户 画出来的线条和方框
    m_customDrawInfos->paintCustomDrawInfos(&painter);
    painter.setPen(QPen(m_toolBarWidget->penColor(), m_toolBarWidget->penWidth()));
    painter.setBrush(QBrush(m_toolBarWidget->brushColor()));
    m_customDrawInfos->paintCustomCurrentDrawInfo(&painter);

    paintMask(painter);
    paintBlueRect(painter);
}

void CutAndEditDialog::initToolBar()
{
    m_customDrawInfos = new CustomDrawInfos(this);
    // 初始化动作条
    m_toolBarWidget = new ToolBarWidget(this);
    connect(m_toolBarWidget, &ToolBarWidget::toolBarModelChange, this, &CutAndEditDialog::slot_toolBarWidget_toolBarModelChange);
    updateTooBarPosition();

    connect(m_toolBarWidget, &ToolBarWidget::undoBtnClicked, m_customDrawInfos, &CustomDrawInfos::undo);
    connect(m_toolBarWidget, &ToolBarWidget::redoBtnClicked, m_customDrawInfos, &CustomDrawInfos::redo);
    connect(m_toolBarWidget, &ToolBarWidget::sizeChange, this, &CutAndEditDialog::updateTooBarPosition);
    connect(m_toolBarWidget, &ToolBarWidget::cancelBtnClicked, this, &CutAndEditDialog::accept);
    connect(m_toolBarWidget, &ToolBarWidget::okBtnClicked, this, [=](){
        savePixmapToClipBoard();
        close();
    });
    connect(m_toolBarWidget, &ToolBarWidget::saveBtnClicked, this, [=](){
        if (savePixmapToLoacl())
            close();
    });
    connect(this, &CutAndEditDialog::screenPixmapChange, this, [=](){
        m_customDrawInfos->setSourcePixMap(&m_screenPixmap);
    });
}


void CutAndEditDialog::slot_toolBarWidget_toolBarModelChange()
{
    setCursor(Qt::ArrowCursor);
}

void CutAndEditDialog::updateTooBarPosition()
{
    QPoint newPos;
    if (m_toolBarWidget->height() + m_blueRect.bottom() > this->rect().bottom())
    {
        // 放蓝框里面
        newPos.setX(m_blueRect.bottomRight().x() - m_toolBarWidget->width());
        newPos.setY(m_blueRect.bottomRight().y() - m_toolBarWidget->height());
    }
    else{
        // 放在篮框外面
        newPos.setX(m_blueRect.bottomRight().x() - m_toolBarWidget->width());
        newPos.setY(m_blueRect.bottomRight().y());
    }
    m_toolBarWidget->move(newPos);
}
