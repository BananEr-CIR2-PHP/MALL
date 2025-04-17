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

    Missile* bulletSpawn = nullptr;
    
    Gun(const Gun& other);
    Gun(const WeaponType::RocketLauncherType::RocketLauncherType gunType);

public:
    Gun(const WeaponType::GunType::GunType gunType = WeaponType::GunType::None);
    Gun(const qreal bulletRange, const qreal bulletDamage, const bool bulletPierces, const qreal bulletSpeed, const Vector2 bulletDimensions, const Sprites::SpriteImage bulletSprite = Sprites::SpriteImage::None, const Sprites::SpriteImage sprite = Sprites::SpriteImage::None);
    ~Gun();

    Weapon* clone() const override;
    void attack(Vector2 position, Vector2 direction, Teams::Team team) override;
    Entity* getSpawned() override;
    void destroySpawned() override;
    bool wantSpawn() override;
};

#endif   // GUN_HPP