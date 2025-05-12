#include <iostream>
#include <QtMath>
#include <QtWidgets>
#include <QWidget>
#include <QApplication>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QAbstractButton>
#include <QMouseEvent>
#include <QTimer>
#include <QAbstractButton>
#include "../include/menu/mainWindow.hpp"
#include "../include/mainScene.hpp"
#include "../include/mainGraphicsView.hpp"
#include "../include/entity/item.hpp"

#include "../include/mainScene.hpp"
#include "../include/mainGraphicsView.hpp"
#include "../include/entity/item.hpp"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    MainWindow mWindow;
    mWindow.showMaximized();

    // Show scene example
    // MainScene scene;       // container for QGraphicsItems

    // MainGraphicsView view(&scene);         // Scrollable area
    // view.setRenderHint(QPainter::Antialiasing);     // Use antialiasing when rendering
  
    // QTimer timer;
    // QObject::connect(&timer, &QTimer::timeout, &scene, &QGraphicsScene::advance);
    // timer.start(1000 / 33);     // 30 fps

    return app.exec();
};