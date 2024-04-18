#ifndef TOOLBARREXTANDERASEWIDGET_H
#define TOOLBARREXTANDERASEWIDGET_H

#include <QWidget>
#include "toolbarextandbasewidget.h"

namespace Ui {
class ToolBarRextandEraseWidget;
}

class ToolBarRextandEraseWidget : public ToolBarExtandBaseWidget
{
    Q_OBJECT

public:
    explicit ToolBarRextandEraseWidget(QWidget *parent = nullptr);
    ~ToolBarRextandEraseWidget();

    virtual void reset();
    int penWidth();
private:
    Ui::ToolBarRextandEraseWidget *ui;
};

#endif // TOOLBARREXTANDERASEWIDGET_H
