#ifndef MOB_HPP
#define MOB_HPP

#include "livingEntity.hpp"

class Mob : public LivingEntity {
protected:
    EntityType entityType = EntityType::Mob;
    
public:
    // Constructors/destructors
    Mob();
    Mob(const Mob& other);
    Mob(double life, const Vector2 position, const Vector2 dimensions, const Sprite* sprite=nullptr);
    ~Mob();
};

#endif   // MOB_HPP