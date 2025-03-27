#ifndef LIVING_ENTITY_HPP
#define LIVING_ENTITY_HPP

#include "entity.hpp"

// Abstract class
class LivingEntity : public Entity{
protected:
    double life;

public:
    // Constructors/destructors
    LivingEntity();
    LivingEntity(const LivingEntity& other);
    LivingEntity(double life, const Vector2 position, const Vector2 dimensions, const Sprite* sprite=nullptr);
    ~LivingEntity();

    // Getters
    double getLife() const;

    // Setters
    void setLife(const double life);
    void takeDamage(const double damage);

    // Abstract methods
    virtual void onDeath() = 0;
};

#endif   // LIVING_ENTITY_HPP