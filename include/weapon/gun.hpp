#ifndef GUN_HPP
#define GUN_HPP

#include "weapon.hpp"
#include "weaponType.hpp"
#include "../entity/missile.hpp"

class Gun : public Weapon {
private:
    bool loadFromJSON(const QString& fileName);

protected:
    qreal bulletRange;
    qreal bulletDamage;
    bool bulletPierces;
    qreal bulletSpeed;
    Vector2 bulletDimensions;
    QString bulletSprite;

    Missile* bulletSpawn = nullptr;
    
    Gun(const Gun& other);
    void initValuesDefault();

public:
    Gun();
    Gun(const WeaponType::GunType::GunType gunType);
    Gun(const QString name, const qint64 energyConsumption, const qint64 delay, const qreal bulletRange, const qreal bulletDamage, const bool bulletPierces, const qreal bulletSpeed, const Vector2 bulletDimensions, const QString bulletSprite = "", Vector2 dimensions = Vector2::zero, const Sprites::SpriteImage sprite = Sprites::SpriteImage::None);
    ~Gun();

    Weapon* clone() const override;
    void attack(Vector2 position, Vector2 direction, Teams::Team team) override;
    Entity* getSpawned() override;
    void destroySpawned() override;
    bool wantSpawn() override;
};

#endif   // GUN_HPP