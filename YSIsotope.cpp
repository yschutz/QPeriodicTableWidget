//
//  YSIsotope.cpp
//  PeriodicTableWidget
//
//  Created by Yves Schutz on 15/08/13.
//  Copyright (c) 2013 Yves Schutz. All rights reserved.
//

#include <QDebug>

#include "YSIsotope.h"
#include "YSElement.h"

//______________________________________________________________________________
YSIsotope::YSIsotope() : fName("A"), fSymbol("A")
{
    // default ctor

    fZ          = 0 ;
    fA          = 0 ;
    fN          = fA - fZ  ;
    fAtomicMass = 0.0 ;
    fAbundance  = 0.0 ;
}

//______________________________________________________________________________

YSIsotope::YSIsotope(const QString symbol, const QString name, int z, int a, double amass, double abundance) : fName(name), fSymbol(symbol)
{
    // ctor

    fZ          = z ;
    fA          = a ;
    fN          = fA - fZ  ;
    fAtomicMass = amass ;
    fAbundance  = abundance ;
}

//______________________________________________________________________________
YSIsotope::YSIsotope(const YSIsotope & iso)
{
    // copy ctor
    ((YSIsotope&)iso).Copy(*this) ;
}

//______________________________________________________________________________
int YSIsotope::Compare(const YSIsotope * iso) const
{
    //compare 2 isotopes for sorting according to A
    int rv = 0 ;
    if(A() < ((YSIsotope*)iso)->A())
        rv = -1 ;
    else if(A() > ((YSIsotope*)iso)->A())
        rv = 1 ;
    return rv ;
}

//______________________________________________________________________________
void YSIsotope::Copy(YSIsotope &obj) const
{
    //needed by the copy ctor

    ((YSIsotope&)obj).fName = fName ;
    ((YSIsotope&)obj).fSymbol = fSymbol ;
    ((YSIsotope&)obj).fZ = fZ ;
    ((YSIsotope&)obj).fA = fA ;
    ((YSIsotope&)obj).fN = fN ;
    ((YSIsotope&)obj).fAtomicMass = fAtomicMass ;
    ((YSIsotope&)obj).fAbundance = fAbundance ;
}

//______________________________________________________________________________
double YSIsotope::Mass(int unit ) const
{
    // returns mass in various units
    double fac= 0.0 ;
    switch (unit) {
    case U_EV:
        fac = 1E6 ;
        break ;
    case U_KEV:
        fac = 1E3 ;
        break ;
    case U_MEV:
        fac = 1. ;
        break ;
    case U_GEV:
        fac = 1E-3 ;
        break ;
    case U_TEV:
        fac = 1E-6 ;
        break ;
    default:
        break ;
    }
    return Mass() * fac ;
}

//______________________________________________________________________________
void YSIsotope::Print() const
{
    // print the isotope

  QString message = QString("\n%1: %2, %3%4(Z=%5, N=%6)\n").arg(metaObject()->className()).arg(Name()).arg(A()).arg(Symbol()).arg(Z()).arg(N()) ;
  qDebug() << message ;
}


