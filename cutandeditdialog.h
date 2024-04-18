#ifndef CUTANDEDITDIALOG_H
#define CUTANDEDITDIALOG_H

#include <QWidget>
#include "cutdialog.h"


class ToolBarWidget;
class CustomDrawInfos;
class CutAndEditDialog : public CutDialog
{
    Q_OBJECT
public:
    explicit CutAndEditDialog(QWidget *parent = nullptr);

    virtual void reset();
signals:

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

    void initToolBar();

protected slots:
    void slot_toolBarWidget_toolBarModelChange();
    void updateTooBarPosition();
private:
    ToolBarWidget *m_toolBarWidget = nullptr;
    CustomDrawInfos *m_customDrawInfos = nullptr;
    QPoint m_drawStartPos;
};

#endif // CUTANDEDITDIALOG_H
