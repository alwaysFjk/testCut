#include "toolbarrextanderasewidget.h"
#include "ui_toolbarrextanderasewidget.h"

ToolBarRextandEraseWidget::ToolBarRextandEraseWidget(QWidget *parent) :
    ToolBarExtandBaseWidget(parent),
    ui(new Ui::ToolBarRextandEraseWidget)
{
    ui->setupUi(this);
}

ToolBarRextandEraseWidget::~ToolBarRextandEraseWidget()
{
    delete ui;
}

void ToolBarRextandEraseWidget::reset()
{
    ui->spinBox->setValue(10);
}

int ToolBarRextandEraseWidget::penWidth()
{
    return ui->spinBox->value();
}
