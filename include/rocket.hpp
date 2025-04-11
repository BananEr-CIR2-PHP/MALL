#ifndef ROCKET_HPP
#define ROCKET_HPP

#include "missile.hpp"

class Rocket : public Missile{
protected:
    Rocket(const Rocket& other);
public:
    // Constructor/Destructor
    Rocket();
    Rocket(const Vector2 velocity, const qreal range, const qreal damage, const bool pierceEntities, const Vector2 position, const Vector2 dimensions, Sprites::SpriteImage sprite = Sprites::SpriteImage::None);
    Rocket(const Vector2 velocity, const qreal range, const Vector2 position, const Vector2 dimensions, Sprites::SpriteImage sprite = Sprites::SpriteImage::None);
    ~Rocket();

    // Methods
    void explode();

    // Inherited methods
    virtual void onCollide(Entity* other);
    virtual void onUpdate(qint64 deltaTime);
};

#endif   // ROCKET_HPP