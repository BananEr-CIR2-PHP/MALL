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
    QString getEffectSprite(Effects::EffectType effectType);
    EffectZone(const EffectZone& other);

public:
    // Constructor/destructor
    EffectZone();
    EffectZone(Effect effect, const Vector2 position, const qreal range, const QString& sprite);
    EffectZone(Effect effect, const Vector2 position, const qreal range);
    ~EffectZone();

    // Inherited methods
    QPainterPath shape() const override;
    void onCollide(Entity* other, qint64 deltaTime) override;
    bool onUpdate(qint64 deltaTime) override;
    Entity* getSpawned() override;

    // Methods related to effects
    void repelEntity(Entity* entity);
};

#endif   // EFFECTZONE_HPP