#ifndef MAINGRAPHICSVIEW_HPP
#define MAINGRAPHICSVIEW_HPP

#include <QGraphicsView>
#include <QWidget>
#include "mainScene.hpp"

class MainGraphicsView : public QGraphicsView {
public:
    MainGraphicsView(MainScene* scene, QWidget* parent = nullptr);
    ~MainGraphicsView();
};

#endif   // MAINGRAPHICSVIEW_HPP