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

    setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "Mall"));
    resize(1000, 1000);
    show();
}

/**
 * Destructor
 */
MainGraphicsView::~MainGraphicsView() { }