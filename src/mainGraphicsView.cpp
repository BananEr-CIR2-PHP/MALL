#include <QApplication>
#include "../include/mainGraphicsView.hpp"

/**
 * Default constructor
 */
MainGraphicsView::MainGraphicsView(MainScene* scene, QWidget* parent) : QGraphicsView(scene, parent) {
    setRenderHint(QPainter::Antialiasing);     // Use antialiasing when rendering

    // Options
    setCacheMode(QGraphicsView::CacheBackground);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setMouseTracking(true);

    setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "Mall"));
    resize(1000, 1000);
    show();
}

/**
 * Handle mouse move event
 */
void MainGraphicsView::mouseMoveEvent(QMouseEvent* event) {
    // Since there was a weird bug here and nothing talking about it on internet
    // I decided to do the QGraphicsView's job by hand
    // It definitely is not the clean way, so feel free to change
    QGraphicsSceneMouseEvent sceneEvent(QEvent::GraphicsSceneMouseMove);
    sceneEvent.setScenePos(mapToScene(event->pos()));
    QApplication::sendEvent(scene(), &sceneEvent);
}

/**
 * Destructor
 */
MainGraphicsView::~MainGraphicsView() { }