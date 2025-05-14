#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP
#include <QWidget>
#include <QObject>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QTimer>
#include <QScreen>
#include "../mainScene.hpp"
#include "../mainGraphicsView.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow(){};
    void newGameClicked(MainScene *scene);
    void centerOnSelectedPlayer(MainGraphicsView* view, MainScene* scene);
private:
    QLineEdit* pseudoInput = nullptr;
    QPushButton* newGame = nullptr;
    QPushButton* quitGame = nullptr;
    QPushButton* scoreBoard = nullptr;
    MainScene* scene = nullptr;
    MainGraphicsView* view = nullptr;
};

#endif