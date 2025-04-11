#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <QKeyEvent>
#include "livingEntity.hpp"
#include "item.hpp"

#define PLAYER_SPEED 0.05

class Player : public LivingEntity {
private:
    bool leftKeyPressed = false;
    bool rightKeyPressed = false;
    bool upKeyPressed = false;
    bool downKeyPressed = false;
    
    void initFlags();

public:
    // Constructors/destructors
    Player();
    Player(const Player& other);
    Player(qreal life, const Vector2 position, const Vector2 dimensions, Sprites::SpriteImage sprite = Sprites::SpriteImage::None);
    ~Player();

    void gatherItem(Item* item);

    // Inherited methods
    virtual void onDeath();
    virtual void onCollide(Entity* other);
    virtual bool onUpdate(qint64 deltaTime);
    virtual Entity* getSpawned();

    // Input events
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
};

#endif   // PLAYER_HPP