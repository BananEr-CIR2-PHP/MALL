#ifndef MISSILE_HPP
#define MISSILE_HPP

#include "../vector2.hpp"
#include "entity.hpp"

class Missile : public Entity {
protected:
    Vector2 velocity;
    qreal lifetime;    // Distance left to travel before despawn
    qreal damage;
    bool pierceEntities;

    Missile(const Missile& other);

public:
    // Constructor/destructor
    Missile();
    Missile(const Vector2 velocity, const qreal range, const qreal damage, const bool pierceEntities, const Vector2 position, const Vector2 dimensions, Sprites::SpriteImage sprite = Sprites::SpriteImage::None, Teams::Team team = Teams::None);
    ~Missile();

    // Getters
    Vector2 getSpeed() const;

    // Setters
    void setSpeed(const Vector2 speed);

    // Inherited methods
    virtual void onCollide(Entity* other);
    virtual bool onUpdate(qint64 deltaTime);
    virtual Entity* getSpawned();
};

#endif   // MISSILE_HPP