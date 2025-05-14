#ifndef MAINGRAPHICSVIEW_HPP
#define MAINGRAPHICSVIEW_HPP

#include <QGraphicsView>
#include <QWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include "mainScene.hpp"

class MainGraphicsView : public QGraphicsView {
public:
    MainGraphicsView(MainScene* scene, QWidget* parent = nullptr);
    ~MainGraphicsView();
    void mouseMoveEvent(QMouseEvent* event) override;
    void centerOnPlayer(Player* player);
protected:
    void wheelEvent(QWheelEvent *event) override;
private:
};

#endif   // MAINGRAPHICSVIEW_HPP