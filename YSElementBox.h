//
//  YSElementBox.h
//  PeriodicTableWidget
//
//  Created by Yves Schutz on 16/08/13.
//  Copyright (c) 2013 Yves Schutz. All rights reserved.
//

#ifndef YSELEMENTBOX_H
#define YSELEMENTBOX_H

#include <QGraphicsItem>
#include <QtWidgets>

#include "YSElement.h"

class QPropertyAnimation;
class YSIsotopeSheet;

class YSElementBox : public QGraphicsObject
{
    Q_OBJECT

public:
    explicit YSElementBox(YSElement *, QGraphicsScene *parent = NULL);
    virtual ~YSElementBox() ;

    QRectF       boundingRect() const;
    YSElement *  Element() const     { return fElement; }
    QColor       GetPenColor() const { return fPenColor; }
    double       kAngle() const      { return 20.0 * M_PI / 180.; }
    qreal        kD() const          { return 20.0; }
    int          kZOffset() const    { return 10; }
    void         mousePressEvent(QGraphicsSceneMouseEvent *event);
    void         mouseReleaseEvent(QGraphicsSceneMouseEvent *);
    int          NumberOfIsotopes() { return Element()->GetIsotopes().size(); }
    void         paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void         Print();
    void         ShowIsotopeList(QAbstractAnimation::Direction);
    
public slots:
    void         AnimationStateChanged();
    
private:
    void                 SetBoxColor(YSElement::YSElementFamily);
    void                 SetPenColor(YSElement::YSElementState);

    QPropertyAnimation   *fOpenDrawer;
    QColor               fBoxColor;     // filling color of the box
    YSElement            *fElement;     // the element in this box (class is not owner)
    YSIsotopeSheet       **fSheets;     // an array of isotope sheets
    int                  fMouseClicks;  // count the mouse clicks
    QGraphicsScene       *fParent;      // scene where this object is drawn
    QColor               fPenColor;     // color of the pen

};

#endif // YSELEMENTBOX_H
