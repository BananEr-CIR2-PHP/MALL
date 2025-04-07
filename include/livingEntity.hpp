#ifndef LIVING_ENTITY_HPP
#define LIVING_ENTITY_HPP

#include <QtGlobal>
#include "entity.hpp"

// Abstract class
class LivingEntity : public Entity {
protected:
    qreal life;

    LivingEntity(const LivingEntity& other);

public:
    // Constructors/destructors
    LivingEntity();
    LivingEntity(qreal life, const Vector2 position, const Vector2 dimensions, Sprite* sprite=nullptr);
    ~LivingEntity();

    // Getters
    qreal getLife() const;

    // Setters
    void setLife(const qreal life);
    void takeDamage(const qreal damage);

    // Abstract methods
    virtual void onDeath() = 0;
};

#endif   // LIVING_ENTITY_HPP