#ifndef ROCKETLAUNCHER_HPP
#define ROCKETLAUNCHER_HPP

#include "gun.hpp"
#include "../entity/effect.hpp"

class RocketLauncher : public Gun {
private:
    bool loadFromJSON(const QString& fileName);
    
protected:
    Effect rocketEffect;
    qreal effectRange;

    RocketLauncher(const RocketLauncher& other);
    void initValuesDefault();
    
public:
    RocketLauncher();
    RocketLauncher(const WeaponType::RocketLauncherType::RocketLauncherType weaponType);
    RocketLauncher(const QString name, const qint64 energyConsumption, const Effect rocketEffect, const qreal effectRange, const qreal rocketRange, const qreal rocketDamage, const bool rocketPierces, const qreal rocketSpeed, const Vector2 rocketDimensions, const QString rocketSprite = "", Vector2 dimensions = Vector2::zero, const Sprites::SpriteImage sprite = Sprites::SpriteImage::None);
    RocketLauncher(const QString name, const qint64 energyConsumption, const Effect rocketEffect, const qreal effectRange, const qreal rocketRange, const qreal rocketSpeed, const Vector2 rocketDimensions, const QString rocketSprite = "", Vector2 dimensions = Vector2::zero, const Sprites::SpriteImage sprite = Sprites::SpriteImage::None);
    ~RocketLauncher();

    Weapon* clone() const override;
    void attack(Vector2 position, Vector2 direction, Teams::Team team) override;
};

#endif   // ROCKETLAUNCHER_HPP