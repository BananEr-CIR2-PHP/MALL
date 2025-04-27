#ifndef MOB_HPP
#define MOB_HPP

#include "livingEntity.hpp"
#include "player.hpp"

class Mob : public LivingEntity {
protected:
    qreal damage;
    Player* target;         // Safe, since players can't be deleted until end of scene

public:
    // Constructors/destructors
    Mob();
    Mob(const Mob& other);
    Mob(const qreal life, const qreal damage, const qreal speed, const Vector2 position, const Vector2 dimensions, Sprites::SpriteImage sprite = Sprites::SpriteImage::None, Teams::Team team = Teams::None, Player* playerTarget = nullptr);
    ~Mob();

    // Inherited methods
    virtual void onDeath();
    virtual void onCollide(Entity* other);
    virtual bool onUpdate(qint64 deltaTime);
    virtual Entity* getSpawned();

    qreal getDamage() const;
};

#endif   // MOB_HPP