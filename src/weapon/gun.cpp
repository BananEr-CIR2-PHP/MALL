#include "../../include/weapon/gun.hpp"

// --- CONSTRUCTOR/DESTRUCTOR ---

/**
 * Constructor
 * 
 * @param gunType Type of gun
 */
Gun::Gun(const WeaponType::GunType::GunType gunType) {
    // TODO: fill with weapon details
    // TODO: weapon details in a separate file
    switch (gunType) {
        case WeaponType::GunType::DesertEagle:
            bulletRange = 600;
            bulletDamage = 1;
            bulletPierces = false;
            bulletSpeed = 0.2;
            bulletDimensions = Vector2(15, 50);
            bulletSprite = Sprites::SpriteImage::Coin;
            setSprite(Sprites::SpriteImage::None);
            break;

        default:
            bulletRange = 0;
            bulletDamage = 0;
            bulletPierces = false;
            bulletSpeed = 0;
            bulletDimensions = Vector2::zero;
            bulletSprite = Sprites::SpriteImage::None;
            setSprite(Sprites::SpriteImage::None);
            break;
    }
}

/**
 * Constructor
 * 
 * @param gunType type of gun
 */
Gun::Gun(const WeaponType::RocketLauncherType::RocketLauncherType gunType) {
    // TODO: fill with weapon details
    // TODO: weapon details in a separate file
    switch (gunType) {
        case WeaponType::RocketLauncherType::Bazooka:
            bulletRange = 400;
            bulletDamage = 0;
            bulletPierces = false;
            bulletSpeed = 0.1;
            bulletDimensions = Vector2(30, 30);
            bulletSprite = Sprites::SpriteImage::None;
            setSprite(Sprites::SpriteImage::None);
            break;

        default:
            bulletRange = 0;
            bulletDamage = 0;
            bulletPierces = false;
            bulletSpeed = 0;
            bulletDimensions = Vector2::zero;
            bulletSprite = Sprites::SpriteImage::None;
            setSprite(Sprites::SpriteImage::None);
            break;
    }
}

/**
 * Copy contructor
 * 
 * @param other Another gun
 */
Gun::Gun(const Gun& other) :
    Weapon(other), bulletRange(other.bulletRange), bulletDamage(other.bulletDamage), bulletPierces(other.bulletPierces),
    bulletSpeed(other.bulletSpeed), bulletDimensions(other.bulletDimensions), bulletSprite(other.bulletSprite)
{

}

/**
 * Constructor
 * 
 * @param bulletRange Range of the bullets the gun shoots
 * @param bulletDamage Damage of the bullets the gun shoots
 * @param bulletPierces Whether if the bullets despawn on first hit or not
 * @param bulletSpeed Speed of the bullets
 * @param bulletDimensions Dimensions of the bullets
 * @param bulletSprite Sprite of spawned bullets
 * @param sprite Sprite of weapon
 */
Gun::Gun(const qreal bulletRange, const qreal bulletDamage, const bool bulletPierces, const qreal bulletSpeed, const Vector2 bulletDimensions, const Sprites::SpriteImage bulletSprite, const Sprites::SpriteImage sprite) :
    Weapon(sprite), bulletRange(bulletRange), bulletDamage(bulletDamage), bulletPierces(bulletPierces),
    bulletSpeed(bulletSpeed), bulletDimensions(bulletDimensions), bulletSprite(bulletSprite)
{

}

/**
 * Destructor
 */
Gun::~Gun() { }

// --- INHERITED METHODS ---

/**
 * Clone this gun
 * 
 * @return Cloned gun (allocated using new keyword)
 */
Weapon* Gun::clone() const {
    return new Gun(*this);
}

/**
 * Use gun attack at given position, towards given direction
 * 
 * @param position Gun position
 * @param direction Gun pointing direction
 * @return Entity spawned during attack. nullptr if no Entity spawned
 */
void Gun::attack(Vector2 position, Vector2 direction) {
    Vector2 velocity = direction.normalized() * bulletSpeed;

    // If no bullet is waiting for spawn
    if (!bulletSpawn) {
        bulletSpawn = new Missile(velocity, bulletRange, bulletDamage, bulletPierces, position, bulletDimensions, bulletSprite);
    }
}

/**
 * Know whether if this gun wants to spawn an entity or not
 * 
 * @return True if it wants to spawn an entity, false otherwise
 */
bool Gun::wantSpawn() {
    return bulletSpawn;
}

/**
 * Get an entity that this gun wants to spawn, if any
 * 
 * @return Spawned entity, nullptr if no entity to spawn
 */
Entity* Gun::getSpawned() {
    Missile* newMissile = bulletSpawn;
    bulletSpawn = nullptr;
    return newMissile;
}

/**
 * If this gun wants to spawn an entity, destroy it.
 * Useful when gun changes state, and can no longer spawn an object for a while
 */
void Gun::destroySpawned() {
    delete bulletSpawn;
    bulletSpawn = nullptr;
}