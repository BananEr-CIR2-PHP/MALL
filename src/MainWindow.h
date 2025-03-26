#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>

class MainWindow : public QMainWindow {
        Q_Object
    public :
        MainWindow(QWidget* parent = nullptr);
        virtual ~MainWindow(){};
    private:
        QPushButton* button;
        QTextEdit* textedit;
};
#endif