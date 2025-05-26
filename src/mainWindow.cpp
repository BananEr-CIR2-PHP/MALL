#include <iostream>
#include "../include/mainGraphicsView.hpp"
#include "../include/menu/mainWindow.hpp"
#include "../include/mainScene.hpp"
#include <QVBoxLayout>
#include <QTimer>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPixmap>
#include <QDialog>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QObject>
#include <QAction>
#include <QLabel>
#include <QPixmap>
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

    QLabel* logo = new QLabel(mainMenu);
    QPixmap pixmap("../res/img/logo.png");
    logo->setPixmap(pixmap);
    logo->setScaledContents(true);
    QWidget* logoWidget = new QWidget(mainMenu);
    logoWidget->setContentsMargins(400,100,400,0);
    QHBoxLayout* logoLayout = new QHBoxLayout(logoWidget);
    logoLayout->addWidget(logo);
    

    this->pseudoInput = new QLineEdit(mainMenu);
    this->heroName = new QString();
    pseudoInput->setPlaceholderText("Nom du Héros");
    pseudoInput->setMaxLength(50);
    QWidget* pseudoWidget = new QWidget(mainMenu);
    pseudoWidget->setContentsMargins(300,0,300,0);
    QHBoxLayout* pseudoLayout = new QHBoxLayout(pseudoWidget);
    pseudoLayout->addWidget(pseudoInput);

    this->newGame = new QPushButton("Nouvelle Partie");
    this->quitGame = new QPushButton("Quitter le jeu");
    this->scoreBoard = new QPushButton("Tableau des scores");  
    QWidget* buttonWidget = new QWidget(mainMenu);
    buttonWidget->setContentsMargins(100,0,100,200);
    QHBoxLayout* buttonLayout = new QHBoxLayout(buttonWidget);
    buttonLayout->addWidget(newGame);
    buttonLayout->addWidget(scoreBoard);
    buttonLayout->addWidget(quitGame);

    logoWidget->setLayout(logoLayout);
    pseudoWidget->setLayout(pseudoLayout);
    buttonWidget->setLayout(buttonLayout);

    mainLayout->addWidget(logoWidget);
    mainLayout->addWidget(pseudoWidget);
    mainLayout->addWidget(buttonWidget);
    QAction *showViewAction = new QAction("Show Graphics View", this);
    newGame->addAction(showViewAction);

    mainMenu->setLayout(mainLayout);
    this->setCentralWidget(mainMenu);
    this->connect(this->quitGame, &QPushButton::clicked, [this](){QCoreApplication::quit();}); //Dernier paramètre trouvé avec IA
    this->connect(this->scoreBoard, &QPushButton::clicked, [this](){
        QDialog* scoreDialog = new QDialog(this);
        scoreDialog->setWindowTitle("Tableau des scores");
        scoreDialog->setMinimumSize(500, 500);
        QVBoxLayout* scoreLayout = new QVBoxLayout(scoreDialog);

        QTableWidget* scoreTable = new QTableWidget(scoreDialog);
        scoreTable->setColumnCount(2);
        scoreTable->setHorizontalHeaderLabels({"Joueur", "Score"});
        scoreTable->setRowCount(10); //A modifier en fonction du nombre de parties lancées
        scoreTable->setWordWrap(true);

        scoreLayout->addWidget(scoreTable);
        scoreDialog->setLayout(scoreLayout);
        scoreDialog->exec();
    });
    this->connect(this->newGame, &QPushButton::clicked, [this](){
        *this->heroName = pseudoInput->text();
        qDebug() << *this->heroName;

        scene = new MainScene(this, 60);
        scene->setBackgroundTile("../res/img/background2.png");
        newGameClicked(scene);
        scene->setFocus();
    });
};

void MainWindow::newGameClicked(MainScene *scene){
    view = new MainGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);
    QTimer timer;
    timer.start(1000 / 33);
    setCentralWidget(view);
    this->centerOnSelectedPlayer(view,scene);
    scene->setFocus();
}

void MainWindow::centerOnSelectedPlayer(MainGraphicsView* view, MainScene* scene) {
    view->centerOnPlayer(scene->getMainPlayer());
}

QString* MainWindow::getHeroName(){
    return this->heroName;
}

void MainWindow::setHeroName(QString* name){
    this->heroName = name;
}