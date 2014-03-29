//
//  YSElementBox.cpp
//  PeriodicTableWidget
//
//  Created by Yves Schutz on 16/08/13.
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
YSElementBox::YSElementBox(YSElement * element, QGraphicsScene * parent)
    : QGraphicsObject(), fElement(element), fMouseClicks(0), fParent(parent)
{
    //ctor initialize a box with an element
    SetBoxColor(element->GetFamily());
    SetPenColor(element->GetState());
    setZValue(kZOffset());

    fSheets = new YSIsotopeSheet*[NumberOfIsotopes()];
    int count = 0;
    foreach (YSIsotope * iso, Element()->GetIsotopes()) {
        fSheets[count]= new YSIsotopeSheet(iso, this);
        fSheets[count]->setPos(pos().x(), pos().y());
        fParent->addItem(fSheets[count]);
        count++;
    }

    // define the animation

    fOpenDrawer = new QPropertyAnimation(this, "pos");

    fOpenDrawer->setDuration(500);

    qreal x = Element()->GetGroupe()  * boundingRect().width();
    qreal y = Element()->GetPeriode() * boundingRect().height();

    fOpenDrawer->setStartValue(QPointF(x, y));

    x += kD() * cos(kAngle());
    y += kD() * sin(kAngle());

    fOpenDrawer->setEndValue(QPointF(x, y));

    connect((QAbstractAnimation*) fOpenDrawer, SIGNAL(stateChanged(QAbstractAnimation::State, QAbstractAnimation::State)), this, SLOT(AnimationStateChanged()));
}

//______________________________________________________________________________
YSElementBox::~YSElementBox()
{
    // dtor
    delete fOpenDrawer;
    delete[] fSheets;
}

//______________________________________________________________________________
void YSElementBox::AnimationStateChanged()
{
    // reset the z value of the box when animation is done
    if( fOpenDrawer->state() == QAbstractAnimation::Stopped && fMouseClicks == 2) {
        setZValue(zValue() - kZOffset());
        fMouseClicks = 0;
        for(int iso = 0; iso < NumberOfIsotopes(); iso++) {
            fSheets[iso]->setZValue(kZOffset()-1);
        }
    }
}

//______________________________________________________________________________
QRectF YSElementBox::boundingRect() const
{
    // define the bounding rectangle of the box
    qreal width = 55.0, height = 55.0;
    return QRectF (0, 0, width, height);
}

//______________________________________________________________________________
void YSElementBox::mousePressEvent(QGraphicsSceneMouseEvent */*event*/)
{
    // shows on the Periodic Table (scene) the sheets of isotopes
    if (Element()->Name() == YSElement::kDummy)
        return;
    fMouseClicks++;
    QAbstractAnimation::Direction direction = QAbstractAnimation::Forward;
    if (fMouseClicks == 1) {
        setZValue(zValue() + kZOffset());
    } else if (fMouseClicks == 2) {
        direction = QAbstractAnimation::Backward;
   }
    ShowIsotopeList(direction);
}

//______________________________________________________________________________
void YSElementBox::mouseReleaseEvent(QGraphicsSceneMouseEvent */*event*/)
{
//    fOpenDrawer->setDirection(QAbstractAnimation::Backward);
//    fOpenDrawer->start();
}

//______________________________________________________________________________
void YSElementBox::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    // paints the box
    const int kPointSize = 12;
    // draw the element symbol within a rectangle
    // the bounding rectangle
    painter->setBrush(fBoxColor);
    painter->drawRect(boundingRect().adjusted(0,0,0,0));
    painter->setPen(fPenColor);

    QFont font;
    font.setWeight(QFont::Bold);
    font.setStyleHint(QFont::SansSerif);
    font.setPointSize(kPointSize);
    painter->setFont(font);
    painter->drawText(boundingRect(), Qt::AlignCenter, QString("%1\n ").arg(fElement->Symbol()));

    font.setPointSize(kPointSize/1.3);
    painter->setFont(font);
    if ( fElement->Z() >= 0 ) {
        painter->drawText(boundingRect(),Qt::AlignTop,QString(" %1").arg(fElement->Z()));

        font.setPointSize(kPointSize/1.4);
        painter->setFont(font);
        YSIsotope * iso = fElement->MostAbundantIsotope();
        if (iso) {
            if ( iso->Abundance() == 0) {
                painter->drawText(boundingRect(),Qt::AlignBottom|Qt::AlignHCenter,QString(" %1\n(%2)").arg(fElement->Name()).arg(iso->AtomicMass()));
            } else {
                painter->drawText(boundingRect(),Qt::AlignBottom|Qt::AlignHCenter, QString(" %1\n%2").arg(fElement->Name()).arg(iso->AtomicMass()));
            }
        }
    } else if ( fElement->Z() == -2) {
        painter->drawText(boundingRect(),Qt::AlignTop,QString(" Z"));
        painter->drawText(boundingRect(),Qt::AlignBottom|Qt::AlignHCenter,QString(" Name\nat. mass"));
    }
}

//______________________________________________________________________________
void YSElementBox::Print()
{
    // Print the element
    QString message = QString("\n%1:").arg(metaObject()->className());
    qDebug() << message;
    fElement->Print();
}

//______________________________________________________________________________
void YSElementBox::SetBoxColor(YSElement::YSElementFamily family)
{
    //set color according to family
    int red = 255, green = 255, blue = 255;
    switch (family) {
    case YSElement::kMetalloid:
        red   = 0;
        green = 190;
        blue  = 113;
        break;
    case YSElement::kOtherNonMetal:
        red   = 0;
        green = 255;
        blue  = 0;
        break;
    case YSElement::kHalogen:
        red   = 112;
        green = 210;
        blue  = 171;
        break;
    case YSElement::kNobleGas:
        red   = 152;
        green = 217;
        blue  = 251;
        break;
    case YSElement::KAlkaliMetal:
        red   = 247;
        green = 150;
        blue  = 70;
        break;
    case YSElement::kAlkalineEarthMetal:
        red   = 255;
        green = 255;
        blue  = 0;
        break;
    case YSElement::kTransitionMetal:
        red   = 217;
        green = 150;
        blue  = 148;
        break;
    case YSElement::kPostTransitionMetal:
        red   = 195;
        green = 214;
        blue  = 155;
        break;
    case YSElement::kLanthanoid:
        red   = 250;
        green = 192;
        blue  = 144;
        break;
    case YSElement::kActinoid:
        red   = 250;
        green = 192;
        blue  = 242;
        break;
    default:
        break;
    }
    fBoxColor.setRgb(red, green, blue);



}

//______________________________________________________________________________
void YSElementBox::SetPenColor(YSElement::YSElementState /*state*/)
{
    //set the pen color according to the state of the element

    switch (fElement->GetState()) {
    case YSElement::kUnknown:
        fPenColor = QColor(127, 127, 127);
        break;
    case YSElement::kSolid:
        fPenColor = Qt::black;
        break;
    case YSElement::kLiquid:
        fPenColor = Qt::blue;
        break;
    case YSElement::kGas:
        fPenColor = Qt::red;
        break;
    case YSElement::kNoState:
         fPenColor = Qt::black;
        break;
    default:
        break;
    }
}

//______________________________________________________________________________
void YSElementBox::ShowIsotopeList(QAbstractAnimation::Direction direction)
{
    // displays the isotopes of given element

    // open the drawer

    qreal xstart = Element()->GetGroupe()  * boundingRect().width();
    qreal ystart = Element()->GetPeriode() * boundingRect().height();

    const qreal kOffset = 15;

    qreal x = xstart - kOffset;
    qreal y = ystart - (NumberOfIsotopes() + 1) * kOffset;

    // open/close the drawer

    setZValue(zValue()+2) ;
    for(int iso = 0; iso < NumberOfIsotopes(); iso++) {
        fSheets[iso]->setZValue(kZOffset()+1);
        x += kD() / NumberOfIsotopes() * cos(kAngle());
        y += kD() / NumberOfIsotopes() * sin(kAngle()) + kOffset;
        fSheets[iso]->Animate(QPointF(xstart, ystart), QPointF(x,y), direction);
    }
    fOpenDrawer->setDirection(direction);
    fOpenDrawer->start();
    setZValue(zValue()-2) ;
}
