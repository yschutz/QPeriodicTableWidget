#include "YSPTView.h"

#include <QAction>
#include <QContextMenuEvent>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QMenu>
#include <QMouseEvent>
#include <QScrollBar>
#include <QTextStream>
#include <QWheelEvent>

#include "YSPeriodictable.h"

//______________________________________________________________________________
YSPTView::YSPTView(QWidget* parent) : QGraphicsView(parent)
{
    // Sets up the subclassed QGraphicsView

    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);


    //Set-up the scene

    fPt = new YSPeriodicTable(this);
    setScene(fPt);

    //Set-up the view
    setSceneRect(fPt->sceneRect());

    //Use ScrollHand Drag Mode to enable Panning
    setDragMode(ScrollHandDrag);
}

//______________________________________________________________________________
YSPTView::YSPTView(QMenuBar *mb)
{
    //creates the view with a shared menubar

    // Sets up the subclassed QGraphicsView

    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);


    //Set-up the scene

    fPt = new YSPeriodicTable(this);
    setScene(fPt);

    //Set-up the view
    setSceneRect(fPt->sceneRect());

    //Use ScrollHand Drag Mode to enable Panning
    setDragMode(ScrollHandDrag);

    fMenuBar = mb;
    CreateActions();
    CreateMenus();
}


//______________________________________________________________________________
YSPTView::~YSPTView()
{
    // dtor

    delete fPt;
}

//______________________________________________________________________________
void YSPTView::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(fActionQuit);
    menu.exec(event->globalPos());
}

//______________________________________________________________________________
void YSPTView::CreateActions()
{
    // create the actions for the menu items
    fActionQuit = new QAction("&Done", this);
    fActionQuit->setShortcuts(QKeySequence::Quit);
    fActionQuit->setStatusTip(tr("Quit this view"));
    connect(fActionQuit, SIGNAL(triggered()), this, SLOT(Close()));
}

//______________________________________________________________________________
void YSPTView::CreateMenus()
{
    // create the menu (so far only Done implemented)
    fMenu = fMenuBar->addMenu("Periodic Table");
    fMenu->addAction(fActionQuit);
}

//______________________________________________________________________________
void YSPTView::wheelEvent(QWheelEvent* event)
{
    //Zoom the view in and out.

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    // Scale the view / do the zoom
    double scaleFactor = 1.15;
    if(event->delta() > 0) {
        // Zoom in
        scale(scaleFactor, scaleFactor);
    } else {
        // Zooming out
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }

    // Don't call superclass handler here
    // as wheel is normally used for moving scrollbars
}
