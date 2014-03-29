//
//  YSElementBox.h
//  PeriodicTableWidget
//
//  Created by Yves Schutz on 02/09/13.
//  Copyright (c) 2013 Yves Schutz. All rights reserved.
//

#ifndef YSISOTOPESHEET_H
#define YSISOTOPESHEET_H

#include <QGraphicsItem>
#include <QtWidgets>

class QPointF;
class QPropertyAnimation;
class YSElementBox;
class YSIsotope;

class YSIsotopeSheet: public QGraphicsObject
{
    Q_OBJECT

public:
    explicit YSIsotopeSheet(YSIsotope *, YSElementBox *parent = NULL);
    virtual ~YSIsotopeSheet() { delete fDrawOutOfDrawer; }

    void        Animate(QPointF, QPointF, QAbstractAnimation::Direction);
    QRectF      boundingRect() const;
    YSIsotope * Isotope() const { return fIsotope; }
    void        paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void        mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void        mousePressEvent(QGraphicsSceneMouseEvent *event);
    void        mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

signals:
    void        Select();

private:
    YSElementBox       *fElementBox;         // element of this isotope
    YSIsotope          *fIsotope;         // the isotope on this sheet (class is not owner)
    QPropertyAnimation *fDrawOutOfDrawer; // animation taking sheet out of YSElementBox
};

#endif // YSISOTOPESHEET_H
