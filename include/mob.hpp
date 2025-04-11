#ifndef MOB_HPP
#define MOB_HPP

#include "livingEntity.hpp"

class Mob : public LivingEntity {    
public:
    // Constructors/destructors
    Mob();
    Mob(const Mob& other);
    Mob(qreal life, const Vector2 position, const Vector2 dimensions, Sprites::SpriteImage sprite = Sprites::SpriteImage::None);
    ~Mob();

    // Inherited methods
    virtual void onDeath();
    virtual void onCollide(Entity* other);
    virtual bool onUpdate(qint64 deltaTime);
    virtual Entity* getSpawned();
};

#endif   // MOB_HPP