#include <iostream>
#include "../include/mainGraphicsView.hpp"
#include "../include/menu/mainWindow.hpp"
#include "../include/mainScene.hpp"
#include <QVBoxLayout>
#include <QTimer>
#include <QApplication>
#include <QGraphicsView>



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

    mainMenu->setLayout(mainLayout);
    this->setCentralWidget(mainMenu);
    this->connect(this->quitGame, &QPushButton::clicked, [=](){QCoreApplication::quit();}); //Dernier paramètre trouvé avec IA
    // this->connect(this->scoreBoard, &QPushButton::clicked, [=](){});
    this->connect(this->newGame, &QPushButton::clicked, [=](){
        MainScene scene;
        newGameClicked(&scene);
    });
};
void MainWindow::newGameClicked(MainScene *scene){
    MainGraphicsView view(scene);
    view.setRenderHint(QPainter::Antialiasing);
    // QTimer timer;
    // QObject::connect(&timer, &QTimer::timeout, &scene, &QGraphicsScene::advance);
    // timer->start(1000 / 33);
}