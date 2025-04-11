#ifndef EFFECTZONE_HPP
#define EFFECTZONE_HPP

#include "entity.hpp"
#include "effectType.hpp"
#include "effect.hpp"

class EffectZone : public Entity {
private:
    qreal range;
    Effect effect;

protected:
    EffectZone(const EffectZone& other);

public:
    // Constructor/destructor
    EffectZone();
    EffectZone(Effect effect, const Vector2 position, const qreal range, Sprites::SpriteImage sprite=Sprites::SpriteImage::None);
    ~EffectZone();

    // Inherited methods
    virtual void onCollide(Entity* other);
    virtual bool onUpdate(qint64 deltaTime);
    virtual Entity* getSpawned();

    // Methods related to effects
    void repelEntity(Entity* entity);
};

#endif   // EFFECTZONE_HPP