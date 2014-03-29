//
//  YSElement.h
//  PeriodicTableWidget
//
//  Created by Yves Schutz on 14/08/13.
//  Copyright (c) 2013 Yves Schutz. All rights reserved.
//

#ifndef YSELEMENT_H
#define YSELEMENT_H

#include <QDebug>
#include <QList>
#include <QObject>

#include "YSIsotope.h"

class QString;

class YSElement : public QObject
{
    Q_OBJECT

public:
    enum YSElementState {
        kNoState, kUnknown, kSolid, kLiquid, kGas
    };

    enum YSElementFamily {
        kNoFamily, kMetalloid, kOtherNonMetal, kHalogen, kNobleGas,
        KAlkaliMetal, kAlkalineEarthMetal,
        kLanthanoid, kActinoid, kTransitionMetal,
        kPostTransitionMetal
    };

    static const QString kDummy ;

    YSElement(const QString&  ,const QString& , int) ;
    virtual ~YSElement() ;

    void                    AddIsotope(YSIsotope*) ;
    int                     Compare(const YSElement *) const ;
    YSIsotope *             GetIsotope(int index) const { return fIsotopes.at(index) ; }
    const QList<YSIsotope*>& GetIsotopes() const {return fIsotopes;}
    int                     GetPeriode() const { return fPeriode ; }
    YSElementFamily         GetFamily() const { return fFamily ;}
    int                     GetGroupe() const {return fGroup ; }
    YSElementState          GetState() const { return fState ; }
    bool                    IsSortable() const { return true; }
    YSIsotope *             MostAbundantIsotope() const ;
    const QString&          Name() const { return fName ; }
    int                     NumberOfIsotopes() const { return fIsotopes.size() ; }
    void                    Print() const ;
    const QString&          Reference() const { return fReference ; }
    void                    SetFamily(YSElementFamily f) { fFamily = f ; }
    void                    SetPeriodGroup(int p, int g) { fPeriode = p  ; fGroup = g ; }
    void                    SetReference(const char * ref) { fReference = ref ; }
    void                    SetState(YSElementState s) { fState = s ; }
    const QString&          Symbol() const { return fSymbol ;}
    int                     Z() const { return fZ ; }

signals:

    void IsotopeSelected(YSIsotope *);

public slots:

    void SelectedIsotope(YSIsotope * iso) {emit IsotopeSelected(iso);}

private:

    QString              fSymbol, fName, fReference ; // full name and symbolic name and wikipedia link of element
    int                  fZ ;                         // charge of element
    QList<YSIsotope *>   fIsotopes ;                  // list of element's isotopes
    YSElementFamily      fFamily ;                    // among Element Family
    int                  fPeriode, fGroup ;           // periode and group in periodic table
    YSElementState       fState ;                     // among ElementState                   // state of the element
};

#endif // YSELEMENT_H
