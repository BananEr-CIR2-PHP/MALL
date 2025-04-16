#ifndef GUN_HPP
#define GUN_HPP

#include "weapon.hpp"
#include "weaponType.hpp"
#include "../entity/missile.hpp"

class Gun : public Weapon {
protected:
    qreal bulletRange;
    qreal bulletDamage;
    bool bulletPierces;
    qreal bulletSpeed;
    Vector2 bulletDimensions;
    Sprites::SpriteImage bulletSprite;
    
    Gun(const Gun& other);

public:
    Gun(const WeaponType::GunType::GunType gunType = WeaponType::GunType::None);
    Gun(const WeaponType::RocketLauncherType::RocketLauncherType gunType);
    Gun(const qreal bulletRange, const qreal bulletDamage, const bool bulletPierces, const qreal bulletSpeed, const Vector2 bulletDimensions, const Sprites::SpriteImage bulletSprite = Sprites::SpriteImage::None, const Sprites::SpriteImage sprite = Sprites::SpriteImage::None);
    ~Gun();

    Weapon* clone() const override;
    Entity* attack(Vector2 position, Vector2 direction) override;
};

#endif   // GUN_HPP