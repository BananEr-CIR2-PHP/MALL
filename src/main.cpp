#include <iostream>
#include <QApplication>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTextEdit>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget *stMenu = new QWidget;
    QPushButton *newGame = new QPushButton("Nouvelle Partie");
    QPushButton *quitGame = new QPushButton("Quitter le jeu");
    QHBoxLayout *stMenuLayout = new QHBoxLayout(stMenu);
    stMenuLayout->addWidget(newGame);
    stMenuLayout->addWidget(quitGame);
    
    QMainWindow *mainWindow = new QMainWindow(stMenu);
    mainWindow->setWindowTitle("Main Window");
    QMenuBar *menuBar = mainWindow->menuBar();

    QMenu *fileMenu = menuBar->addMenu("Fichier");
    fileMenu->addAction("Nouveau");
    fileMenu->addAction("Ouvrir le jeu");
    fileMenu->addAction("Enregistrer");
    fileMenu->addAction("Quitter", &app, &QApplication::quit);

    stMenu->show();
    mainWindow->show();
    return app.exec();
};
