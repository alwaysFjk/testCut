#ifndef TOOLBAREXTANDPENWIDGET_H
#define TOOLBAREXTANDPENWIDGET_H

#include <QWidget>
#include "toolbarextandbasewidget.h"

namespace Ui {
class ToolBarExtandPenWidget;
}

class ToolBarExtandPenWidget : public ToolBarExtandBaseWidget
{
    Q_OBJECT

    Q_PROPERTY(QColor penColor READ penColor WRITE setPenColor NOTIFY penColorChanged FINAL)
    Q_PROPERTY(QColor brushColor READ brushColor WRITE setBrushColor NOTIFY brushColorChanged FINAL)
public:
    explicit ToolBarExtandPenWidget(QWidget *parent = nullptr);
    ~ToolBarExtandPenWidget();

    virtual void reset();
    QColor penColor() const;
    void setPenColor(const QColor &newPenColor);
    int penWidth();
    QColor brushColor() const;
    void setBrushColor(const QColor &newBrushColor);

signals:
    void penColorChanged();

    void brushColorChanged();

protected slots:
    void on_colorButton_clicked();

private slots:
    void on_brushButton_clicked();

private:
    Ui::ToolBarExtandPenWidget *ui;
    QColor m_penColor;
    QColor m_brushColor;
};

#endif // TOOLBAREXTANDPENWIDGET_H
