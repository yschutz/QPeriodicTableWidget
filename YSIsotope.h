//
//  YSIsotope.h
//  PeriodicTableWidget
//
//  Created by Yves Schutz on 15/08/13.
//  Copyright (c) 2013 Yves Schutz. All rights reserved.
//

#ifndef YSISOTOPE_H
#define YSISOTOPE_H

#include <QObject>
#include <QString>

class YSElement;

static const double kgAtomicMassUnit = 931.494061 ; // MeV/c2


class YSIsotope : public QObject
{
    Q_OBJECT

public:

    YSIsotope();
    YSIsotope(const QString, const QString , int, int, double, double) ;
    YSIsotope(const YSIsotope &) ;
    virtual ~YSIsotope() {;}

    int           A() const { return fA ; }
    double        Abundance() const { return fAbundance * 100 ; }
    double        AtomicMass() const { return fAtomicMass ; }
    virtual int   Compare(const YSIsotope *) const ;
    void          Copy(YSIsotope &) const ;
    bool          IsSortable() const { return true ; }
    double        Mass() const { return fAtomicMass * kgAtomicMassUnit ; }
    double        Mass(int) const ;
    int           N() const { return fN ; }
    const QString Name() const { return fName ; }
    void          SetAtomicMass(double mass) { fAtomicMass = mass ; }
    const QString Symbol() const { return fSymbol ; }
    void          Print() const ;
    int           Z() const { return fZ ; }
//    bool operator==(const YSIsotope & iso) const  { return (iso.fA == fA && iso.fZ == fZ) ? true : false ; }


    enum MAssUnitOption {
        U_EV,
        U_KEV,
        U_MEV,
        U_GEV,
        U_TEV
    };

signals:
    void Select(YSIsotope*);

public slots:
    void Selected() {emit Select(this); }

private:
    QString fName, fSymbol;             // name and symbol of isotope
    int     fA, fN, fZ  ;               // nucleon number, neutron number, proton number
    double  fAtomicMass, fAbundance;   // atomic mass and abudance of isotope in %
};

#endif // YSISOTOPE_H
