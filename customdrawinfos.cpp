#include "customdrawinfos.h"
#include "qwidget.h"

#include <QPainter>

CustomDrawInfos::CustomDrawInfos(QObject *parent)
    : QObject{parent}
{

}

void CustomDrawInfos::reset()
{
    m_customOperatorList.clear();
    m_customOperatorListIndex = 0;
    m_currenterasePoins.clear();
    m_currentDrawPoints.clear();
}

void CustomDrawInfos::undo()
{
    // 撤销
    m_customOperatorListIndex--;
    if (m_customOperatorListIndex < 0)
        m_customOperatorListIndex = 0;

    updateParent();
}

void CustomDrawInfos::redo()
{
    // 回复
    m_customOperatorListIndex++;
    if (m_customOperatorListIndex >=  m_customOperatorList.size())
        m_customOperatorListIndex = m_customOperatorList.size();
    updateParent();
}

void CustomDrawInfos::clearCurrenCache()
{
    m_currentDrawPoints.clear();
}

void CustomDrawInfos::addCurrentErasePoint(QPoint pos, int width)
{
    m_erasePointWidth = width;
    m_currenterasePoins << pos;
    updateParent();
}

void CustomDrawInfos::endCurrentErasePoint(int width)
{
    CustomDrawInfo customDrawInfo;
    customDrawInfo.erasePoints = std::move(m_currenterasePoins);
    customDrawInfo.eraseWidth = m_erasePointWidth;
    addCustomDrawInfo(customDrawInfo);
}

void CustomDrawInfos::addCurrentDrawPoint(QPoint pos)
{
    m_currentDrawPoints << pos;
    updateParent();
}

void CustomDrawInfos::endCurrentDrawPoint(QPen pen, QBrush brush)
{
    CustomDrawInfo customDrawInfo;
    customDrawInfo.drawPoints = std::move(m_currentDrawPoints);
    customDrawInfo.pen = pen;
    customDrawInfo.brush = brush;
    addCustomDrawInfo(customDrawInfo);
}

void CustomDrawInfos::setCurrentDrawRect(QRect rect)
{
    m_currentDrawRect = rect;
    updateParent();
}

void CustomDrawInfos::addCurrentDrawRect(QRect rect, QPen pen, QBrush brush)
{
    CustomDrawInfo customDrawInfo;
    customDrawInfo.drawRect = rect;
    customDrawInfo.pen = pen;
    customDrawInfo.brush = brush;
    addCustomDrawInfo(customDrawInfo);
    m_currentDrawRect.setRect(0,0,0,0);
}

void CustomDrawInfos::paintCustomDrawInfos(QPainter *paintEvent)
{
//    for(auto customDrawInfo : m_customOperatorList ){
    for(int i = 0 ; i < m_customOperatorList.size() && i < m_customOperatorListIndex; i++){
        auto customDrawInfo = m_customOperatorList.at(i);
        paintEvent->setPen(customDrawInfo.pen);
        paintEvent->setBrush(customDrawInfo.brush);
        for(int i = 0 ; i < customDrawInfo.drawPoints.size() - 1; i++){
            paintEvent->drawLine(customDrawInfo.drawPoints[i], customDrawInfo.drawPoints[i+1]);
        }
        if (customDrawInfo.drawRect.width() > 0){
            paintEvent->drawRect(customDrawInfo.drawRect);
        }
        for(int i = 0 ; i < customDrawInfo.erasePoints.size() - 1; i++){
            QRect rect;
            rect.setWidth(customDrawInfo.eraseWidth);
            rect.setHeight(customDrawInfo.eraseWidth);
            rect.moveCenter(customDrawInfo.erasePoints[i]);
            paintEvent->drawPixmap(rect, *m_screenPixmap, rect);
        }
    }
}

void CustomDrawInfos::paintCustomCurrentDrawInfo(QPainter *paintEvent)
{
    for(int i = 0 ; i <m_currentDrawPoints.size() - 1; i++){
        paintEvent->drawLine(m_currentDrawPoints[i], m_currentDrawPoints[i+1]);
    }
    if (m_currentDrawRect.width() > 0){
        paintEvent->drawRect(m_currentDrawRect);
    }
    for(int i = 0 ; i < m_currenterasePoins.size() - 1; i++){
        QRect rect;
        rect.setWidth(m_erasePointWidth);
        rect.setHeight(m_erasePointWidth);
        rect.moveCenter(m_currenterasePoins[i]);
        paintEvent->drawPixmap(rect, *m_screenPixmap, rect);
    }

}

void CustomDrawInfos::setSourcePixMap(QPixmap *pixmap)
{
    m_screenPixmap = pixmap;
}

void CustomDrawInfos::updateParent()
{
    auto parentWidget = qobject_cast<QWidget*>(parent());
    if (parentWidget){
        parentWidget->update();
    }
}

void CustomDrawInfos::addCustomDrawInfo(CustomDrawInfo &info)
{
    // 这个时候又添加了， 所以要吧列表里面的后面的删除
    if(m_customOperatorListIndex != m_customOperatorList.size()){
        m_customOperatorList.remove(m_customOperatorListIndex, m_customOperatorList.size() - m_customOperatorListIndex);
    }

    m_customOperatorListIndex++;
    m_customOperatorList.append(std::move(info));
    m_currentDrawPoints.clear();
    m_currenterasePoins.clear();
    updateParent();
}
