//
//  YSPeriodicTable.h
//  PeriodicTableWidget
//
//  Created by Yves Schutz on 14/08/13.
//  Copyright (c) 2013 Yves Schutz. All rights reserved.
//

#ifndef YSPERIODICTABLE_H
#define YSPERIODICTABLE_H

#include <QGraphicsView>
#include <QList>
#include <QListWidget>

#include "YSElement.h"
#include "YSElementBox.h"
class YSPeriodicTable : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit YSPeriodicTable(QGraphicsView *parent = NULL);
    virtual ~YSPeriodicTable();

    void        AddElementBox(YSElementBox*);
    void        AddLegend();
    void        ConnectElements();
    void        Draw();
    void        FillPeriodicTable();
    YSElement * FindElement(const QString) const;
    YSElement * FindElement(int) const;

    static YSIsotope * Neutron() ;
    static YSIsotope * Proton() ;


signals:
    void IsotopeIsSelected(YSIsotope *);

public slots:
    void SelectedIsotope(YSIsotope * iso) {emit IsotopeIsSelected(iso);}

private:
    QList<YSElement *> fElements;      // list of elements stored with increasing charge
};

#endif // YSPERIODICTABLE_H
