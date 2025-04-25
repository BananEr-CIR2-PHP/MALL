#include <iostream>
#include <QWidget>
#include <QApplication>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include "../include/menu/mainWindow.hpp"
#include "../include/mainScene.hpp"
#include "../include/mainGraphicsView.hpp"
#include "../include/entity/item.hpp"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent){
    this->setWindowTitle("MALL");
    
    //QScreen *screen = gApp->primaryScreen();
    //QSize screenSize = screen->availableSize();

    
    QWidget* mainMenu = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(mainMenu);
    QWidget* boxLayout = new QWidget(mainMenu);
    QHBoxLayout* buttonLayout = new QHBoxLayout(boxLayout);
    
    this->pseudoInput = new QLineEdit(mainMenu);
    this->newGame = new QPushButton("Nouvelle Partie");
    this->quitGame = new QPushButton("Quitter le jeu");
    this->scoreBoard = new QPushButton("Tableau des scores");
    
    
    pseudoInput->setPlaceholderText("Nom du Héros");
    mainLayout->addWidget(pseudoInput);

    buttonLayout->addWidget(newGame);
    buttonLayout->addWidget(scoreBoard);
    buttonLayout->addWidget(quitGame);

    boxLayout->setLayout(buttonLayout);

    mainLayout->addWidget(boxLayout);

    // "&QApplication::quit" -> pour quitter automatiquement, jsp comment l'intégrer
    mainMenu->setLayout(mainLayout);
    this->setCentralWidget(mainMenu);
};
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