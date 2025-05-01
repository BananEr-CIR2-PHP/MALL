#ifndef ROCKET_HPP
#define ROCKET_HPP

#include "missile.hpp"
#include "effectZone.hpp"

class Rocket : public Missile{
protected:
    Effect* effect;
    qreal effectRange;

    Rocket(const Rocket& other);

public:
    // Constructor/Destructor
    Rocket();
    Rocket(const Effect effect, const qreal effectRange, const Vector2 velocity, const qreal range, const qreal damage, const bool pierceEntities, const Vector2 position, const Vector2 dimensions, const QString sprite = "", Teams::Team team = Teams::None);
    Rocket(const Effect effect, const qreal effectRange, const Vector2 velocity, const qreal range, const Vector2 position, const Vector2 dimensions, const QString sprite = "", Teams::Team team = Teams::None);
    ~Rocket();

    Missile* copy() const override;

    // Methods
    void explode();

    // Inherited methods
    virtual void onCollide(Entity* other);
    virtual bool onUpdate(qint64 deltaTime);
    virtual Entity* getSpawned();
};

#endif   // ROCKET_HPP