#include <iostream>
#include <QtMath>
#include <QtWidgets>
#include <QApplication>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QScreen>
#include <QGuiApplication>

#include "../include/mainScene.hpp"
#include "../include/mainGraphicsView.hpp"
#include "../include/entity/item.hpp"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QMainWindow *mainWindow = new QMainWindow;
    
    //QScreen *screen = gApp->primaryScreen();
    //QSize screenSize = screen->availableSize();

    
    QWidget *stMenu = new QWidget(mainWindow);
    QVBoxLayout *menuLayout = new QVBoxLayout(stMenu);
    QLineEdit *pseudoInput = new QLineEdit(stMenu);
    QWidget *boxLayout = new QWidget(stMenu);
    QPushButton *newGame = new QPushButton("Nouvelle Partie");
    QPushButton *quitGame = new QPushButton("Quitter le jeu");
    QPushButton *scoreBoard = new QPushButton("Tableau des scores");
    QHBoxLayout *buttonLayout = new QHBoxLayout(boxLayout);

    mainWindow->setWindowTitle("Main Window");
    //int mWinWidth = (&screenSize)->width();
    //int mWinHeight = (&screenSize)->height();

    stMenu->setGeometry(450,300,500,300);
    
    pseudoInput->setPlaceholderText("Nom du Héros");
    menuLayout->addWidget(pseudoInput);
    
    boxLayout->setGeometry(0,150,500,150);

    buttonLayout->addWidget(newGame);
    buttonLayout->addWidget(scoreBoard);
    buttonLayout->addWidget(quitGame);
    
    menuLayout->addWidget(boxLayout);

    // "&QApplication::quit" -> pour quitter automatiquement, jsp comment l'intégrer

    mainWindow->showMaximized();
  
    // Show scene example
    // MainScene scene;       // container for QGraphicsItems

    // MainGraphicsView view(&scene);         // Scrollable area
    // view.setRenderHint(QPainter::Antialiasing);     // Use antialiasing when rendering
  
    // QTimer timer;
    // QObject::connect(&timer, &QTimer::timeout, &scene, &QGraphicsScene::advance);
    // timer.start(1000 / 33);     // 30 fps

    return app.exec();
};