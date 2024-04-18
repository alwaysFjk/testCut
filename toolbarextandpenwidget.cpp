#include "toolbarextandpenwidget.h"
#include "ui_toolbarextandpenwidget.h"

#include <QColorDialog>

ToolBarExtandPenWidget::ToolBarExtandPenWidget(QWidget *parent) :
    ToolBarExtandBaseWidget(parent),
    ui(new Ui::ToolBarExtandPenWidget)
{
    ui->setupUi(this);
    reset();
}

ToolBarExtandPenWidget::~ToolBarExtandPenWidget()
{
    delete ui;
}

void ToolBarExtandPenWidget::reset()
{
    ui->spinBox->setValue(1);
    setPenColor(Qt::black);
    setBrushColor(Qt::transparent);
}

QColor ToolBarExtandPenWidget::penColor() const
{
    return m_penColor;
}

void ToolBarExtandPenWidget::setPenColor(const QColor &newPenColor)
{
    if (m_penColor == newPenColor)
        return;
    m_penColor = newPenColor;
    emit penColorChanged();
    QString colorStyle = QString("background-color: #%1").arg(newPenColor.rgb(), 6, 16, QChar('0'));
    ui->colorButton->setStyleSheet(colorStyle);
}

int ToolBarExtandPenWidget::penWidth()
{
    return ui->spinBox->value();
}

void ToolBarExtandPenWidget::on_colorButton_clicked()
{
    QColor selectedColor = QColorDialog::getColor(Qt::white, this, "Select Color");
    if (selectedColor.isValid()) {
        setPenColor(selectedColor);
    }
}

void ToolBarExtandPenWidget::on_brushButton_clicked()
{
    QColor selectedColor = QColorDialog::getColor(Qt::white, this, "Select Color", QColorDialog::ShowAlphaChannel);
    if (selectedColor.isValid()) {
        setBrushColor(selectedColor);
    }
}

QColor ToolBarExtandPenWidget::brushColor() const
{
    return m_brushColor;
}

void ToolBarExtandPenWidget::setBrushColor(const QColor &newBrushColor)
{
    if (m_brushColor == newBrushColor)
        return;
    m_brushColor = newBrushColor;
    emit brushColorChanged();
    QString colorStyle = QString("background-color: #%1").arg(newBrushColor.rgb(), 6, 16, QChar('0'));
    ui->brushButton->setStyleSheet(colorStyle);
}
