#ifndef MISSILE_HPP
#define MISSILE_HPP

#include "vector2.hpp"
#include "entity.hpp"

class Missile : public Entity {
protected:
    Vector2 velocity;

public:
    // Constructor/destructor
    Missile();
    Missile(const Missile& other);
    Missile(const Vector2 velocity, const Vector2 position, const Vector2 dimensions, const Sprite* sprite = nullptr);
    ~Missile();

    // Getters
    Vector2 getSpeed() const;

    // Setters
    void setSpeed(const Vector2 speed);

    // Inherited methods
    virtual void onCollide(Entity& other);
    virtual void onUpdate(qint64 deltaTime);
};

#endif   // MISSILE_HPP