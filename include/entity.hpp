#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <QtGlobal>

#include "vector2.hpp"
#include "sprite.hpp"

class Entity {
private:
    Vector2 collisionTL;    // Collision box, top left corner
    Vector2 collisionBR;    // Collision box, bottom right corner
    Vector2 position;
    Vector2 dimensions;

    void updateCollisionBox();

protected:
    const Sprite* sprite = nullptr;     // sprite object cannot be modified but pointer can

public:
    // Constructor/destructor
    Entity();
    Entity(const Entity& other);
    Entity(const Vector2 position, const Vector2 dimensions, const Sprite* sprite=nullptr);
    ~Entity();

    // Getters
    Vector2 getPos() const;
    Vector2 getDims() const;

    // Setters
    void setPos(const Vector2 pos);
    void setDims(const Vector2 dims);

    // Methods
    bool collidesWith(const Entity& other) const;

    // Abstract methods
    virtual void onCollide(Entity& other) = 0;
    virtual void onUpdate(qint64 deltaTime) = 0;
};

#endif   // ENTITY_HPP