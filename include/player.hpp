#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "livingEntity.hpp"
#include "item.hpp"

class Player : public LivingEntity {
public:
    // Constructors/destructors
    Player();
    Player(const Player& other);
    Player(qreal life, const Vector2 position, const Vector2 dimensions, Sprite* sprite=nullptr);
    ~Player();

    void gatherItem(Item::ItemType type);

    // Inherited methods
    virtual void onDeath();
    virtual void onCollide(Entity& other);
    virtual void onUpdate(qint64 deltaTime);
};

#endif   // PLAYER_HPP