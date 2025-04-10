#ifndef MOB_HPP
#define MOB_HPP

#include "livingEntity.hpp"

class Mob : public LivingEntity {    
public:
    // Constructors/destructors
    Mob();
    Mob(const Mob& other);
    Mob(qreal life, const Vector2 position, const Vector2 dimensions, Sprite* sprite=nullptr);
    ~Mob();

    // Inherited methods
    virtual void onDeath();
    virtual void onCollide(Entity* other);
    virtual void onUpdate(qint64 deltaTime);
};

#endif   // MOB_HPP