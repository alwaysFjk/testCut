#ifndef CUSTOMDRAWINFOS_H
#define CUSTOMDRAWINFOS_H

#include <QObject>
#include <QPaintEvent>
#include <QPen>


class CustomDrawInfo{
public:
    QList<QPoint> drawPoints;
    QRect drawRect;
    QPen pen;
    QBrush brush;

    QList<QPoint> erasePoints;
    int eraseWidth;
};


class CustomDrawInfos : public QObject
{
    Q_OBJECT
public:
    explicit CustomDrawInfos(QObject *parent = nullptr);

    void reset();

    void undo();
    void redo();
    void clearCurrenCache();

    void addCurrentErasePoint(QPoint pos, int width);
    void endCurrentErasePoint(int width);

    void addCurrentDrawPoint(QPoint pos);
    void endCurrentDrawPoint(QPen pen, QBrush brush);

    void setCurrentDrawRect(QRect rect);
    void addCurrentDrawRect(QRect rect,QPen pen, QBrush brush);

    void paintCustomDrawInfos(QPainter *paintEvent);
    void paintCustomCurrentDrawInfo(QPainter *paintEvent);

    void setSourcePixMap(QPixmap *pixmap);
signals:

protected:
    void updateParent();
    void addCustomDrawInfo(CustomDrawInfo & info);
private:
    int m_erasePointWidth;
    QList<QPoint> m_currenterasePoins;

    QList<QPoint> m_currentDrawPoints;
    QRect m_currentDrawRect;

    QList<CustomDrawInfo> m_customOperatorList;
    int m_customOperatorListIndex = 0;
    QPixmap *m_screenPixmap = nullptr;
};

#endif // CUSTOMDRAWINFOS_H
