//
//  YSPeriodicTable.cpp
//  PeriodicTableWidget
//
//  Created by Yves Schutz on 14/08/13.
//  Copyright (c) 2013 Yves Schutz. All rights reserved.
//

#define _USE_MATH_DEFINES

#include <cmath>
#include "YSPeriodictable.h"
#include "YSElement.h"
#include "YSElementBox.h"
#include "YSIsotope.h"
#include "YSIsotopeSheet.h"

//______________________________________________________________________________
YSPeriodicTable::YSPeriodicTable(QGraphicsView *parent)
    :QGraphicsScene(parent)
{
    // set up the periodic table in a Scene of a QGraphicsView

    YSIsotope * dummy = new YSIsotope("", "", 0,0,0.,0.);
    connect(dummy, SIGNAL(Select(YSIsotope*)), this, SLOT(SelectedIsotope(YSIsotope*)));

    FillPeriodicTable();
    AddLegend();
    Draw();
    ConnectElements();
}

//______________________________________________________________________________
YSPeriodicTable::~YSPeriodicTable()
{
    qDeleteAll(fElements);
    fElements.clear();
    qDeleteAll(items());
    clear();
}

//______________________________________________________________________________
void YSPeriodicTable::AddElementBox(YSElementBox *item)
{
    //add element into the scene
    addItem(item);
    item->setPos( item->Element()->GetGroupe()  * item->boundingRect().width(),
                  item->Element()->GetPeriode() * item->boundingRect().height());


}

//______________________________________________________________________________
void YSPeriodicTable::AddLegend()
{
    //add elements for the legend

    YSElement *element = new YSElement("Symb", YSElement::kDummy, -2);
    element->SetPeriodGroup(1, 2);
    element->SetState(YSElement::kNoState);
    element->SetFamily(YSElement::kNoFamily);

    YSElementBox *box = new YSElementBox(element);
    box->setPos( box->Element()->GetGroupe() * box->boundingRect().width(),
                 box->Element()->GetPeriode() * box->boundingRect().height());
    addItem(box);

    qreal xbeg = box->boundingRect().width()*3;
    qreal xend = box->boundingRect().width()*4;
    qreal ybeg = box->boundingRect().height()*6;
    qreal yend = box->boundingRect().height()*6;
    addLine(xbeg, ybeg, xend, yend, QPen(QBrush(Qt::red),1))->setZValue(99);

    xbeg = xend ;
    ybeg = yend ;
    yend += box->boundingRect().height()*3;
    addLine(xbeg, ybeg, xend, yend, QPen(QBrush(Qt::red),1))->setZValue(99);

    xbeg -= box->boundingRect().width();
    xend = xbeg;
    yend += box->boundingRect().height()*2;
    addLine(xbeg, ybeg, xend, yend, QPen(QBrush(Qt::red),1))->setZValue(99);

    xend += box->boundingRect().width();
    ybeg = yend;
    addLine(xbeg, ybeg, xend, yend, QPen(QBrush(Qt::red),1))->setZValue(99);

    element = new YSElement("\nC", YSElement::kDummy, -1);
    element->SetState(YSElement::kSolid);

    box = new YSElementBox(element);
    box->setScale(0.7);
    int posx = box->boundingRect().width()*3.2, posy = box->boundingRect().height();
    box->setPos(posx, posy);
    addItem(box);

    QGraphicsTextItem * textItem = addText("Solid");
    textItem->setDefaultTextColor(box->GetPenColor());
    textItem->setPos(posx+box->boundingRect().width(),posy+box->boundingRect().height()/5.);

    element = new YSElement("\nHg", YSElement::kDummy, -1);
    element->SetState(YSElement::kLiquid);

    box = new YSElementBox(element);
    box->setScale(0.7);
    posy = box->boundingRect().height()*1.8;
    box->setPos(posx, posy);
    addItem(box);

    textItem = addText("Liquid");
    textItem->setDefaultTextColor(box->GetPenColor());
    textItem->setPos(posx+box->boundingRect().width(),posy+box->boundingRect().height()/5.);

    element = new YSElement("\nH", YSElement::kDummy, -1);
    element->SetState(YSElement::kGas);

    box = new YSElementBox(element);
    box->setScale(0.7);
    posy = box->boundingRect().height()*2.6;
    box->setPos(posx, posy);
    addItem(box);

    textItem = addText("Gas");
    textItem->setDefaultTextColor(box->GetPenColor());
    textItem->setPos(posx+box->boundingRect().width(),posy+box->boundingRect().height()/5.);

}

//______________________________________________________________________________
void YSPeriodicTable::ConnectElements()
{
    //connects SIGNAL from element to pt slot

    YSElement * element;
    foreach(element, fElements) {
        connect(element, SIGNAL(IsotopeSelected(YSIsotope*)), this, SLOT(SelectedIsotope(YSIsotope*)));
    }
}

//______________________________________________________________________________
void YSPeriodicTable::Draw()
{
    // Draw the periodic table

    int viewWidth = 1100, viewHeight = 650;
    setSceneRect(0,0,viewWidth, viewHeight);

    YSElement * element;
    YSElementBox * box;
    foreach(element, fElements) {
        box = new YSElementBox(element, this);
        addItem(box);
        box->setPos( box->Element()->GetGroupe() * box->boundingRect().width(),
                     box->Element()->GetPeriode() * box->boundingRect().height());
    }
    element = new YSElement("57-71", YSElement::kDummy, -1);
    element->SetPeriodGroup(6, 3);
    element->SetState(YSElement::kNoState);
    element->SetFamily(YSElement::kNoFamily);

    box = new YSElementBox(element);
    addItem(box);
    box->setPos( box->Element()->GetGroupe() * box->boundingRect().width(),
                 box->Element()->GetPeriode() * box->boundingRect().height());

    element = new YSElement("89-103", YSElement::kDummy, -1);
    element->SetPeriodGroup(7, 3);
    element->SetState(YSElement::kNoState);
    element->SetFamily(YSElement::kNoFamily);

    box = new YSElementBox(element);
    addItem(box);
    box->setPos( box->Element()->GetGroupe() * box->boundingRect().width(),
                 box->Element()->GetPeriode() * box->boundingRect().height());

}

//______________________________________________________________________________
void YSPeriodicTable::FillPeriodicTable()
{
    // Fill the periodic table with elements


    // Neutron
    int eCharge = 0;
    YSElement * neutronium      = new YSElement("n", "neutron", eCharge);
    neutronium->SetReference("http://en.wikipedia.org/wiki/Neutron");

    neutronium->AddIsotope(new YSIsotope(neutronium->Symbol(), "neutron", 0, 1, 1.00866491597, 0.));

    fElements.insert(eCharge, neutronium);

    neutronium->SetPeriodGroup(0, 0);
    neutronium->SetState(YSElement::kNoState);
    neutronium->SetFamily(YSElement::kNoFamily);

    // Hydrogen
    eCharge = 1;
    YSElement     * hydrogen  = new YSElement("H", "hydrogen", eCharge);
    hydrogen->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_hydrogen");

    hydrogen->AddIsotope(new YSIsotope("p", "proton", hydrogen->Z(), 1, 1.00794, 0.999885));
    hydrogen->AddIsotope(new YSIsotope("D", "deuterium", hydrogen->Z(), 2, 2.014102, 0.000115));

    fElements.insert(eCharge, hydrogen);
    hydrogen->SetPeriodGroup(1, 1);
    hydrogen->SetState(YSElement::kGas);
    hydrogen->SetFamily(YSElement::kOtherNonMetal);

    // Helium
    eCharge = 2;
    YSElement * helium      = new YSElement("He", "helium", eCharge);
    helium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_helium");

    helium->AddIsotope(new YSIsotope(helium->Symbol(), "alpha", helium->Z(), 4, 4.00260325415, 0.99999866));
    helium->AddIsotope(new YSIsotope(helium->Symbol(), helium->Name(), helium->Z(), 3, 3.0160293191, 1.34E-6));

    fElements.insert(eCharge, helium);
    helium->SetPeriodGroup(1, 18);
    helium->SetState(YSElement::kGas);
    helium->SetFamily(YSElement::kNobleGas);

    // Lithium
    eCharge = 3;
    YSElement * lithium      = new YSElement("Li", "lithium", eCharge);
    lithium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_lithium");

    lithium->AddIsotope(new YSIsotope(lithium->Symbol(), lithium->Name(), lithium->Z(), 7, 7.01600455, 0.9241));
    lithium->AddIsotope(new YSIsotope(lithium->Symbol(), lithium->Name(), lithium->Z(), 6, 6.015122795, 0.0759));

    fElements.insert(eCharge, lithium);
    lithium->SetPeriodGroup(2, 1);
    lithium->SetState(YSElement::kSolid);
    lithium->SetFamily(YSElement::KAlkaliMetal);

    // Beryllium
    eCharge = 4;
    YSElement * beryllium      = new YSElement("Be", "beryllium", eCharge);
    beryllium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_lithium");

    beryllium->AddIsotope(new YSIsotope(beryllium->Symbol(), beryllium->Name(), beryllium->Z(), 9, 9.0121822, 1.0));

    fElements.insert(eCharge, beryllium);
    beryllium->SetPeriodGroup(2, 2);
    beryllium->SetState(YSElement::kSolid);
    beryllium->SetFamily(YSElement::kAlkalineEarthMetal);

    // Boron
    eCharge = 5;
    YSElement * boron      = new YSElement("B", "boron", eCharge);
    boron->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_boron");

    boron->AddIsotope(new YSIsotope(boron->Symbol(), boron->Name(), boron->Z(), 10, 10.0129370, 0.199));
    boron->AddIsotope(new YSIsotope(boron->Symbol(), boron->Name(), boron->Z(), 11, 11.0093054, 0.801));

    fElements.insert(eCharge, boron);
    boron->SetPeriodGroup(2, 13);
    boron->SetState(YSElement::kSolid);
    boron->SetFamily(YSElement::kMetalloid);

    // Carbon
    eCharge = 6;
    YSElement * carbon      = new YSElement("C", "carbon", eCharge);
    carbon->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_carbon");

    carbon->AddIsotope(new YSIsotope(carbon->Symbol(), carbon->Name(), carbon->Z(), 12, 12.0, 0.9893));
    carbon->AddIsotope(new YSIsotope(carbon->Symbol(), carbon->Name(), carbon->Z(), 13, 13.0033548378,  0.0107));

    fElements.insert(eCharge, carbon);
    carbon->SetPeriodGroup(2, 14);
    carbon->SetState(YSElement::kSolid);
    carbon->SetFamily(YSElement::kOtherNonMetal);

    // Nitrogen
    eCharge = 7;
    YSElement * nitrogen      = new YSElement("N", "nitrogen", eCharge);
    nitrogen->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_nitrogen");

    nitrogen->AddIsotope(new YSIsotope(nitrogen->Symbol(), nitrogen->Name(), nitrogen->Z(), 14, 14.0030740048, 0.99636));
    nitrogen->AddIsotope(new YSIsotope(nitrogen->Symbol(), nitrogen->Name(), nitrogen->Z(), 15, 15.0001088982, 0.00364));

    fElements.insert(eCharge, nitrogen);
    nitrogen->SetPeriodGroup(2, 15);
    nitrogen->SetState(YSElement::kGas);
    nitrogen->SetFamily(YSElement::kOtherNonMetal);

    // Oxygen
    eCharge= 8;
    YSElement * oxygen      = new YSElement("O", "oxygen", eCharge);
    oxygen->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_oxygen");

    oxygen->AddIsotope(new YSIsotope(oxygen->Symbol(), oxygen->Name(), oxygen->Z(), 16, 15.99491461956, 0.99757) );
    oxygen->AddIsotope(new YSIsotope(oxygen->Symbol(), oxygen->Name(), oxygen->Z(), 17, 16.99913170, 3.4E-4));
    oxygen->AddIsotope(new YSIsotope(oxygen->Symbol(), oxygen->Name(), oxygen->Z(), 18, 17.9991610, 2.05E-3));

    fElements.insert(eCharge, oxygen);
    oxygen->SetPeriodGroup(2, 16);
    oxygen->SetState(YSElement::kGas);
    oxygen->SetFamily(YSElement::kOtherNonMetal);

    // Fluorine
    eCharge = 9;
    YSElement * fluorin      = new YSElement("F", "fluorin", eCharge);
    fluorin->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_fluorin");

    fluorin->AddIsotope(new YSIsotope(fluorin->Symbol(), fluorin->Name(), fluorin->Z(), 19, 18.99840322, 1.0));

    fElements.insert(eCharge, fluorin);
    fluorin->SetPeriodGroup(2, 17);
    fluorin->SetState(YSElement::kGas);
    fluorin->SetFamily(YSElement::kHalogen);

    // Neon
    eCharge = 10;
    YSElement * neon      = new YSElement("Ne", "neon", eCharge);
    neon->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_neon");

    neon->AddIsotope(new YSIsotope(neon->Symbol(), neon->Name(), neon->Z(), 20, 19.9924401754, 0.9048));
    neon->AddIsotope(new YSIsotope(neon->Symbol(), neon->Name(), neon->Z(), 21, 20.99384668, 0.0027));

    neon->AddIsotope(new YSIsotope(neon->Symbol(), neon->Name(), neon->Z(), 22, 21.991385114, 0.0925));

    fElements.insert(eCharge, neon);
    neon->SetPeriodGroup(2, 18);
    neon->SetState(YSElement::kGas);
    neon->SetFamily(YSElement::kNobleGas);

    // Sodium
    eCharge = 11;
    YSElement * sodium      = new YSElement("Na", "sodium", eCharge);
    sodium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_sodium");

    sodium->AddIsotope(new YSIsotope(sodium->Symbol(), sodium->Name(), sodium->Z(),  23, 22.9897692809, 1.0));

    fElements.insert(eCharge, sodium);
    sodium->SetPeriodGroup(3, 1);
    sodium->SetState(YSElement::kSolid);
    sodium->SetFamily(YSElement::KAlkaliMetal);

    // Magnesium
    eCharge = 12;
    YSElement * magnesium      = new YSElement("Mg", "magnesium", eCharge);
    magnesium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_magnesium");

    magnesium->AddIsotope(new YSIsotope(magnesium->Symbol(), magnesium->Name(), magnesium->Z(), 24, 23.985041700, 0.7899));
    magnesium->AddIsotope(new YSIsotope(magnesium->Symbol(), magnesium->Name(), magnesium->Z(), 25, 24.98583692, 0.1000));
    magnesium->AddIsotope(new YSIsotope(magnesium->Symbol(), magnesium->Name(), magnesium->Z(), 26, 25.982592929, 0.1101));

    fElements.insert(eCharge, magnesium);
    magnesium->SetPeriodGroup(3, 2);
    magnesium->SetState(YSElement::kSolid);
    magnesium->SetFamily(YSElement::kAlkalineEarthMetal);

    // Aluminium
    eCharge = 13;
    YSElement * aluminium      = new YSElement("Al", "aluminium", eCharge);
    aluminium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_aluminium");

    aluminium->AddIsotope(new YSIsotope(aluminium->Symbol(), aluminium->Name(), aluminium->Z(),  27, 26.98153863, 1.0));

    fElements.insert(eCharge, aluminium);
    aluminium->SetPeriodGroup(3, 13);
    aluminium->SetState(YSElement::kSolid);
    aluminium->SetFamily(YSElement::kPostTransitionMetal);

    // Silicon
    eCharge = 14;
    YSElement * silicon      = new YSElement("Si", "silicon", eCharge);
    silicon->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_silicon");

    silicon->AddIsotope(new YSIsotope(silicon->Symbol(), silicon->Name(), silicon->Z(), 28, 27.9769265325, 0.92223));
    silicon->AddIsotope(new YSIsotope(silicon->Symbol(), silicon->Name(), silicon->Z(), 29, 28.976494700, 0.04685));
    silicon->AddIsotope(new YSIsotope(silicon->Symbol(), silicon->Name(), silicon->Z(), 30, 29.97377017, 0.03092));

    fElements.insert(eCharge, silicon);
    silicon->SetPeriodGroup(3, 14);
    silicon->SetState(YSElement::kSolid);
    silicon->SetFamily(YSElement::kMetalloid);

    // Phosphorus
    eCharge = 15;
    YSElement * phosphorus      = new YSElement("P", "phosphorus", eCharge);
    phosphorus->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_phosphorus");

    phosphorus->AddIsotope(new YSIsotope(phosphorus->Symbol(), phosphorus->Name(), phosphorus->Z(), 31, 30.97376163, 1.0));

    fElements.insert(eCharge, phosphorus);
    phosphorus->SetPeriodGroup(3, 15);
    phosphorus->SetState(YSElement::kSolid);
    phosphorus->SetFamily(YSElement::kOtherNonMetal);

    // Sulfur
    eCharge = 16;
    YSElement * sulfur      = new YSElement("S", "sulfur", eCharge);
    sulfur->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_sulfur");

    sulfur->AddIsotope(new YSIsotope(sulfur->Symbol(), sulfur->Name(), sulfur->Z(),  32, 31.97207100, 0.9493));
    sulfur->AddIsotope(new YSIsotope(sulfur->Symbol(), sulfur->Name(), sulfur->Z(), 33, 32.97145876, 0.0076));
    sulfur->AddIsotope(new YSIsotope(sulfur->Symbol(), sulfur->Name(), sulfur->Z(), 34, 33.96786690, 0.0429));
    sulfur->AddIsotope(new YSIsotope(sulfur->Symbol(), sulfur->Name(), sulfur->Z(), 36, 35.96708076, 2E-4));

    fElements.insert(eCharge, sulfur);
    sulfur->SetPeriodGroup(3, 16);
    sulfur->SetState(YSElement::kSolid);
    sulfur->SetFamily(YSElement::kOtherNonMetal);

    // Chlorine
    eCharge = 17;
    YSElement * chlorine      = new YSElement("Cl", "chlorine", eCharge);
    chlorine->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_clorine");

    chlorine->AddIsotope(new YSIsotope(chlorine->Symbol(), chlorine->Name(), chlorine->Z(),  35, 34.96885268, 0.7576));
    chlorine->AddIsotope(new YSIsotope(chlorine->Symbol(), chlorine->Name(), chlorine->Z(), 37, 36.96590259, 0.2424));

    fElements.insert(eCharge, chlorine);
    chlorine->SetPeriodGroup(3, 17);
    chlorine->SetState(YSElement::kGas);
    chlorine->SetFamily(YSElement::kHalogen);

    // Argon
    eCharge = 18;
    YSElement * argon      = new YSElement("Ar", "argon", eCharge);
    argon->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_argon");

    argon->AddIsotope(new YSIsotope(argon->Symbol(), argon->Name(), argon->Z(),  36, 35.967545106, 0.003336));
    argon->AddIsotope(new YSIsotope(argon->Symbol(), argon->Name(), argon->Z(), 38, 37.9627324, 6.29E-4));
    argon->AddIsotope(new YSIsotope(argon->Symbol(), argon->Name(), argon->Z(), 40, 39.9623831225, 0.996035));

    fElements.insert(eCharge, argon);
    argon->SetPeriodGroup(3, 18);
    argon->SetState(YSElement::kGas);
    argon->SetFamily(YSElement::kNobleGas);

    // Potassium
    eCharge = 19;
    YSElement * potassium      = new YSElement("K", "potassium", eCharge);
    potassium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_potassium");

    potassium->AddIsotope(new YSIsotope(potassium->Symbol(), potassium->Name(), potassium->Z(),  39, 38.96370668, 0.932581));
    potassium->AddIsotope(new YSIsotope(potassium->Symbol(), potassium->Name(), potassium->Z(), 41, 40.96182576, 0.067302));

    fElements.insert(eCharge, potassium);
    potassium->SetPeriodGroup(4, 1);
    potassium->SetState(YSElement::kSolid);
    potassium->SetFamily(YSElement::KAlkaliMetal);

    // Calcium
    eCharge = 20;
    YSElement * calcium      = new YSElement("Ca", "calcium", eCharge);
    calcium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_calcium");

    calcium->AddIsotope(new YSIsotope(calcium->Symbol(), calcium->Name(), calcium->Z(),  40, 39.96259098, 0.96941));
    calcium->AddIsotope(new YSIsotope(calcium->Symbol(), calcium->Name(), calcium->Z(), 42, 41.95861801, 0.00647));
    calcium->AddIsotope(new YSIsotope(calcium->Symbol(), calcium->Name(), calcium->Z(), 43, 42.9587666, 0.00135));
    calcium->AddIsotope(new YSIsotope(calcium->Symbol(), calcium->Name(), calcium->Z(), 44, 43.9554818, 0.02086));
    calcium->AddIsotope(new YSIsotope(calcium->Symbol(), calcium->Name(), calcium->Z(), 46, 45.9536926, 4E-5));

    fElements.insert(eCharge, calcium);
    calcium->SetPeriodGroup(4, 2);
    calcium->SetState(YSElement::kSolid);
    calcium->SetFamily(YSElement::kAlkalineEarthMetal);

    // Scandium
    eCharge = 21;
    YSElement * scandium      = new YSElement("Sc", "scandium", eCharge);
    scandium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_scandium");

    scandium->AddIsotope(new YSIsotope(scandium->Symbol(), scandium->Name(), scandium->Z(),  45, 44.9559119, 1.0));

    fElements.insert(eCharge, scandium);
    scandium->SetPeriodGroup(4, 3);
    scandium->SetState(YSElement::kSolid);
    scandium->SetFamily(YSElement::kTransitionMetal);

    // Titanium
    eCharge = 22;
    YSElement * titanium      = new YSElement("Ti", "titanium", eCharge);
    titanium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_titanium");

    titanium->AddIsotope(new YSIsotope(titanium->Symbol(), titanium->Name(), titanium->Z(), 46, 45.9526316, 0.0825));
    titanium->AddIsotope(new YSIsotope(titanium->Symbol(), titanium->Name(), titanium->Z(), 47, 46.9517631, 0.0744));
    titanium->AddIsotope(new YSIsotope(titanium->Symbol(), titanium->Name(), titanium->Z(), 48, 47.9479463, 0.7372));
    titanium->AddIsotope(new YSIsotope(titanium->Symbol(), titanium->Name(), titanium->Z(), 49, 48.9478700, 0.0541));
    titanium->AddIsotope(new YSIsotope(titanium->Symbol(), titanium->Name(), titanium->Z(), 50, 49.9447912, 0.0518));

    fElements.insert(eCharge, titanium);
    titanium->SetPeriodGroup(4, 4);
    titanium->SetState(YSElement::kSolid);
    titanium->SetFamily(YSElement::kTransitionMetal);

    // Vanadium
    eCharge = 23;
    YSElement * vanadium      = new YSElement("V", "vanadium", eCharge);
    vanadium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_vanadium");

    vanadium->AddIsotope(new YSIsotope(vanadium->Symbol(), vanadium->Name(), vanadium->Z(), 50, 49.9471585, 0.00250));
    vanadium->AddIsotope(new YSIsotope(vanadium->Symbol(), vanadium->Name(), vanadium->Z(), 51, 50.9439595, 0.99750));

    fElements.insert(eCharge, vanadium);
    vanadium->SetPeriodGroup(4, 5);
    vanadium->SetState(YSElement::kSolid);
    vanadium->SetFamily(YSElement::kTransitionMetal);

    // Chromium
    eCharge = 24;
    YSElement * chromium      = new YSElement("Cr", "chromium", eCharge);
    chromium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_chromium");

    chromium->AddIsotope(new YSIsotope(chromium->Symbol(), chromium->Name(), chromium->Z(), 50, 49.9460442, 0.04345));
    chromium->AddIsotope(new YSIsotope(chromium->Symbol(), chromium->Name(), chromium->Z(), 52, 51.9405075, 0.83789));
    chromium->AddIsotope(new YSIsotope(chromium->Symbol(), chromium->Name(), chromium->Z(), 53, 52.9406494, 0.09501));
    chromium->AddIsotope(new YSIsotope(chromium->Symbol(), chromium->Name(), chromium->Z(), 54, 53.9388804, 0.02365));

    fElements.insert(eCharge, chromium);
    chromium->SetPeriodGroup(4, 6);
    chromium->SetState(YSElement::kSolid);
    chromium->SetFamily(YSElement::kTransitionMetal);

    // Manganese
    eCharge = 25;
    YSElement * manganese      = new YSElement("Mn", "manganese", eCharge);
    manganese->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_manganese");

    manganese->AddIsotope(new YSIsotope(manganese->Symbol(), manganese->Name(), manganese->Z(), 55, 54.9380451, 1.0));

    fElements.insert(eCharge, manganese);
    manganese->SetPeriodGroup(4, 7);
    manganese->SetState(YSElement::kSolid);
    manganese->SetFamily(YSElement::kTransitionMetal);

    // Iron
    eCharge = 26;
    YSElement * iron      = new YSElement("Fe", "iron", eCharge);
    iron->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_iron");

    iron->AddIsotope(new YSIsotope(iron->Symbol(), iron->Name(), iron->Z(), 54, 53.9396105, 0.05845));
    iron->AddIsotope(new YSIsotope(iron->Symbol(), iron->Name(), iron->Z(), 56, 55.9349375, 0.91754));
    iron->AddIsotope(new YSIsotope(iron->Symbol(), iron->Name(), iron->Z(), 57, 56.9353940, 0.02119));
    iron->AddIsotope(new YSIsotope(iron->Symbol(), iron->Name(), iron->Z(), 58, 57.9332756, 0.00282));

    fElements.insert(eCharge, iron);
    iron->SetPeriodGroup(4, 8);
    iron->SetState(YSElement::kSolid);
    iron->SetFamily(YSElement::kTransitionMetal);

    // Cobalt
    eCharge = 27;
    YSElement * cobalt      = new YSElement("Co", "cobalt", eCharge);
    cobalt->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_cobalt");

    cobalt->AddIsotope(new YSIsotope(cobalt->Symbol(), cobalt->Name(), cobalt->Z(), 59, 58.9331950, 1.0));

    fElements.insert(eCharge, cobalt);
    cobalt->SetPeriodGroup(4, 9);
    cobalt->SetState(YSElement::kSolid);
    cobalt->SetFamily(YSElement::kTransitionMetal);

    // Nickel
    eCharge = 28;
    YSElement * nickel      = new YSElement("Ni", "nickel", eCharge);
    nickel->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_nickel");

    nickel->AddIsotope(new YSIsotope(nickel->Symbol(), nickel->Name(), nickel->Z(), 58, 57.9353429, 0.680769));
    nickel->AddIsotope(new YSIsotope(nickel->Symbol(), nickel->Name(), nickel->Z(), 60, 59.9307864, 0.262231));
    nickel->AddIsotope(new YSIsotope(nickel->Symbol(), nickel->Name(), nickel->Z(), 61, 60.9310560, 0.011399));
    nickel->AddIsotope(new YSIsotope(nickel->Symbol(), nickel->Name(), nickel->Z(), 62, 61.9283451, 0.036345));
    nickel->AddIsotope(new YSIsotope(nickel->Symbol(), nickel->Name(), nickel->Z(), 64, 63.9279660, 0.009256));

    fElements.insert(eCharge, nickel);
    nickel->SetPeriodGroup(4, 10);
    nickel->SetState(YSElement::kSolid);
    nickel->SetFamily(YSElement::kTransitionMetal);

    // Copper
    eCharge = 29;
    YSElement  * copper      = new YSElement("Cu", "copper", eCharge);
    copper->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_copper");

    copper->AddIsotope(new YSIsotope(copper->Symbol(), copper->Name(), copper->Z(), 63, 62.9295975, 0.6915));
    copper->AddIsotope(new YSIsotope(copper->Symbol(), copper->Name(), copper->Z(), 65, 64.9277895, 0.3085));

    fElements.insert(eCharge, copper);
    copper->SetPeriodGroup(4, 11);
    copper->SetState(YSElement::kSolid);
    copper->SetFamily(YSElement::kTransitionMetal);

    // Zinc
    eCharge = 30;
    YSElement * zinc      = new YSElement("Zn", "zinc", eCharge);
    zinc->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_zinc");

    zinc->AddIsotope(new YSIsotope(zinc->Symbol(), zinc->Name(), zinc->Z(), 64, 63.9291422, 0.48268));
    zinc->AddIsotope(new YSIsotope(zinc->Symbol(), zinc->Name(), zinc->Z(), 66, 65.9260334, 0.27975));
    zinc->AddIsotope(new YSIsotope(zinc->Symbol(), zinc->Name(), zinc->Z(), 67, 66.9271273, 0.04102));
    zinc->AddIsotope(new YSIsotope(zinc->Symbol(), zinc->Name(), zinc->Z(), 68, 67.9248442, 0.19024));
    zinc->AddIsotope(new YSIsotope(zinc->Symbol(), zinc->Name(), zinc->Z(), 70, 69.9253193, 0.00631));

    fElements.insert(eCharge, zinc);
    zinc->SetPeriodGroup(4, 12);
    zinc->SetState(YSElement::kSolid);
    zinc->SetFamily(YSElement::kTransitionMetal);

    // Gallium
    eCharge = 31;
    YSElement * gallium      = new YSElement("Ga", "gallium", eCharge);
    gallium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_gallium");

    gallium->AddIsotope(new YSIsotope(gallium->Symbol(), gallium->Name(), gallium->Z(), 69, 68.9255736, 0.60108));
    gallium->AddIsotope(new YSIsotope(gallium->Symbol(), gallium->Name(), gallium->Z(), 71, 70.9247013, 0.39892));

    fElements.insert(eCharge, gallium);
    gallium->SetPeriodGroup(4, 13);
    gallium->SetState(YSElement::kSolid);
    gallium->SetFamily(YSElement::kPostTransitionMetal);

    // Germanium
    eCharge = 32;
    YSElement * germanium      = new YSElement("Ge", "germanium", eCharge);
    germanium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_germanium");

    germanium->AddIsotope(new YSIsotope(germanium->Symbol(), germanium->Name(), germanium->Z(), 70, 69.9242474, 0.2038));
    germanium->AddIsotope(new YSIsotope(germanium->Symbol(), germanium->Name(), germanium->Z(), 72, 71.9220758, 0.2731));
    germanium->AddIsotope(new YSIsotope(germanium->Symbol(), germanium->Name(), germanium->Z(), 73, 72.9234589, 0.0776));
    germanium->AddIsotope(new YSIsotope(germanium->Symbol(), germanium->Name(), germanium->Z(), 74, 73.9211778, 0.3672));

    fElements.insert(eCharge, germanium);
    germanium->SetPeriodGroup(4, 14);
    germanium->SetState(YSElement::kSolid);
    germanium->SetFamily(YSElement::kMetalloid);

    // Arsenic
    eCharge = 33;
    YSElement * arsenic      = new YSElement("As", "arsenic", eCharge);
    arsenic->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_arsenic");

    arsenic->AddIsotope(new YSIsotope(arsenic->Symbol(), arsenic->Name(), arsenic->Z(), 75, 74.9215965, 1.0));

    fElements.insert(eCharge, arsenic);
    arsenic->SetPeriodGroup(4, 15);
    arsenic->SetState(YSElement::kSolid);
    arsenic->SetFamily(YSElement::kMetalloid);

    // Selenium
    eCharge = 34;
    YSElement * selenium      = new YSElement("Se", "selenium", eCharge);
    selenium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_selenium");

    selenium->AddIsotope(new YSIsotope(selenium->Symbol(), selenium->Name(), selenium->Z(), 74, 73.9224764, 0.0089));
    selenium->AddIsotope(new YSIsotope(selenium->Symbol(), selenium->Name(), selenium->Z(), 76, 75.9192136, 0.0937));
    selenium->AddIsotope(new YSIsotope(selenium->Symbol(), selenium->Name(), selenium->Z(), 77, 76.9199140, 0.0763));
    selenium->AddIsotope(new YSIsotope(selenium->Symbol(), selenium->Name(), selenium->Z(), 78, 77.9173091, 0.2377));
    selenium->AddIsotope(new YSIsotope(selenium->Symbol(), selenium->Name(), selenium->Z(), 80, 79.9165213, 0.4961));

    fElements.insert(eCharge, selenium);
    selenium->SetPeriodGroup(4, 16);
    selenium->SetState(YSElement::kSolid);
    selenium->SetFamily(YSElement::kOtherNonMetal);

    // Bromine
    eCharge = 35;
    YSElement * bromine      = new YSElement("Br", "bromine", eCharge);
    bromine->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_bromine");

    bromine->AddIsotope(new YSIsotope(bromine->Symbol(), bromine->Name(), bromine->Z(), 79, 78.9183371, 0.5069));
    bromine->AddIsotope(new YSIsotope(bromine->Symbol(), bromine->Name(), bromine->Z(), 81, 80.9162906, 0.4931));

    fElements.insert(eCharge, bromine);
    bromine->SetPeriodGroup(4, 17);
    bromine->SetState(YSElement::kLiquid);
    bromine->SetFamily(YSElement::kHalogen);

    // Krypton
    eCharge = 36;
    YSElement * krypton      = new YSElement("Kr", "krypton", eCharge);
    krypton->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_krypton");

    krypton->AddIsotope(new YSIsotope(krypton->Symbol(), krypton->Name(), krypton->Z(), 78, 77.9203648, 0.00355));
    krypton->AddIsotope(new YSIsotope(krypton->Symbol(), krypton->Name(), krypton->Z(), 80, 79.9163790, 0.02286));
    krypton->AddIsotope(new YSIsotope(krypton->Symbol(), krypton->Name(), krypton->Z(), 82, 81.9134836, 0.11593));
    krypton->AddIsotope(new YSIsotope(krypton->Symbol(), krypton->Name(), krypton->Z(), 83, 82.914136, 0.11500));
    krypton->AddIsotope(new YSIsotope(krypton->Symbol(), krypton->Name(), krypton->Z(), 84, 83.911507, 0.56987));
    krypton->AddIsotope(new YSIsotope(krypton->Symbol(), krypton->Name(), krypton->Z(), 86, 85.91061073, 0.17279));

    fElements.insert(eCharge, krypton);
    krypton->SetPeriodGroup(4, 18);
    krypton->SetState(YSElement::kGas);
    krypton->SetFamily(YSElement::kNobleGas);

    // Rubidium
    eCharge = 37;
    YSElement * rubidium      = new YSElement("Rb", "rubidium", eCharge);
    rubidium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_rubidium");

    rubidium->AddIsotope(new YSIsotope(rubidium->Symbol(), rubidium->Name(), rubidium->Z(), 85, 84.911789738, 0.7217));
    rubidium->AddIsotope(new YSIsotope(rubidium->Symbol(), rubidium->Name(), rubidium->Z(), 87, 86.909180527, 0.2783));

    fElements.insert(eCharge, rubidium);
    rubidium->SetPeriodGroup(5, 1);
    rubidium->SetState(YSElement::kSolid);
    rubidium->SetFamily(YSElement::KAlkaliMetal);

    // Strontium
    eCharge = 38;
    YSElement * strontium      = new YSElement("Sr", "strontium", eCharge);
    strontium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_strontium");

    strontium->AddIsotope(new YSIsotope(strontium->Symbol(), strontium->Name(), strontium->Z(), 84, 83.913425, 0.0056));
    strontium->AddIsotope(new YSIsotope(strontium->Symbol(), strontium->Name(), strontium->Z(), 86, 85.9092607309, 0.0986));
    strontium->AddIsotope(new YSIsotope(strontium->Symbol(), strontium->Name(), strontium->Z(), 87, 86.9088774970, 0.0700));
    strontium->AddIsotope(new YSIsotope(strontium->Symbol(), strontium->Name(), strontium->Z(), 88, 87.9056122571, 0.8258));

    fElements.insert(eCharge, strontium);
    strontium->SetPeriodGroup(5, 2);
    strontium->SetState(YSElement::kSolid);
    strontium->SetFamily(YSElement::kAlkalineEarthMetal);

    // Yttrium
    eCharge = 39;
    YSElement * yttrium      = new YSElement("Y", "yttrium", eCharge);
    yttrium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_yttrium");

    yttrium->AddIsotope(new YSIsotope(yttrium->Symbol(), yttrium->Name(), yttrium->Z(), 89, 88.9058483, 1.0));

    fElements.insert(eCharge, yttrium);
    yttrium->SetPeriodGroup(5, 3);
    yttrium->SetState(YSElement::kSolid);
    yttrium->SetFamily(YSElement::kTransitionMetal);

    // Zirconium
    eCharge = 40;
    YSElement * zirconium      = new YSElement("Zr", "zirconium", eCharge);
    zirconium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_zirconium");

    zirconium->AddIsotope(new YSIsotope(zirconium->Symbol(), zirconium->Name(), zirconium->Z(), 90, 89.9047044, 0.5145));
    zirconium->AddIsotope(new YSIsotope(zirconium->Symbol(), zirconium->Name(), zirconium->Z(), 91, 90.9056458, 0.1122));
    zirconium->AddIsotope(new YSIsotope(zirconium->Symbol(), zirconium->Name(), zirconium->Z(), 92, 91.9050408, 0.1715));
    zirconium->AddIsotope(new YSIsotope(zirconium->Symbol(), zirconium->Name(), zirconium->Z(), 94, 93.9063152, 0.1738));
    zirconium->AddIsotope(new YSIsotope(zirconium->Symbol(), zirconium->Name(), zirconium->Z(), 96, 95.9082734, 0.0280));

    fElements.insert(eCharge, zirconium);
    zirconium->SetPeriodGroup(5, 4);
    zirconium->SetState(YSElement::kSolid);
    zirconium->SetFamily(YSElement::kTransitionMetal);

    // Niobium
    eCharge = 41;
    YSElement * niobium      = new YSElement("Nb", "niobium", eCharge);
    niobium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_niobium");

    niobium->AddIsotope(new YSIsotope(niobium->Symbol(), niobium->Name(), niobium->Z(), 93, 92.9063781, 1.0));

    fElements.insert(eCharge, niobium);
    niobium->SetPeriodGroup(5, 5);
    niobium->SetState(YSElement::kSolid);
    niobium->SetFamily(YSElement::kTransitionMetal);

    // Molybdenum
    eCharge = 42;
    YSElement * molybdenum      = new YSElement("Mo", "molybdenum", eCharge);
    molybdenum->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_molybdenum");

    molybdenum->AddIsotope(new YSIsotope(molybdenum->Symbol(), molybdenum->Name(), molybdenum->Z(), 92, 91.906811, 0.1477));
    molybdenum->AddIsotope(new YSIsotope(molybdenum->Symbol(), molybdenum->Name(), molybdenum->Z(), 94, 93.9050883, 0.0923));
    molybdenum->AddIsotope(new YSIsotope(molybdenum->Symbol(), molybdenum->Name(), molybdenum->Z(), 95, 94.9058421, 0.1590));
    molybdenum->AddIsotope(new YSIsotope(molybdenum->Symbol(), molybdenum->Name(), molybdenum->Z(), 96, 95.9046795, 0.1668));
    molybdenum->AddIsotope(new YSIsotope(molybdenum->Symbol(), molybdenum->Name(), molybdenum->Z(), 97, 96.9060215, 0.0956));
    molybdenum->AddIsotope(new YSIsotope(molybdenum->Symbol(), molybdenum->Name(), molybdenum->Z(), 98, 97.9054082, 0.2419));

    fElements.insert(eCharge, molybdenum);
    molybdenum->SetPeriodGroup(5, 6);
    molybdenum->SetState(YSElement::kSolid);
    molybdenum->SetFamily(YSElement::kTransitionMetal);

    // Technetium
    eCharge = 43;
    YSElement * technetium      = new YSElement("Tc", "technetium", eCharge);
    technetium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_technetium");

    technetium->AddIsotope(new YSIsotope(technetium->Symbol(), technetium->Name(), technetium->Z(), 99, 98.9062547, 0.0));

    fElements.insert(eCharge, technetium);
    technetium->SetPeriodGroup(5, 7);
    technetium->SetState(YSElement::kSolid);
    technetium->SetFamily(YSElement::kTransitionMetal);

    // Ruthenium
    eCharge = 44;
    YSElement * ruthenium      = new YSElement("Ru", "ruthenium", eCharge);
    ruthenium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_ruthenium");

    ruthenium->AddIsotope(new YSIsotope(ruthenium->Symbol(), ruthenium->Name(), ruthenium->Z(), 96, 95.907598, 0.0554));
    ruthenium->AddIsotope(new YSIsotope(ruthenium->Symbol(), ruthenium->Name(), ruthenium->Z(), 98, 97.905287, 0.0187));
    ruthenium->AddIsotope(new YSIsotope(ruthenium->Symbol(), ruthenium->Name(), ruthenium->Z(), 99, 98.9059393, 0.1276));
    ruthenium->AddIsotope(new YSIsotope(ruthenium->Symbol(), ruthenium->Name(), ruthenium->Z(), 100, 99.9042195, 0.1260));
    ruthenium->AddIsotope(new YSIsotope(ruthenium->Symbol(), ruthenium->Name(), ruthenium->Z(), 101, 100.9055821, 0.1706));
    ruthenium->AddIsotope(new YSIsotope(ruthenium->Symbol(), ruthenium->Name(), ruthenium->Z(), 102, 101.9043493, 0.3155));
    ruthenium->AddIsotope(new YSIsotope(ruthenium->Symbol(), ruthenium->Name(), ruthenium->Z(), 104, 103.905433, 0.1862));

    fElements.insert(eCharge, ruthenium);
    ruthenium->SetPeriodGroup(5, 8);
    ruthenium->SetState(YSElement::kSolid);
    ruthenium->SetFamily(YSElement::kTransitionMetal);

    // Rhodium
    eCharge = 45;
    YSElement * rhodium      = new YSElement("Rh", "rhodium", eCharge);
    rhodium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_rhodium");

    rhodium->AddIsotope(new YSIsotope(rhodium->Symbol(), rhodium->Name(), rhodium->Z(), 103, 102.905504, 1.0));

    fElements.insert(eCharge, rhodium);
    rhodium->SetPeriodGroup(5, 9);
    rhodium->SetState(YSElement::kSolid);
    rhodium->SetFamily(YSElement::kTransitionMetal);

    // Palladium
    eCharge = 46;
    YSElement * palladium      = new YSElement("Pd", "palladium", eCharge);
    palladium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_palladium");

    palladium->AddIsotope(new YSIsotope(palladium->Symbol(), palladium->Name(), palladium->Z(), 102, 101.905609, 0.0102));
    palladium->AddIsotope(new YSIsotope(palladium->Symbol(), palladium->Name(), palladium->Z(), 104, 103.904036, 0.1114));
    palladium->AddIsotope(new YSIsotope(palladium->Symbol(), palladium->Name(), palladium->Z(), 105, 104.905085, 0.2233));
    palladium->AddIsotope(new YSIsotope(palladium->Symbol(), palladium->Name(), palladium->Z(), 106, 105.903486, 0.2733));
    palladium->AddIsotope(new YSIsotope(palladium->Symbol(), palladium->Name(), palladium->Z(), 108, 107.903892, 0.2646));
    palladium->AddIsotope(new YSIsotope(palladium->Symbol(), palladium->Name(), palladium->Z(), 110, 109.905153, 0.1172));

    fElements.insert(eCharge, palladium);
    palladium->SetPeriodGroup(5, 10);
    palladium->SetState(YSElement::kSolid);
    palladium->SetFamily(YSElement::kTransitionMetal);

    // Silver
    eCharge = 47;
    YSElement * silver      = new YSElement("Ag", "silver", eCharge);
    silver->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_silver");

    silver->AddIsotope(new YSIsotope(silver->Symbol(), silver->Name(), silver->Z(), 107, 106.905097, 0.51839));
    silver->AddIsotope(new YSIsotope(silver->Symbol(), silver->Name(), silver->Z(), 109, 108.904752, 0.48161));

    fElements.insert(eCharge, silver);
    silver->SetPeriodGroup(5, 11);
    silver->SetState(YSElement::kSolid);
    silver->SetFamily(YSElement::kTransitionMetal);

    // Cadmium
    eCharge = 48;
    YSElement * cadmium      = new YSElement("Cd", "cadmium", eCharge);
    cadmium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_cadmium");

    cadmium->AddIsotope(new YSIsotope(cadmium->Symbol(), cadmium->Name(), cadmium->Z(), 106, 105.906459, 0.0125));
    cadmium->AddIsotope(new YSIsotope(cadmium->Symbol(), cadmium->Name(), cadmium->Z(), 108, 107.904184, 0.0089));
    cadmium->AddIsotope(new YSIsotope(cadmium->Symbol(), cadmium->Name(), cadmium->Z(), 110, 109.9030021, 0.1249));
    cadmium->AddIsotope(new YSIsotope(cadmium->Symbol(), cadmium->Name(), cadmium->Z(), 111, 110.9041781, 0.1280));
    cadmium->AddIsotope(new YSIsotope(cadmium->Symbol(), cadmium->Name(), cadmium->Z(), 112, 111.9027578, 0.2413));
    cadmium->AddIsotope(new YSIsotope(cadmium->Symbol(), cadmium->Name(), cadmium->Z(), 112, 113.9033585, 0.2873));

    fElements.insert(eCharge, cadmium);
    cadmium->SetPeriodGroup(5, 12);
    cadmium->SetState(YSElement::kSolid);
    cadmium->SetFamily(YSElement::kTransitionMetal);

    // Indium
    eCharge = 49;
    YSElement * indium      = new YSElement("In", "indium", eCharge);
    indium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_indium");

    indium->AddIsotope(new YSIsotope(indium->Symbol(), indium->Name(), indium->Z(), 113, 112.904058, 0.0429));
    indium->AddIsotope(new YSIsotope(indium->Symbol(), indium->Name(), indium->Z(), 115, 114.903878, 0.9571));

    fElements.insert(eCharge, indium);
    indium->SetPeriodGroup(5, 13);
    indium->SetState(YSElement::kSolid);
    indium->SetFamily(YSElement::kPostTransitionMetal);

    // Tin
    eCharge = 50;
    YSElement * tin      = new YSElement("Sn", "tin", eCharge);
    tin->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_tin");

    tin->AddIsotope(new YSIsotope(tin->Symbol(), tin->Name(), tin->Z(), 112, 111.904818, 0.0097));
    tin->AddIsotope(new YSIsotope(tin->Symbol(), tin->Name(), tin->Z(), 114, 113.902779, 0.0066));
    tin->AddIsotope(new YSIsotope(tin->Symbol(), tin->Name(), tin->Z(), 115, 114.903342, 0.0034));
    tin->AddIsotope(new YSIsotope(tin->Symbol(), tin->Name(), tin->Z(), 116, 115.901741, 0.1454));
    tin->AddIsotope(new YSIsotope(tin->Symbol(), tin->Name(), tin->Z(), 117, 116.902952, 0.0768));
    tin->AddIsotope(new YSIsotope(tin->Symbol(), tin->Name(), tin->Z(), 118, 117.901603, 0.2422));
    tin->AddIsotope(new YSIsotope(tin->Symbol(), tin->Name(), tin->Z(), 119, 118.903308, 0.0859));
    tin->AddIsotope(new YSIsotope(tin->Symbol(), tin->Name(), tin->Z(), 120, 119.9021947, 0.3258));

    fElements.insert(eCharge, tin);
    tin->SetPeriodGroup(5, 14);
    tin->SetState(YSElement::kSolid);
    tin->SetFamily(YSElement::kPostTransitionMetal);

    // Antomony
    eCharge = 51;
    YSElement * antimony      = new YSElement("Sb", "antimony", eCharge);
    antimony->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_antimony");

    antimony->AddIsotope(new YSIsotope(antimony->Symbol(), antimony->Name(), antimony->Z(), 121, 120.9038157, 0.5721));
    antimony->AddIsotope(new YSIsotope(antimony->Symbol(), antimony->Name(), antimony->Z(), 123, 122.9042140, 0.4279));

    fElements.insert(eCharge, antimony);
    antimony->SetPeriodGroup(5, 15);
    antimony->SetState(YSElement::kSolid);
    antimony->SetFamily(YSElement::kMetalloid);

    // Tellurium
    eCharge = 52;
    YSElement * tellurium      = new YSElement("Te", "tellurium", eCharge);
    tellurium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_tellurium");

    tellurium->AddIsotope(new YSIsotope(tellurium->Symbol(), tellurium->Name(), tellurium->Z(), 120, 119.90402, 9E-4));
    tellurium->AddIsotope(new YSIsotope(tellurium->Symbol(), tellurium->Name(), tellurium->Z(), 122, 121.9030439, 0.0255));
    tellurium->AddIsotope(new YSIsotope(tellurium->Symbol(), tellurium->Name(), tellurium->Z(), 123, 122.9042700, 0.0089));
    tellurium->AddIsotope(new YSIsotope(tellurium->Symbol(), tellurium->Name(), tellurium->Z(), 124, 123.9028179, 0.0474));
    tellurium->AddIsotope(new YSIsotope(tellurium->Symbol(), tellurium->Name(), tellurium->Z(), 126, 125.9033117, 0.1884));

    fElements.insert(eCharge, tellurium);
    tellurium->SetPeriodGroup(5, 16);
    tellurium->SetState(YSElement::kSolid);
    tellurium->SetFamily(YSElement::kMetalloid);

    // Iodine
    eCharge = 53;
    YSElement * iodine      = new YSElement("I", "iodine", eCharge);
    iodine->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_iodine");

    iodine->AddIsotope(new YSIsotope(iodine->Symbol(), iodine->Name(), iodine->Z(), 127, 126.904473, 1.0));

    fElements.insert(eCharge, iodine);
    iodine->SetPeriodGroup(5, 17);
    iodine->SetState(YSElement::kSolid);
    iodine->SetFamily(YSElement::kHalogen);

    // Xenon
    eCharge = 54;
    YSElement * xenon      = new YSElement("Xe", "xenon", eCharge);
    xenon->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_xenon");

    xenon->AddIsotope(new YSIsotope(xenon->Symbol(), xenon->Name(), xenon->Z(), 124, 123.905893, 9.52E-4));
    xenon->AddIsotope(new YSIsotope(xenon->Symbol(), xenon->Name(), xenon->Z(), 126, 125.904274, 8.902E-4));
    xenon->AddIsotope(new YSIsotope(xenon->Symbol(), xenon->Name(), xenon->Z(), 128, 127.9035313, 0.019102));
    xenon->AddIsotope(new YSIsotope(xenon->Symbol(), xenon->Name(), xenon->Z(), 129, 128.9047794, 0.264006));
    xenon->AddIsotope(new YSIsotope(xenon->Symbol(), xenon->Name(), xenon->Z(), 130, 129.9035080, 0.040710));
    xenon->AddIsotope(new YSIsotope(xenon->Symbol(), xenon->Name(), xenon->Z(), 131, 130.9050824, 0.212324));
    xenon->AddIsotope(new YSIsotope(xenon->Symbol(), xenon->Name(), xenon->Z(), 132, 131.9041535, 0.269086));
    xenon->AddIsotope(new YSIsotope(xenon->Symbol(), xenon->Name(), xenon->Z(), 134, 133.9053945, 0.104357));
    xenon->AddIsotope(new YSIsotope(xenon->Symbol(), xenon->Name(), xenon->Z(), 136, 135.907219, 0.088573));

    fElements.insert(eCharge, xenon);
    xenon->SetPeriodGroup(5, 18);
    xenon->SetState(YSElement::kGas);
    xenon->SetFamily(YSElement::kNobleGas);

    // Caesium
    eCharge = 55;
    YSElement * caesium      = new YSElement("Cs", "caesium", eCharge);
    caesium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_caesium");

    caesium->AddIsotope(new YSIsotope(caesium->Symbol(), caesium->Name(), caesium->Z(), 133, 132.905451933, 1.0));

    fElements.insert(eCharge, caesium);
    caesium->SetPeriodGroup(6, 1);
    caesium->SetState(YSElement::kSolid);
    caesium->SetFamily(YSElement::KAlkaliMetal);

    // Barium
    eCharge = 56;
    YSElement * barium      = new YSElement("Ba", "barium", eCharge);
    barium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_barium");

    barium->AddIsotope(new YSIsotope(barium->Symbol(), barium->Name(), barium->Z(), 132, 131.9050613, 0.00101));
    barium->AddIsotope(new YSIsotope(barium->Symbol(), barium->Name(), barium->Z(), 134, 133.9045084, 0.02417));
    barium->AddIsotope(new YSIsotope(barium->Symbol(), barium->Name(), barium->Z(), 135, 134.9056886, 0.06592));
    barium->AddIsotope(new YSIsotope(barium->Symbol(), barium->Name(), barium->Z(), 136, 135.9045759, 0.07854));
    barium->AddIsotope(new YSIsotope(barium->Symbol(), barium->Name(), barium->Z(), 137, 136.9058274, 0.11232));
    barium->AddIsotope(new YSIsotope(barium->Symbol(), barium->Name(), barium->Z(), 138, 137.9052472, 0.71698));

    fElements.insert(eCharge, barium);
    barium->SetPeriodGroup(6, 2);
    barium->SetState(YSElement::kSolid);
    barium->SetFamily(YSElement::kAlkalineEarthMetal);

    // Lanthanum
    eCharge = 57;
    YSElement * lanthanum      = new YSElement("La", "lanthanum", eCharge);
    lanthanum->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_lanthanum");

    lanthanum->AddIsotope(new YSIsotope(lanthanum->Symbol(), lanthanum->Name(), lanthanum->Z(), 138, 137.907112, 9E-4));
    lanthanum->AddIsotope(new YSIsotope(lanthanum->Symbol(), lanthanum->Name(), lanthanum->Z(), 139, 138.9063533, 0.99910));

    fElements.insert(eCharge, lanthanum);
    lanthanum->SetPeriodGroup(9, 4);
    lanthanum->SetState(YSElement::kSolid);
    lanthanum->SetFamily(YSElement::kLanthanoid);

    // Cerium
    eCharge = 58;
    YSElement * cerium      = new YSElement("Ce", "cerium", eCharge);
    cerium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_cerium");

    cerium->AddIsotope(new YSIsotope(cerium->Symbol(), cerium->Name(), cerium->Z(), 136, 135.907172, 0.00185));
    cerium->AddIsotope(new YSIsotope(cerium->Symbol(), cerium->Name(), cerium->Z(), 138, 137.905991, 0.00251));
    cerium->AddIsotope(new YSIsotope(cerium->Symbol(), cerium->Name(), cerium->Z(), 140, 139.9054387, 0.88450));
    cerium->AddIsotope(new YSIsotope(cerium->Symbol(), cerium->Name(), cerium->Z(), 142, 141.909244, 0.11114));

    fElements.insert(eCharge, cerium);
    cerium->SetPeriodGroup(9, 5);
    cerium->SetState(YSElement::kSolid);
    cerium->SetFamily(YSElement::kLanthanoid);

    // Praseodymium
    eCharge = 59;
    YSElement * praseodymium      = new YSElement("Pr", "praseodymium", eCharge);
    praseodymium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_praseodymium");

    praseodymium->AddIsotope(new YSIsotope(praseodymium->Symbol(), praseodymium->Name(), praseodymium->Z(), 140, 140.9076528, 1.0));

    fElements.insert(eCharge, praseodymium);
    praseodymium->SetPeriodGroup(9, 6);
    praseodymium->SetState(YSElement::kSolid);
    praseodymium->SetFamily(YSElement::kLanthanoid);


    // Neodymium
    eCharge = 60;
    YSElement * neodymium      = new YSElement("Nd", "neodymium", eCharge);
    neodymium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_neodymium");

    neodymium->AddIsotope(new YSIsotope(neodymium->Symbol(), neodymium->Name(), neodymium->Z(), 142, 141.9077233, 0.272));
    neodymium->AddIsotope(new YSIsotope(neodymium->Symbol(), neodymium->Name(), neodymium->Z(), 143, 142.9098143, 0.122));
    neodymium->AddIsotope(new YSIsotope(neodymium->Symbol(), neodymium->Name(), neodymium->Z(), 145, 144.9125736, 0.083));
    neodymium->AddIsotope(new YSIsotope(neodymium->Symbol(), neodymium->Name(), neodymium->Z(), 146, 145.9131169, 0.172));
    neodymium->AddIsotope(new YSIsotope(neodymium->Symbol(), neodymium->Name(), neodymium->Z(), 148, 147.916893, 0.057));

    fElements.insert(eCharge, neodymium);
    neodymium->SetPeriodGroup(9, 7);
    neodymium->SetState(YSElement::kSolid);
    neodymium->SetFamily(YSElement::kLanthanoid);

    // Promethium
    eCharge = 61;
    YSElement * promethium      = new YSElement("Pm", "promethium", eCharge);
    promethium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_promethium");

    promethium->AddIsotope(new YSIsotope(promethium->Symbol(), "no stable isotope", promethium->Z(), 0, 0, 0.0));

    fElements.insert(eCharge, promethium);
    promethium->SetPeriodGroup(9, 8);
    promethium->SetState(YSElement::kSolid);
    promethium->SetFamily(YSElement::kLanthanoid);

    // Samarium
    eCharge = 62;
    YSElement * samarium      = new YSElement("Sm", "samarium", eCharge);
    samarium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_samarium");

    samarium->AddIsotope(new YSIsotope(samarium->Symbol(), samarium->Name(), samarium->Z(), 144, 143.911999, 0.0307));
    samarium->AddIsotope(new YSIsotope(samarium->Symbol(), samarium->Name(), samarium->Z(), 149, 148.9171847, 0.1382));
    samarium->AddIsotope(new YSIsotope(samarium->Symbol(), samarium->Name(), samarium->Z(), 150, 149.9172755, 0.0738));
    samarium->AddIsotope(new YSIsotope(samarium->Symbol(), samarium->Name(), samarium->Z(), 152, 151.9197324, 0.2675));
    samarium->AddIsotope(new YSIsotope(samarium->Symbol(), samarium->Name(), samarium->Z(), 154, 153.9222093, 0.2275));

    fElements.insert(eCharge, samarium);
    samarium->SetPeriodGroup(9, 9);
    samarium->SetState(YSElement::kSolid);
    samarium->SetFamily(YSElement::kLanthanoid);

    // Europium
    eCharge = 63;
    YSElement * europium      = new YSElement("Eu", "europium", eCharge);
    europium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_europium");

    europium->AddIsotope(new YSIsotope(europium->Symbol(), europium->Name(), europium->Z(), 151, 150.9198502, 0.4781));
    europium->AddIsotope(new YSIsotope(europium->Symbol(), europium->Name(), europium->Z(), 153, 152.9212303, 0.5219));

    fElements.insert(eCharge, europium);
    europium->SetPeriodGroup(9, 10);
    europium->SetState(YSElement::kSolid);
    europium->SetFamily(YSElement::kLanthanoid);

    // Gadolinium
    eCharge = 64;
    YSElement * gadolinium      = new YSElement("Gd", "gadolinium", eCharge);
    gadolinium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_gadolinium");

    gadolinium->AddIsotope(new YSIsotope(gadolinium->Symbol(), gadolinium->Name(), gadolinium->Z(), 152, 151.9197910, 0.0020));
    gadolinium->AddIsotope(new YSIsotope(gadolinium->Symbol(), gadolinium->Name(), gadolinium->Z(), 154, 153.9208656, 0.0218));
    gadolinium->AddIsotope(new YSIsotope(gadolinium->Symbol(), gadolinium->Name(), gadolinium->Z(), 155, 154.9226220, 0.1480));
    gadolinium->AddIsotope(new YSIsotope(gadolinium->Symbol(), gadolinium->Name(), gadolinium->Z(), 156, 155.9221227, 0.2047));
    gadolinium->AddIsotope(new YSIsotope(gadolinium->Symbol(), gadolinium->Name(), gadolinium->Z(), 157, 156.9239601, 0.2484));
    gadolinium->AddIsotope(new YSIsotope(gadolinium->Symbol(), gadolinium->Name(), gadolinium->Z(), 158, 157.9241039, 0.1565));
    gadolinium->AddIsotope(new YSIsotope(gadolinium->Symbol(), gadolinium->Name(), gadolinium->Z(), 160, 159.9270541, 0.2186));

    fElements.insert(eCharge, gadolinium);
    gadolinium->SetPeriodGroup(9, 11);
    gadolinium->SetState(YSElement::kSolid);
    gadolinium->SetFamily(YSElement::kLanthanoid);

    // Terbium
    eCharge = 65;
    YSElement * terbium      = new YSElement("Tb", "terbium", eCharge);
    terbium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_terbium");

    terbium->AddIsotope(new YSIsotope(terbium->Symbol(), terbium->Name(), terbium->Z(), 159, 158.9253468, 1.0));

    fElements.insert(eCharge, terbium);
    terbium->SetPeriodGroup(9, 12);
    terbium->SetState(YSElement::kSolid);
    terbium->SetFamily(YSElement::kLanthanoid);

    // Dysprosium
    eCharge = 66;
    YSElement * dysprosium      = new YSElement("Dy", "dysprosium", eCharge);
    dysprosium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_dysprosium");

    dysprosium->AddIsotope(new YSIsotope(dysprosium->Symbol(), dysprosium->Name(), dysprosium->Z(), 156, 155.924283, 5.6E-4));
    dysprosium->AddIsotope(new YSIsotope(dysprosium->Symbol(), dysprosium->Name(), dysprosium->Z(), 158, 157.924409, 9.5E-4));
    dysprosium->AddIsotope(new YSIsotope(dysprosium->Symbol(), dysprosium->Name(), dysprosium->Z(), 160, 159.9251975, 0.02329));
    dysprosium->AddIsotope(new YSIsotope(dysprosium->Symbol(), dysprosium->Name(), dysprosium->Z(), 161, 160.9269334, 0.18889));
    dysprosium->AddIsotope(new YSIsotope(dysprosium->Symbol(), dysprosium->Name(), dysprosium->Z(), 162, 161.9267984, 0.25475));
    dysprosium->AddIsotope(new YSIsotope(dysprosium->Symbol(), dysprosium->Name(), dysprosium->Z(), 163, 162.9287312, 0.24896));
    dysprosium->AddIsotope(new YSIsotope(dysprosium->Symbol(), dysprosium->Name(), dysprosium->Z(), 164, 163.9291748, 0.28260));

    fElements.insert(eCharge, dysprosium);
    dysprosium->SetPeriodGroup(9, 13);
    dysprosium->SetState(YSElement::kSolid);
    dysprosium->SetFamily(YSElement::kLanthanoid);

    // Holmium
    eCharge = 67;
    YSElement * holmium      = new YSElement("Ho", "holmium", eCharge);
    holmium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_holmium");

    holmium->AddIsotope(new YSIsotope(holmium->Symbol(), holmium->Name(), holmium->Z(), 165, 164.9303221, 1.0));

    fElements.insert(eCharge, holmium);
    holmium->SetPeriodGroup(9, 14);
    holmium->SetState(YSElement::kSolid);
    holmium->SetFamily(YSElement::kLanthanoid);

    // Erbium
    eCharge = 68;
    YSElement * erbium      = new YSElement("Er", "erbium", eCharge);
    erbium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_erbium");

    erbium->AddIsotope(new YSIsotope(erbium->Symbol(), erbium->Name(), erbium->Z(), 162, 161.928778, 0.00139));
    erbium->AddIsotope(new YSIsotope(erbium->Symbol(), erbium->Name(), erbium->Z(), 164, 163.929200, 0.01601));
    erbium->AddIsotope(new YSIsotope(erbium->Symbol(), erbium->Name(), erbium->Z(), 166, 165.9302931, 0.33503));
    erbium->AddIsotope(new YSIsotope(erbium->Symbol(), erbium->Name(), erbium->Z(), 167, 166.9320482, 0.22869));
    erbium->AddIsotope(new YSIsotope(erbium->Symbol(), erbium->Name(), erbium->Z(), 168, 167.9323702, 0.26978));
    erbium->AddIsotope(new YSIsotope(erbium->Symbol(), erbium->Name(), erbium->Z(), 170, 169.9354643, 0.14910));

    fElements.insert(eCharge, erbium);
    erbium->SetPeriodGroup(9, 15);
    erbium->SetState(YSElement::kSolid);
    erbium->SetFamily(YSElement::kLanthanoid);

    // Thulium
    eCharge = 69;
    YSElement * thulium      = new YSElement("Tm", "thulium", eCharge);
    thulium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_thulium");

    thulium->AddIsotope(new YSIsotope(thulium->Symbol(), thulium->Name(), thulium->Z(), 169, 168.9342133, 1.0));

    fElements.insert(eCharge, thulium);
    thulium->SetPeriodGroup(9, 16);
    thulium->SetState(YSElement::kSolid);
    thulium->SetFamily(YSElement::kLanthanoid);

    // Ytterbium
    eCharge = 70;
    YSElement * ytterbium      = new YSElement("Yb", "ytterbium", eCharge);
    ytterbium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_ytterbium");

    ytterbium->AddIsotope(new YSIsotope(ytterbium->Symbol(), ytterbium->Name(), ytterbium->Z(), 168, 167.933897, 0.0013));
    ytterbium->AddIsotope(new YSIsotope(ytterbium->Symbol(), ytterbium->Name(), ytterbium->Z(), 170, 169.9347618, 0.0304));
    ytterbium->AddIsotope(new YSIsotope(ytterbium->Symbol(), ytterbium->Name(), ytterbium->Z(), 171, 170.9363258, 0.1428));
    ytterbium->AddIsotope(new YSIsotope(ytterbium->Symbol(), ytterbium->Name(), ytterbium->Z(), 172, 171.9363815, 0.2183));
    ytterbium->AddIsotope(new YSIsotope(ytterbium->Symbol(), ytterbium->Name(), ytterbium->Z(), 173, 171.9363815, 0.1613));
    ytterbium->AddIsotope(new YSIsotope(ytterbium->Symbol(), ytterbium->Name(), ytterbium->Z(), 174, 173.9388621, 0.3183));
    ytterbium->AddIsotope(new YSIsotope(ytterbium->Symbol(), ytterbium->Name(), ytterbium->Z(), 176, 175.9425717, 0.1276));

    fElements.insert(eCharge, ytterbium);
    ytterbium->SetPeriodGroup(9, 17);
    ytterbium->SetState(YSElement::kSolid);
    ytterbium->SetFamily(YSElement::kLanthanoid);

    // Lutetium
    eCharge = 71;
    YSElement * lutetium      = new YSElement("Lu", "lutetium", eCharge);
    lutetium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_lutetium");

    lutetium->AddIsotope(new YSIsotope(lutetium->Symbol(), lutetium->Name(), lutetium->Z(), 175, 174.9407718, 0.9741));
    lutetium->AddIsotope(new YSIsotope(lutetium->Symbol(), lutetium->Name(), lutetium->Z(), 176, 175.9426863, 0.0259));

    fElements.insert(eCharge, lutetium);
    lutetium->SetPeriodGroup(9, 18);
    lutetium->SetState(YSElement::kSolid);
    lutetium->SetFamily(YSElement::kLanthanoid);

    // Hafnium
    eCharge = 72;
    YSElement * hafnium      = new YSElement("Hf", "hafnium", eCharge);
    hafnium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_hafnium");

    hafnium->AddIsotope(new YSIsotope(hafnium->Symbol(), hafnium->Name(), hafnium->Z(), 176, 175.9414086, 0.0526));
    hafnium->AddIsotope(new YSIsotope(hafnium->Symbol(), hafnium->Name(), hafnium->Z(), 177, 176.9432207, 0.1860));
    hafnium->AddIsotope(new YSIsotope(hafnium->Symbol(), hafnium->Name(), hafnium->Z(), 178, 177.9436988, 0.2728));
    hafnium->AddIsotope(new YSIsotope(hafnium->Symbol(), hafnium->Name(), hafnium->Z(), 179, 178.9458161, 0.1362));
    hafnium->AddIsotope(new YSIsotope(hafnium->Symbol(), hafnium->Name(), hafnium->Z(), 180, 179.9465500, 0.3508));

    fElements.insert(eCharge, hafnium);
    hafnium->SetPeriodGroup(6, 4);
    hafnium->SetState(YSElement::kSolid);
    hafnium->SetFamily(YSElement::kTransitionMetal);


    // Tantalum
    eCharge = 73;
    YSElement * tantalum      = new YSElement("Ta", "tantalum", eCharge);
    tantalum->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_tantalum");

    tantalum->AddIsotope(new YSIsotope(tantalum->Symbol(), tantalum->Name(), tantalum->Z(), 181, 180.9479958, 0.99988));

    fElements.insert(eCharge, tantalum);
    tantalum->SetPeriodGroup(6, 5);
    tantalum->SetState(YSElement::kSolid);
    tantalum->SetFamily(YSElement::kTransitionMetal);

    // Tungsten
    eCharge = 74;
    YSElement * tungsten      = new YSElement("W", "tungsten", eCharge);
    tungsten->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_tungsten");

    tungsten->AddIsotope(new YSIsotope(tungsten->Symbol(), tungsten->Name(), tungsten->Z(), 182, 181.9482042, 0.2650));
    tungsten->AddIsotope(new YSIsotope(tungsten->Symbol(), tungsten->Name(), tungsten->Z(), 183, 182.9502230, 0.1431));
    tungsten->AddIsotope(new YSIsotope(tungsten->Symbol(), tungsten->Name(), tungsten->Z(), 184, 183.9509312, 0.3064));
    tungsten->AddIsotope(new YSIsotope(tungsten->Symbol(), tungsten->Name(), tungsten->Z(), 186, 185.9543641, 0.2843));

    fElements.insert(eCharge, tungsten);
    tungsten->SetPeriodGroup(6, 6);
    tungsten->SetState(YSElement::kSolid);
    tungsten->SetFamily(YSElement::kTransitionMetal);

    // Rhenium
    eCharge = 75;
    YSElement * rhenium      = new YSElement("Re", "rhenium", eCharge);
    rhenium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_rhenium");

    rhenium->AddIsotope(new YSIsotope(rhenium->Symbol(), rhenium->Name(), rhenium->Z(), 185, 184.9529550, 0.3740));
    rhenium->AddIsotope(new YSIsotope(rhenium->Symbol(), rhenium->Name(), rhenium->Z(), 187, 186.9557531, 0.6260));

    fElements.insert(eCharge, rhenium);
    rhenium->SetPeriodGroup(6, 7);
    rhenium->SetState(YSElement::kSolid);
    rhenium->SetFamily(YSElement::kTransitionMetal);

    // Osmium
    eCharge = 76;
    YSElement * osmium      = new YSElement("Os", "osmium", eCharge);
    osmium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_osmium");

    osmium->AddIsotope(new YSIsotope(osmium->Symbol(), osmium->Name(), osmium->Z(), 184, 183.9524891, 2E-4));
    osmium->AddIsotope(new YSIsotope(osmium->Symbol(), osmium->Name(), osmium->Z(), 187, 186.9557505, 0.0196));
    osmium->AddIsotope(new YSIsotope(osmium->Symbol(), osmium->Name(), osmium->Z(), 188, 187.9558382, 0.1324));
    osmium->AddIsotope(new YSIsotope(osmium->Symbol(), osmium->Name(), osmium->Z(), 189, 188.9581475, 0.1615));
    osmium->AddIsotope(new YSIsotope(osmium->Symbol(), osmium->Name(), osmium->Z(), 190, 189.9584470, 0.2626));
    osmium->AddIsotope(new YSIsotope(osmium->Symbol(), osmium->Name(), osmium->Z(), 192, 191.9614807, 0.4078));

    fElements.insert(eCharge, osmium);
    osmium->SetPeriodGroup(6, 8);
    osmium->SetState(YSElement::kSolid);
    osmium->SetFamily(YSElement::kTransitionMetal);

    // Iridium
    eCharge = 77;
    YSElement * iridium      = new YSElement("Ir", "iridium", eCharge);
    iridium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_iridium");

    iridium->AddIsotope(new YSIsotope(iridium->Symbol(), iridium->Name(), iridium->Z(), 191, 190.9605940, 0.373));
    iridium->AddIsotope(new YSIsotope(iridium->Symbol(), iridium->Name(), iridium->Z(), 193, 192.9629264, 0.627));

    fElements.insert(eCharge, iridium);
    iridium->SetPeriodGroup(6, 9);
    iridium->SetState(YSElement::kSolid);
    iridium->SetFamily(YSElement::kTransitionMetal);

    // Platinum
    eCharge = 78;
    YSElement * platinum      = new YSElement("Pt", "platinum", eCharge);
    platinum->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_platinum");

    platinum->AddIsotope(new YSIsotope(platinum->Symbol(), platinum->Name(), platinum->Z(), 192, 191.9610380, 0.00782));
    platinum->AddIsotope(new YSIsotope(platinum->Symbol(), platinum->Name(), platinum->Z(), 194, 193.9626803, 0.32967));
    platinum->AddIsotope(new YSIsotope(platinum->Symbol(), platinum->Name(), platinum->Z(), 196, 195.9649515, 0.25242));
    platinum->AddIsotope(new YSIsotope(platinum->Symbol(), platinum->Name(), platinum->Z(), 198, 197.967893, 0.07163));

    fElements.insert(eCharge, platinum);
    platinum->SetPeriodGroup(6, 10);
    platinum->SetState(YSElement::kSolid);
    platinum->SetFamily(YSElement::kTransitionMetal);

    // Gold
    eCharge = 79;
    YSElement * gold      = new YSElement("Au", "gold", eCharge);
    gold->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_gold");

    gold->AddIsotope(new YSIsotope(gold->Symbol(), gold->Name(), gold->Z(), 196, 196.9665687, 1.0));

    fElements.insert(eCharge, gold);
    gold->SetPeriodGroup(6, 11);
    gold->SetState(YSElement::kSolid);
    gold->SetFamily(YSElement::kTransitionMetal);

    // Mercury
    eCharge = 80;
    YSElement * mercury      = new YSElement("Hg", "mercury", eCharge);
    mercury->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_mercury");

    mercury->AddIsotope(new YSIsotope(mercury->Symbol(), mercury->Name(), mercury->Z(), 196, 195.965833, 0.0015));
    mercury->AddIsotope(new YSIsotope(mercury->Symbol(), mercury->Name(), mercury->Z(), 198, 197.9667690, 0.0997));
    mercury->AddIsotope(new YSIsotope(mercury->Symbol(), mercury->Name(), mercury->Z(), 199, 198.9682799, 0.1687));
    mercury->AddIsotope(new YSIsotope(mercury->Symbol(), mercury->Name(), mercury->Z(), 200, 199.9683260, 0.2310));
    mercury->AddIsotope(new YSIsotope(mercury->Symbol(), mercury->Name(), mercury->Z(), 202, 201.9706430, 0.2986));
    mercury->AddIsotope(new YSIsotope(mercury->Symbol(), mercury->Name(), mercury->Z(), 204, 203.9734939, 0.0687));

    fElements.insert(eCharge, mercury);
    mercury->SetPeriodGroup(6, 12);
    mercury->SetState(YSElement::kLiquid);
    mercury->SetFamily(YSElement::kTransitionMetal);

    // Thallium
    eCharge = 81;
    YSElement * thallium      = new YSElement("Tl", "thallium", eCharge);
    thallium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_thallium");

    thallium->AddIsotope(new YSIsotope(thallium->Symbol(), thallium->Name(), thallium->Z(), 203, 202.9723442, 0.2952));
    thallium->AddIsotope(new YSIsotope(thallium->Symbol(), thallium->Name(), thallium->Z(), 205, 204.9744275, 0.7048));

    fElements.insert(eCharge, thallium);
    thallium->SetPeriodGroup(6, 13);
    thallium->SetState(YSElement::kSolid);
    thallium->SetFamily(YSElement::kPostTransitionMetal);

    // Lead
    eCharge = 82;
    YSElement     * lead  = new YSElement("Pb", "lead", eCharge);
    lead->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_lead");

    lead->AddIsotope(new YSIsotope(lead->Symbol(), lead->Name(), lead->Z(), 204, 203.9730436, 0.014));
    lead->AddIsotope(new YSIsotope(lead->Symbol(), lead->Name(), lead->Z(), 206, 205.9744653, 0.24));
    lead->AddIsotope(new YSIsotope(lead->Symbol(), lead->Name(), lead->Z(), 207, 206.9758969, 0.221));
    lead->AddIsotope(new YSIsotope(lead->Symbol(), lead->Name(), lead->Z(), 208, 207.9766521, 0.524));

    fElements.insert(eCharge, lead);
    lead->SetPeriodGroup(6, 14);
    lead->SetState(YSElement::kSolid);
    lead->SetFamily(YSElement::kPostTransitionMetal);

    // Bismuth
    eCharge = 83;
    YSElement     * bismuth  = new YSElement("Bi", "bismuth", eCharge);
    bismuth->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_bismuth");

    bismuth->AddIsotope(new YSIsotope(bismuth->Symbol(), bismuth->Name(), bismuth->Z(), 209, 208.9803987, 1));

    fElements.insert(eCharge, bismuth);
    bismuth->SetPeriodGroup(6, 15);
    bismuth->SetState(YSElement::kSolid);
    bismuth->SetFamily(YSElement::kPostTransitionMetal);

    // Polonium
    eCharge = 84;
    YSElement     * polonium  = new YSElement("Po", "polonium", eCharge);
    polonium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_polonium");

    polonium->AddIsotope(new YSIsotope(polonium->Symbol(), polonium->Name(), polonium->Z(), 210, 209.9828737, 0));

    fElements.insert(eCharge, polonium);
    polonium->SetPeriodGroup(6, 16);
    polonium->SetState(YSElement::kSolid);
    polonium->SetFamily(YSElement::kMetalloid);

    // Astatine
    eCharge = 85;
    YSElement     * astatine  = new YSElement("At", "astatine", eCharge);
    astatine->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_astatine");

    astatine->AddIsotope(new YSIsotope(astatine->Symbol(), astatine->Name(), astatine->Z(), 210, 209.987148, 0));

    fElements.insert(eCharge, astatine);
    astatine->SetPeriodGroup(6, 17);
    astatine->SetState(YSElement::kSolid);
    astatine->SetFamily(YSElement::kHalogen);

    // Radon
    eCharge = 86;
    YSElement     * radon  = new YSElement("Rn", "radon", eCharge);
    radon->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_radon");

    radon->AddIsotope(new YSIsotope(radon->Symbol(), radon->Name(), radon->Z(), 222, 222.0175777, 0));

    fElements.insert(eCharge, radon);
    radon->SetPeriodGroup(6, 18);
    radon->SetState(YSElement::kGas);
    radon->SetState(YSElement::kGas);
    radon->SetFamily(YSElement::kNobleGas);

    // Francium
    eCharge = 87;
    YSElement     * francium  = new YSElement("Fr", "francium", eCharge);
    francium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_francium");

    francium->AddIsotope(new YSIsotope(francium->Symbol(), francium->Name(), francium->Z(), 223, 223.0197359, 0));

    fElements.insert(eCharge, francium);
    francium->SetPeriodGroup(7, 1);
    francium->SetState(YSElement::kSolid);
    francium->SetFamily(YSElement::KAlkaliMetal);

    // Radium
    eCharge = 88;
    YSElement     * radium = new YSElement("Ra", "radium", eCharge);
    radium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_radium");

    radium->AddIsotope(new YSIsotope(radium->Symbol(), radium->Name(), radium->Z(), 226, 226.0254098, 0));

    fElements.insert(eCharge, radium);
    radium->SetPeriodGroup(7, 2);
    radium->SetState(YSElement::kSolid);
    radium->SetFamily(YSElement::kAlkalineEarthMetal);

    // Actinium
    eCharge = 89;
    YSElement     * actinium = new YSElement("Ac", "actinium", eCharge);
    actinium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_actinium");

    actinium->AddIsotope(new YSIsotope(actinium->Symbol(), actinium->Name(), actinium->Z(), 227, 227.0277521, 0));

    fElements.insert(eCharge, actinium);
    actinium->SetPeriodGroup(10, 4);
    actinium->SetState(YSElement::kSolid);
    actinium->SetFamily(YSElement::kActinoid);

    // Thorium
    eCharge = 90;
    YSElement     * thorium = new YSElement("Th", "thorium", eCharge);
    thorium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_thorium");

    thorium->AddIsotope(new YSIsotope(thorium->Symbol(), thorium->Name(), thorium->Z(), 232, 232.0380553, 1));

    fElements.insert(eCharge, thorium);
    thorium->SetPeriodGroup(10, 5);
    thorium->SetState(YSElement::kSolid);
    thorium->SetFamily(YSElement::kActinoid);

    // Protactinium
    eCharge = 91;
    YSElement     * protactinium = new YSElement("Pa", "protactinium", eCharge);
    protactinium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_protactinium");

    protactinium->AddIsotope(new YSIsotope(protactinium->Symbol(), protactinium->Name(), protactinium->Z(), 231, 231.0358840, 1));

    fElements.insert(eCharge, protactinium);
    protactinium->SetPeriodGroup(10, 6);
    protactinium->SetState(YSElement::kSolid);
    protactinium->SetFamily(YSElement::kActinoid);

    // Uranium
    eCharge = 92;
    YSElement     * uranium = new YSElement("U", "uranium", eCharge);
    uranium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_uranium");

    uranium->AddIsotope(new YSIsotope(uranium->Symbol(), uranium->Name(), uranium->Z(), 235, 235.0439299, 0.007204));
    uranium->AddIsotope(new YSIsotope(uranium->Symbol(), uranium->Name(), uranium->Z(), 238, 238.0507882, 0.992742));

    fElements.insert(eCharge, uranium);
    uranium->SetPeriodGroup(10, 7);
    uranium->SetState(YSElement::kSolid);
    uranium->SetFamily(YSElement::kActinoid);

    // Neptunium
    eCharge = 93;
    YSElement     * neptunium = new YSElement("Np", "neptunium", eCharge);
    neptunium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_neptunium");

    neptunium->AddIsotope(new YSIsotope(neptunium->Symbol(), neptunium->Name(), neptunium->Z(), 239, 239.0529390, 0));

    fElements.insert(eCharge, neptunium);
    neptunium->SetPeriodGroup(10, 8);
    neptunium->SetState(YSElement::kSolid);
    neptunium->SetFamily(YSElement::kActinoid);

    // Plutonium
    eCharge = 94;
    YSElement     * plutonium = new YSElement("Pu", "plutonium", eCharge);
    plutonium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_plutonium");

    plutonium->AddIsotope(new YSIsotope(plutonium->Symbol(), plutonium->Name(), plutonium->Z(), 244, 244.064204, 0));

    fElements.insert(eCharge, plutonium);
    plutonium->SetPeriodGroup(10, 9);
    plutonium->SetState(YSElement::kSolid);
    plutonium->SetFamily(YSElement::kActinoid);

    // Americium
    eCharge = 95;
    YSElement     * americium = new YSElement("Am", "americium", eCharge);
    americium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_americium");

    americium->AddIsotope(new YSIsotope(americium->Symbol(), americium->Name(), americium->Z(), 243, 243.0613811, 0));

    fElements.insert(eCharge, americium);
    americium->SetPeriodGroup(10, 10);
    americium->SetState(YSElement::kSolid);
    americium->SetFamily(YSElement::kActinoid);

    // Curium
    eCharge = 96;
    YSElement     * curium = new YSElement("Cm", "curium", eCharge);
    curium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_curium");

    curium->AddIsotope(new YSIsotope(curium->Symbol(), curium->Name(), curium->Z(), 247, 247.070354, 0));

    fElements.insert(eCharge, curium);
    curium->SetPeriodGroup(10, 11);
    curium->SetState(YSElement::kSolid);
    curium->SetFamily(YSElement::kActinoid);

    // Berkelium
    eCharge = 97;
    YSElement     * berkelium = new YSElement("Bk", "berkelium", eCharge);
    berkelium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_berkelium");

    berkelium->AddIsotope(new YSIsotope(berkelium->Symbol(), berkelium->Name(), berkelium->Z(), 247, 247.070307, 0));

    fElements.insert(eCharge, berkelium);
    berkelium->SetPeriodGroup(10, 12);
    berkelium->SetState(YSElement::kSolid);
    berkelium->SetFamily(YSElement::kActinoid);

    // Californium
    eCharge = 98;
    YSElement     * californium = new YSElement("Cf", "californium", eCharge);
    californium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_californium");

    californium->AddIsotope(new YSIsotope(californium->Symbol(), californium->Name(), californium->Z(), 251, 251.079587, 0));

    fElements.insert(eCharge, californium);
    californium->SetPeriodGroup(10, 13);
    californium->SetState(YSElement::kSolid);
    californium->SetFamily(YSElement::kActinoid);

    // Einsteinium
    eCharge = 99;
    YSElement     * einsteinium = new YSElement("Es", "einsteinium", eCharge);
    einsteinium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_einsteinium");

    einsteinium->AddIsotope(new YSIsotope(einsteinium->Symbol(), einsteinium->Name(), einsteinium->Z(), 251, 251.079587, 0));

    fElements.insert(eCharge, einsteinium);
    einsteinium->SetPeriodGroup(10, 14);
    einsteinium->SetState(YSElement::kSolid);
    einsteinium->SetFamily(YSElement::kActinoid);

    // Fermium
    eCharge = 100;
    YSElement     * fermium = new YSElement("Fm", "fermium", eCharge);
    fermium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_fermium");

    fermium->AddIsotope(new YSIsotope(fermium->Symbol(), fermium->Name(), fermium->Z(), 257, 257.095105, 0));

    fElements.insert(eCharge, fermium);
    fermium->SetPeriodGroup(10, 15);
    fermium->SetState(YSElement::kSolid);
    fermium->SetFamily(YSElement::kActinoid);

    // Mendelevium
    eCharge = 101;
    YSElement     * mendelevium = new YSElement("Md", "mendelevium", eCharge);
    mendelevium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_mendelevium");

    mendelevium->AddIsotope(new YSIsotope(mendelevium->Symbol(), mendelevium->Name(), mendelevium->Z(), 257, 257.095105, 0));

    fElements.insert(eCharge, mendelevium);
    mendelevium->SetPeriodGroup(10, 16);
    mendelevium->SetState(YSElement::kSolid);
    mendelevium->SetFamily(YSElement::kActinoid);

    // Nobelium
    eCharge = 102;
    YSElement     * nobelium = new YSElement("No", "nobelium", eCharge);
    nobelium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_nobelium");

    nobelium->AddIsotope(new YSIsotope(nobelium->Symbol(), nobelium->Name(), nobelium->Z(), 259, 259.10103, 0));

    fElements.insert(eCharge, nobelium);
    nobelium->SetPeriodGroup(10, 17);
    nobelium->SetState(YSElement::kSolid);
    nobelium->SetFamily(YSElement::kActinoid);

    // Lawrencium
    eCharge = 103;
    YSElement     * lawrencium = new YSElement("Lr", "lawrencium", eCharge);
    lawrencium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_lawrencium");

    lawrencium->AddIsotope(new YSIsotope(lawrencium->Symbol(), lawrencium->Name(), lawrencium->Z(), 262, 262.10963, 0));

    fElements.insert(eCharge, lawrencium);
    lawrencium->SetPeriodGroup(10, 18);
    lawrencium->SetState(YSElement::kSolid);
    lawrencium->SetFamily(YSElement::kActinoid);

    // rutherfordium
    eCharge = 104;
    YSElement     * rutherfordium = new YSElement("Rf", "rutherfordium", eCharge);
    rutherfordium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_rutherfordium");

    rutherfordium->AddIsotope(new YSIsotope(rutherfordium->Symbol(), rutherfordium->Name(), rutherfordium->Z(), 267, 267.12179, 0));

    fElements.insert(eCharge, rutherfordium);
    rutherfordium->SetPeriodGroup(7, 4);
    rutherfordium->SetState(YSElement::kUnknown);
    rutherfordium->SetFamily(YSElement::kTransitionMetal);

    // dubnium
    eCharge = 105;
    YSElement     * dubnium = new YSElement("Db", "dubnium", eCharge);
    dubnium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_dubnium");

    dubnium->AddIsotope(new YSIsotope(dubnium->Symbol(), dubnium->Name(), dubnium->Z(), 268, 268.12567, 0));

    fElements.insert(eCharge, dubnium);
    dubnium->SetPeriodGroup(7, 5);
    dubnium->SetState(YSElement::kUnknown);
    dubnium->SetFamily(YSElement::kTransitionMetal);

    // seaborgium
    eCharge = 106;
    YSElement     * seaborgium = new YSElement("Sg", "seaborgium", eCharge);
    seaborgium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_seaborgium");

    seaborgium->AddIsotope(new YSIsotope(seaborgium->Symbol(), seaborgium->Name(), seaborgium->Z(), 271, 271.13393, 0));

    fElements.insert(eCharge, seaborgium);
    seaborgium->SetPeriodGroup(7, 6);
    seaborgium->SetState(YSElement::kUnknown);
    seaborgium->SetFamily(YSElement::kTransitionMetal);

    // bohrium
    eCharge = 107;
    YSElement     * bohrium = new YSElement("Bh", "bohrium", eCharge);
    bohrium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_bohrium");

    bohrium->AddIsotope(new YSIsotope(bohrium->Symbol(), bohrium->Name(), bohrium->Z(), 270, 270.13336, 0));

    fElements.insert(eCharge,bohrium);
    bohrium->SetPeriodGroup(7, 7);
    bohrium->SetState(YSElement::kUnknown);
    bohrium->SetFamily(YSElement::kTransitionMetal);

    // hassium
    eCharge = 108;
    YSElement     * hassium = new YSElement("Hs", "hassium", eCharge);
    hassium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_hassium");

    hassium->AddIsotope(new YSIsotope(hassium->Symbol(), hassium->Name(), hassium->Z(), 269, 269.13375, 0));

    fElements.insert(eCharge, hassium);
    hassium->SetPeriodGroup(7, 8);
    hassium->SetState(YSElement::kUnknown);
    hassium->SetFamily(YSElement::kTransitionMetal);

    // meitnerium
    eCharge = 109;
    YSElement     * meitnerium = new YSElement("Mt", "meitnerium", eCharge);
    meitnerium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_meitnerium");

    meitnerium->AddIsotope(new YSIsotope(meitnerium->Symbol(), meitnerium->Name(), meitnerium->Z(), 278, 278.15631, 0));

    fElements.insert(eCharge, meitnerium);
    meitnerium->SetPeriodGroup(7, 9);
    meitnerium->SetState(YSElement::kUnknown);
    meitnerium->SetFamily(YSElement::kTransitionMetal);

    // darmstadtium
    eCharge = 110;
    YSElement     * darmstadtium = new YSElement("Ds", "darmstadtium", eCharge);
    darmstadtium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_darmstadtium");

    darmstadtium->AddIsotope(new YSIsotope(darmstadtium->Symbol(), darmstadtium->Name(), darmstadtium->Z(), 281, 281.16451, 0));

    fElements.insert(eCharge, darmstadtium);
    darmstadtium->SetPeriodGroup(7, 10);
    darmstadtium->SetState(YSElement::kUnknown);
    darmstadtium->SetFamily(YSElement::kTransitionMetal);

    // roentgenium
    eCharge = 111;
    YSElement     * roentgenium = new YSElement("Rg", "roentgenium", eCharge);
    roentgenium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_roentgenium");

    roentgenium->AddIsotope(new YSIsotope(roentgenium->Symbol(), roentgenium->Name(), roentgenium->Z(), 281, 281.16636, 0));

    fElements.insert(eCharge, roentgenium);
    roentgenium->SetPeriodGroup(7, 11);
    roentgenium->SetState(YSElement::kUnknown);
    roentgenium->SetFamily(YSElement::kTransitionMetal);

    // copernicium
    eCharge = 112;
    YSElement     * copernicium = new YSElement("Cn", "copernicium", eCharge);
    copernicium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_copernicium");

    copernicium->AddIsotope(new YSIsotope(copernicium->Symbol(), copernicium->Name(), copernicium->Z(), 285, 285.17712, 0));

    fElements.insert(eCharge, copernicium);
    copernicium->SetPeriodGroup(7, 12);
    copernicium->SetState(YSElement::kUnknown);
    copernicium->SetFamily(YSElement::kTransitionMetal);

    // ununtrium
    eCharge = 113;
    YSElement     * ununtrium = new YSElement("Uut", "ununtrium", eCharge);
    ununtrium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_ununtrium");

    ununtrium->AddIsotope(new YSIsotope(ununtrium->Symbol(), ununtrium->Name(), ununtrium->Z(), 286, 286.18221, 0));

    fElements.insert(eCharge, ununtrium);
    ununtrium->SetPeriodGroup(7, 13);
    ununtrium->SetState(YSElement::kUnknown);
    ununtrium->SetFamily(YSElement::kPostTransitionMetal);

    // flerovium
    eCharge = 114;
    YSElement     * flerovium = new YSElement("Fl", "flerovium", eCharge);
    flerovium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_flerovium");

    flerovium->AddIsotope(new YSIsotope(flerovium->Symbol(), flerovium->Name(), flerovium->Z(), 289, 289.19042, 0));

    fElements.insert(eCharge, flerovium);
    flerovium->SetPeriodGroup(7, 14);
    flerovium->SetState(YSElement::kUnknown);
    flerovium->SetFamily(YSElement::kPostTransitionMetal);

    // ununpentium
    eCharge = 115;
    YSElement     * ununpentium = new YSElement("Uup", "ununpentium", eCharge);
    ununpentium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_ununpentium");

    ununpentium->AddIsotope(new YSIsotope(ununpentium->Symbol(), ununpentium->Name(), ununpentium->Z(), 289, 289.19363, 0));

    fElements.insert(eCharge, ununpentium);
    ununpentium->SetPeriodGroup(7, 15);
    ununpentium->SetState(YSElement::kUnknown);
    ununpentium->SetFamily(YSElement::kPostTransitionMetal);

    // livermorium
    eCharge = 116;
    YSElement     * livermorium = new YSElement("Lv", "livermorium", eCharge);
    livermorium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_livermorium");

    livermorium->AddIsotope(new YSIsotope(livermorium->Symbol(), livermorium->Name(), livermorium->Z(), 293, 293.20449, 0));

    fElements.insert(eCharge, livermorium);
    livermorium->SetPeriodGroup(7, 16);
    livermorium->SetState(YSElement::kUnknown);
    livermorium->SetFamily(YSElement::kPostTransitionMetal);

    //  ununseptium
    eCharge = 117;
    YSElement     *  ununseptium = new YSElement("Uus", " ununseptium", eCharge);
    ununseptium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_ ununseptium");

    ununseptium->AddIsotope(new YSIsotope( ununseptium->Symbol(),  ununseptium->Name(),  ununseptium->Z(), 294, 294.21046, 0));

    fElements.insert(eCharge, ununseptium);
    ununseptium->SetPeriodGroup(7, 17);
    ununseptium->SetState(YSElement::kUnknown);
    ununseptium->SetFamily(YSElement::kHalogen);

    //  ununoctium
    eCharge = 118;
    YSElement     *  ununoctium = new YSElement("Uuo", " ununoctium", eCharge);
    ununoctium->SetReference("http://en.wikipedia.org/wiki/Isotopes_of_ ununoctium");

    ununoctium->AddIsotope(new YSIsotope( ununoctium->Symbol(),  ununoctium->Name(),  ununoctium->Z(), 294, 294.21392, 0));

    fElements.insert(eCharge, ununoctium);
    ununoctium->SetPeriodGroup(7, 18);
    ununoctium->SetState(YSElement::kUnknown);
    ununoctium->SetFamily(YSElement::kNobleGas);
}

//______________________________________________________________________________
YSElement * YSPeriodicTable::FindElement(int z) const
{
    //retrieves a Element from the periodic table by Z value

    YSElement * rv = (YSElement*)fElements.at(z);
    if (!rv) {
        YSElement * element = NULL;
        foreach(element, fElements){
            if (element->Z() == z) {
                rv = element;
                break;
            }
        }
    }
    return rv;
}

//______________________________________________________________________________
YSIsotope *YSPeriodicTable::Neutron()
{
    //gime a neutron
  YSIsotope * neutron = new YSIsotope("n", "neutron", 0, 1, 1.00866491597, 0.) ;
  return neutron ;
}

//______________________________________________________________________________
YSIsotope *YSPeriodicTable::Proton()
{
    //gime a proton
  YSIsotope * proton = new YSIsotope("p", "proton",  1, 1, 1.00794,       0.999885) ;
  return proton ;
}

//______________________________________________________________________________
YSElement * YSPeriodicTable::FindElement(const QString text) const
{
    //retrieves a Element from the periodic table by symbol

    YSElement * rv = NULL; //fElements.FindObject(element);
    YSElement * element;
    foreach(element, fElements) {
        if (element->Symbol() == text || element->Name() == text) {
            rv = element;
            break;
        }
    }
    if(!rv) {
        QString message = QString("%1 ERROR: element %2 not found\n").arg(metaObject()->className()).arg(text);
        qDebug() << message;
    }
    return rv;
}
