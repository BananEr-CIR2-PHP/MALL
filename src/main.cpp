#include <iostream>
#include <QtMath>
#include <QtWidgets>

#include "../include/mainScene.hpp"
#include "../include/item.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainScene scene;       // container for QGraphicsItems

    QGraphicsView view(&scene);         // Scrollable area
    view.setRenderHint(QPainter::Antialiasing);     // Use antialiasing when rendering

    // Options
    view.setCacheMode(QGraphicsView::CacheBackground);
    view.setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view.setDragMode(QGraphicsView::ScrollHandDrag);

    view.setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "Mall"));
    view.resize(1000, 1000);
    view.show();

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, &scene, &QGraphicsScene::advance);
    timer.start(1000 / 33);     // 30 fps

    return app.exec();
}
