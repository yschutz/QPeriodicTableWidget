//
//  YSElement.cpp
//  PeriodicTableWidget
//
//  Created by Yves Schutz on 14/08/13.
//  Copyright (c) 2013 Yves Schutz. All rights reserved.
//

#include "YSElement.h"

const QString YSElement::kDummy = "dummy" ;

//______________________________________________________________________________
YSElement::YSElement(const QString& symbol, const QString& name, int z) :
    fSymbol(symbol), fName(name), fReference(""),
    fPeriode(0), fGroup(0), fState(YSElement::kUnknown)
{
    // unique allowed ctor

    fZ = z ;
}

//______________________________________________________________________________
YSElement::~YSElement()
{
    // dtor
    qDeleteAll(fIsotopes) ;
    fIsotopes.clear() ;
}

//______________________________________________________________________________
void YSElement::AddIsotope(YSIsotope * iso)
{
    // add a new Element to the isotope list
  fIsotopes.append(iso) ;

  connect(iso, SIGNAL(Select(YSIsotope*)), this, SLOT(SelectedIsotope(YSIsotope*)));

}

//______________________________________________________________________________
int YSElement::Compare(const YSElement * element) const
{
    int rv = 0 ;
    if(Z() < element->Z())
        rv = -1 ;
    else if(Z() > element->Z())
        rv = 1 ;
    return rv ;
}

//______________________________________________________________________________
YSIsotope * YSElement::MostAbundantIsotope() const
{
    // returns the most abundant isotope in the list

    if (fIsotopes.size() == 0)
        return NULL ;
    double max = 0.0 ;
    int    indexMax = 0 ;
    YSIsotope * iso ;
    for (int i = 0; i < fIsotopes.size(); i++ ) {
       iso = fIsotopes.at(i);
        if ( iso->Abundance() >= max ) {
            max = iso->Abundance() ;
            indexMax = i ;
        }
    }
    return fIsotopes.at(indexMax) ;
}

//______________________________________________________________________________
void YSElement::Print() const
{
    // print Element and Isotopes list

    QString message = QString("\n%1: %2, %3(Z=%4)\n").arg(metaObject()->className()).arg(Name()).arg(Symbol()).arg(Z()) ;
    qDebug() << message ;
    YSIsotope * iso ;
      foreach( iso, fIsotopes)
          iso->Print() ;
     message = QString("\nref: %1\n").arg(Reference()) ;
      qDebug() << message ;
}


