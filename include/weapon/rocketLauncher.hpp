#ifndef ROCKETLAUNCHER_HPP
#define ROCKETLAUNCHER_HPP

#include "gun.hpp"
#include "../entity/effect.hpp"

class RocketLauncher : public Gun {
protected:
    Effect rocketEffect;
    qreal effectRange;

    RocketLauncher(const RocketLauncher& other);
    
public:
    RocketLauncher(const WeaponType::RocketLauncherType::RocketLauncherType weaponType = WeaponType::RocketLauncherType::None);
    RocketLauncher(const Effect rocketEffect, const qreal effectRange, const qreal rocketRange, const qreal rocketDamage, const bool rocketPierces, const qreal rocketSpeed, const Vector2 rocketDimensions, const Sprites::SpriteImage rocketSprite = Sprites::SpriteImage::None, const Sprites::SpriteImage sprite = Sprites::SpriteImage::None);
    RocketLauncher(const Effect rocketEffect, const qreal effectRange, const qreal rocketRange, const qreal rocketSpeed, const Vector2 rocketDimensions, const Sprites::SpriteImage rocketSprite = Sprites::SpriteImage::None, const Sprites::SpriteImage sprite = Sprites::SpriteImage::None);
    ~RocketLauncher();

    Weapon* clone() const override;
    void attack(Vector2 position, Vector2 direction) override;
};

#endif   // ROCKETLAUNCHER_HPP