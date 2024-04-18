#ifndef CUTDIALOG_H
#define CUTDIALOG_H

#include "qlabel.h"
#include <QDialog>
#include <QMouseEvent>
#include <QElapsedTimer>

namespace Ui {
class CutDialog;
}

class CustomDrawInfos;
class CutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CutDialog(QWidget *parent = nullptr);
    ~CutDialog();

    virtual void reset();
signals:
    void screenPixmapChange();

public slots:
    void savePixmapToClipBoard();
    bool savePixmapToLoacl();

protected:
    void showEvent(QShowEvent *);
    void paintEvent(QPaintEvent *event);
    virtual void paintScreenPixMap(QPainter &painter);
    virtual void paintMask(QPainter &painter);
    virtual void paintBlueRect(QPainter &painter);

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseMoveEventForAdjustBlueRect(QMouseEvent *event);
    void mouseMoveEventForResizeBlueRect(QMouseEvent *event);

    void mouseDoubleClickEvent(QMouseEvent *event);
    int mouseUnderBuleBorder(QPoint pos);

    void updateScreenToPixmap();

protected:
    Ui::CutDialog *ui;

    QPixmap m_screenPixmap;
    QRect m_blueRect;
    QPoint m_lastPoint;
    int m_mouseMode = 0;

#ifdef QT_DEBUG
    QElapsedTimer m_frameTime;
    int m_frameCount = 0 ;
    float m_frameRate = 0;
#endif

};

#endif // CUTDIALOG_H
