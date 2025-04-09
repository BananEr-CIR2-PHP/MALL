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
    
    QMainWindow *mainWindow = new QMainWindow;
    mainWindow->setWindowTitle("Main Window");
    

    QWidget *stMenu = new QWidget(mainWindow);
    QPushButton *newGame = new QPushButton("Nouvelle Partie");
    QPushButton *quitGame = new QPushButton("Quitter le jeu");
    QHBoxLayout *stMenuLayout = new QHBoxLayout(stMenu);
    stMenuLayout->addWidget(newGame);
    stMenuLayout->addWidget(quitGame);
    // "&QApplication::quit" -> pour quitter automatiquement, jsp comment l'intégrer

    mainWindow->showMaximized();

    return app.exec();
};
