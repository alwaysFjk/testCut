#ifndef TOOLBAREXTANDBASEWIDGET_H
#define TOOLBAREXTANDBASEWIDGET_H

#include <QWidget>

class ToolBarExtandBaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ToolBarExtandBaseWidget(QWidget *parent = nullptr);

    virtual void reset() = 0;
signals:

};

#endif // TOOLBAREXTANDBASEWIDGET_H
