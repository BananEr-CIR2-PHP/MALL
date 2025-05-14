#include <iostream>
#include "../include/mainGraphicsView.hpp"
#include "../include/menu/mainWindow.hpp"
#include "../include/mainScene.hpp"
#include <QVBoxLayout>
#include <QTimer>
#include <QObject>
#include <QAction>
#include <QApplication>
#include <QGraphicsItem>
#include <QDebug>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDebug>
#include "../include/entity/item.hpp"


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent){
    this->setWindowTitle("MALL");
    
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
    QAction *showViewAction = new QAction("Show Graphics View", this);
    newGame->addAction(showViewAction);

    mainMenu->setLayout(mainLayout);
    this->setCentralWidget(mainMenu);
    this->connect(this->quitGame, &QPushButton::clicked, [this](){QCoreApplication::quit();}); //Dernier paramètre trouvé avec IA
    // this->connect(this->scoreBoard, &QPushButton::clicked, [=](){});
    
    this->connect(this->newGame, &QPushButton::clicked, [this](){
        scene = new MainScene(this, 60);
        scene->setBackgroundTile("../res/img/background2.png");
        newGameClicked(scene);
    });
};

void MainWindow::newGameClicked(MainScene *scene){
    view = new MainGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [scene](){scene->advance();});
    timer.start(1000 / 33);
    setCentralWidget(view);
    this->centerOnSelectedPlayer(view,scene);
}

void MainWindow::centerOnSelectedPlayer(MainGraphicsView* view, MainScene* scene) {
    view->centerOnPlayer(scene->getMainPlayer());
}
