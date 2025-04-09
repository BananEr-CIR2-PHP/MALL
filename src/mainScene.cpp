#include "../include/mainScene.hpp"
#include "../include/item.hpp"

MainScene::MainScene() {
    setSceneRect(0, 0, 1000, 1000);       // Scene size
    setItemIndexMethod(QGraphicsScene::NoIndex);      // Collision detection method : linear

    Item* it = new Item(Vector2(50, 600), Vector2(50, 50), new Sprite(new QImage("../res/img/coin.png")));
    addItem(it);
}

MainScene::~MainScene() { }