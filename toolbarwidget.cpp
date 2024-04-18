#include "toolbarwidget.h"
#include "ui_toolbarwidget.h"
#include <QMouseEvent>
#include <QStyle>
#include <QDebug>
#include <QButtonGroup>

#include "toolbarextandpenwidget.h"
#include "toolbarrextanderasewidget.h"

ToolBarWidget::ToolBarWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolBarWidget)
{
    ui->setupUi(this);

    ui->drawPointBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxQuestion));
    ui->drawLineBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarMinButton));
    ui->drawRectBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarMaxButton));
    ui->redoBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowRight));    // 回复
    ui->undoBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowLeft));    // 撤销
    ui->okBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogYesButton));
    ui->cancelBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogNoButton));
    ui->saveBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogSaveButton));


    m_btnGroup << ui->drawPointBtn << ui->drawLineBtn << ui->drawRectBtn << ui->eraseButton;

    setMouseTracking(true);

    connect(ui->undoBtn, &QPushButton::clicked, this, &ToolBarWidget::undoBtnClicked);
    connect(ui->redoBtn, &QPushButton::clicked, this, &ToolBarWidget::redoBtnClicked);
    connect(ui->okBtn, &QPushButton::clicked, this, &ToolBarWidget::okBtnClicked);
    connect(ui->cancelBtn, &QPushButton::clicked, this, &ToolBarWidget::cancelBtnClicked);
    connect(ui->saveBtn, &QPushButton::clicked, this, &ToolBarWidget::saveBtnClicked);
    initExtandWidget();
    updateExtrandWidgetVisiable();
    reset();
}

ToolBarWidget::~ToolBarWidget()
{
    delete ui;
}

ToolBarWidget::ToolBarModel ToolBarWidget::getToolBarModel()
{
    if (ui->drawPointBtn->isChecked())
        return DrawPoint;
    if (ui->drawLineBtn->isChecked())
        return DrawLine;
    if (ui->drawRectBtn->isChecked()){
        return DrawRect;
    }
    if (ui->eraseButton->isChecked()){
        return Erase;
    }
    return None;
}

void ToolBarWidget::reset()
{
    ui->drawPointBtn->setChecked(false);
    ui->drawRectBtn->setChecked(false);
    ui->drawLineBtn->setChecked(false);
    ui->eraseButton->setChecked(false);
    updateExtrandWidgetVisiable();
}

int ToolBarWidget::penWidth()
{
    if (m_toolBarExtandPenWidget->isVisible())
        return m_toolBarExtandPenWidget->penWidth();
    else
        return m_toolBarRextandEraseWidget->penWidth();
}

void ToolBarWidget::mouseMoveEvent(QMouseEvent *event)
{
    event->ignore();
}

void ToolBarWidget::initExtandWidget()
{
    m_toolBarExtandPenWidget = new ToolBarExtandPenWidget(this->parentWidget());
    m_toolBarExtandPenWidget->setVisible(false);

    m_toolBarRextandEraseWidget= new ToolBarRextandEraseWidget(this->parentWidget());
    m_toolBarRextandEraseWidget->setVisible(false);

    m_toolBarExtandWidgetsList << m_toolBarExtandPenWidget << m_toolBarRextandEraseWidget;
}

void ToolBarWidget::updateExtrandWidgetVisiable()
{
    m_toolBarExtandPenWidget->setVisible(ui->drawPointBtn->isChecked() || ui->drawRectBtn->isChecked() || ui->drawLineBtn->isChecked());
    m_toolBarRextandEraseWidget->setVisible(ui->eraseButton->isChecked());
    emit sizeChange();
    updateExtrandWidgetPos();
}

void ToolBarWidget::updateExtrandWidgetPos()
{
    auto pos = mapToParent(this->rect().bottomLeft());
    for(auto var : m_toolBarExtandWidgetsList){
        if (var->isVisible()){
            var->move(pos);
        }
    }
    emit sizeChange();
}

void ToolBarWidget::moveEvent(QMoveEvent *)
{
    emit sizeChange();
}

void ToolBarWidget::resizeEvent(QResizeEvent *)
{
    emit sizeChange();
}

void ToolBarWidget::exclusiveBtn(QObject *sender)
{
    auto btn = qobject_cast<QPushButton*>(sender);
    if (btn->isChecked()){
        for(auto var : m_btnGroup){
            if (var != btn){
                var->setChecked(false);
            }
        }
    }
}

void ToolBarWidget::on_drawPointBtn_clicked()
{
    exclusiveBtn(sender());
    updateExtrandWidgetVisiable();
    emit toolBarModelChange();
}

void ToolBarWidget::on_drawRectBtn_clicked()
{
    exclusiveBtn(sender());
    updateExtrandWidgetVisiable();
    emit toolBarModelChange();
}

void ToolBarWidget::on_drawLineBtn_clicked()
{
    exclusiveBtn(sender());
    updateExtrandWidgetVisiable();
    emit toolBarModelChange();
}

void ToolBarWidget::on_eraseButton_clicked()
{
    exclusiveBtn(sender());
    updateExtrandWidgetVisiable();
    emit toolBarModelChange();
}

QColor ToolBarWidget::penColor() const
{
    return m_toolBarExtandPenWidget->penColor();
}

QColor ToolBarWidget::brushColor() const
{
    return m_toolBarExtandPenWidget->brushColor();
}

int ToolBarWidget::height()
{
    int height = QWidget::height();
    foreach (auto var, m_toolBarExtandWidgetsList) {
        if (var->isVisible()){
            height = height + var->height();
        }
    }
    return height;
}

int ToolBarWidget::width()
{
    int width = QWidget::width();
    foreach (auto var, m_toolBarExtandWidgetsList) {
        if (var->isVisible()){
            width = std::max(width, var->width());
        }
    }
    return width;
}
