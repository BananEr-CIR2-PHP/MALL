#include <iostream>
#include <QtMath>
#include <QtWidgets>

#include "../include/item.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QGraphicsScene scene;       // container for QGraphicsItems
    scene.setSceneRect(0, 0, 1000, 1000);       // Scene size

    scene.setItemIndexMethod(QGraphicsScene::NoIndex);      // Collision detection method : linear

    Item* it = new Item(Vector2(50, 600), Vector2(50, 50), new Sprite(new QImage("../res/img/coin.png")));
    scene.addItem(it);


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
