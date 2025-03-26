#include <iostream>
#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextEdit>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app (argc, argv); 
    MainWindow window;
    window.show();
    
    return app.exec();
};