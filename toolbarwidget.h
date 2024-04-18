#ifndef TOOLBARWIDGET_H
#define TOOLBARWIDGET_H

#include "qpushbutton.h"
#include <QWidget>

namespace Ui {
class ToolBarWidget;
}

class ToolBarExtandPenWidget;
class ToolBarRextandEraseWidget;
class ToolBarExtandBaseWidget;
class ToolBarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ToolBarWidget(QWidget *parent = nullptr);
    ~ToolBarWidget();

    enum ToolBarModel{
        None,
        DrawPoint,
        DrawLine,
        DrawRect,
        Erase
    };

    ToolBarModel getToolBarModel();

    void reset();
    int penWidth();
    QColor penColor() const;
    QColor brushColor() const;

    virtual int height();
    virtual int width();
signals:
    void toolBarModelChange();
    void undoBtnClicked();
    void redoBtnClicked();
    void okBtnClicked();
    void cancelBtnClicked();
    void saveBtnClicked();
    void sizeChange();

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void initExtandWidget();
    void updateExtrandWidgetVisiable();
    void updateExtrandWidgetPos();
    void moveEvent(QMoveEvent *);
    void resizeEvent(QResizeEvent *);
    void exclusiveBtn(QObject *sender);

private slots:
    void on_drawPointBtn_clicked();
    void on_drawRectBtn_clicked();
    void on_drawLineBtn_clicked();
    void on_eraseButton_clicked();

private:
    Ui::ToolBarWidget *ui;
    QList<QPushButton*> m_btnGroup;
    QList<ToolBarExtandBaseWidget *> m_toolBarExtandWidgetsList;
    ToolBarExtandPenWidget *m_toolBarExtandPenWidget = nullptr;
    ToolBarRextandEraseWidget *m_toolBarRextandEraseWidget = nullptr;
};

#endif // TOOLBARWIDGET_H
