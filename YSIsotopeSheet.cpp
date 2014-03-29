//
//  YSIsotopeSheet.cpp
//  PeriodicTableWidget
//
//  Created by Yves Schutz on 02/09/13.
//  Copyright (c) 2013 Yves Schutz. All rights reserved.
//

#include <QDebug>
#include <QListWidget>
#include <QMessageBox>
#include <QPainter>
#include <QPropertyAnimation>

#include "YSElementBox.h"
#include "YSIsotope.h"
#include "YSIsotopeSheet.h"
#include "YSPeriodictable.h"

//______________________________________________________________________________
YSIsotopeSheet::YSIsotopeSheet(YSIsotope * iso, YSElementBox * parent)
    : QGraphicsObject(), fIsotope(iso)
{
    //ctor initialize a sheet wit one isotope

    fElementBox = parent ;

    fDrawOutOfDrawer = new QPropertyAnimation(this, "pos");
    fDrawOutOfDrawer->setDuration(500);

    connect(this, SIGNAL(Select()), fIsotope, SLOT(Selected()));
}

//______________________________________________________________________________
void YSIsotopeSheet::Animate(QPointF start, QPointF end, QAbstractAnimation::Direction direction)
{
    // draws the sheet out of the drawer
    fDrawOutOfDrawer->setStartValue(start);
    fDrawOutOfDrawer->setEndValue(end);
    fDrawOutOfDrawer->setDirection(direction);
    fDrawOutOfDrawer->start();
}

//______________________________________________________________________________
QRectF YSIsotopeSheet::boundingRect() const
{
    // define the bounding rectangle of the sheet
    qreal width = 55.0, height = 55.0;
    return QRectF (0, 0, width, height);
}

//______________________________________________________________________________
void YSIsotopeSheet::mouseDoubleClickEvent(QGraphicsSceneMouseEvent */*event*/)
{
    emit Select();
}

//______________________________________________________________________________
void YSIsotopeSheet::mousePressEvent(QGraphicsSceneMouseEvent */*event*/)
{
    setScale(5);
    setZValue(zValue()+100);
}

//______________________________________________________________________________
void YSIsotopeSheet::mouseReleaseEvent(QGraphicsSceneMouseEvent */*event*/)
{
    setScale(1);
    setZValue(zValue()-100);
}

//______________________________________________________________________________
void YSIsotopeSheet::paint(QPainter *painter, const QStyleOptionGraphicsItem  */*option*/, QWidget */*widget*/)
{
    // paints the sheet
    const int kPointSize = 12;
    painter->setBrush(Qt::lightGray);
    painter->drawRect(boundingRect());
    painter->setPen(Qt::black);

    QFont font;
    font.setWeight(QFont::Bold);
    font.setStyleHint(QFont::SansSerif);
    font.setPointSize(kPointSize/1.4);

    QTextOption opt;
    opt.setAlignment(Qt::AlignCenter);

    QTextDocument doc;
    doc.setDefaultFont(font);
    doc.setDefaultTextOption(opt);
    doc.setHtml(QString("<p align=center>A=%1<br><sub>%2</sub>%3<sub>%4</sub><br>M=%5<br>%6\%</p>").arg(fIsotope->A()).arg(fIsotope->Z()).arg(fIsotope->Symbol()).arg(fIsotope->N()).arg(fIsotope->AtomicMass()).arg(fIsotope->Abundance()));
    doc.drawContents(painter, boundingRect());

}
