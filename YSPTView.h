//
//  YSPTView.h
//  PeriodicTableWidget
//
//  Created by Yves Schutz on 01/09/13.
//  Copyright (c) 2013 Yves Schutz. All rights reserved.
//

#ifndef YSPTView_H
#define YSPTView_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsRectItem>

class QAction;
class QContextMenuEvent;
class QMenu;
class QMenuBar;
class YSPeriodicTable ;

class YSPTView: public QGraphicsView
{

    Q_OBJECT

public:
    YSPTView(QWidget* parent = NULL);
    YSPTView(QMenuBar * mb);

    virtual ~YSPTView();

    YSPeriodicTable * PT() const { return fPt; }

protected:
    virtual void wheelEvent(QWheelEvent* event);
    void         contextMenuEvent(QContextMenuEvent *event);

public slots:
    void  Close() { delete this; }

private:
    void CreateActions();
    void CreateMenus();

//private:
    YSPeriodicTable *fPt;         // the periodic table
    QAction         *fActionQuit; // the quit action in the menu
    QMenu           *fMenu;       // the unique menu
    QMenuBar        *fMenuBar;    // the menu bar
};

#endif // YSPTView_H
