#include "../../include/weapon/rocketLauncher.hpp"
#include "../../include/entity/rocket.hpp"

/**
 * Constructor
 * 
 * @param weaponType Type of rocket launcher
 */
RocketLauncher::RocketLauncher(const WeaponType::RocketLauncherType::RocketLauncherType weaponType) : Gun(weaponType) {
    // TODO: fill with weapon details
    // TODO: weapon details in a separate file
    switch (weaponType) {
        case WeaponType::RocketLauncherType::Bazooka:
            rocketEffect = Effect(Effects::EffectType::Boom, 10000, 1500);
            effectRange = 30;
            break;

        default:
            rocketEffect = Effect();
            effectRange = 0;
            break;
    }
}

/**
 * Copy constructor
 * 
 * @param other another Rocket launcher
 */
RocketLauncher::RocketLauncher(const RocketLauncher& other) :
    Gun(other), rocketEffect(other.rocketEffect), effectRange(other.effectRange)
{

}

/**
 * Constructor
 * 
 * @param rocketEffect Effect spawned on rocket hit
 * @param effectRange Range of effect spawned on rocket hit
 * @param rocketRange Range of rocket (max travel distance)
 * @param rocketDamage Damage of rocket on direct hit
 * @param rocketPierces Whether if rocket explodes on first hit or not
 * @param rocketSpeed Speed of the rocket
 * @param rocketDimensions Dimensions of the rocket
 * @param rocketSprite Sprite of spawned rocket
 * @param sprite Sprite of weapon
 */
RocketLauncher::RocketLauncher(const Effect rocketEffect, const qreal effectRange, const qreal rocketRange, const qreal rocketDamage, const bool rocketPierces, const qreal rocketSpeed, const Vector2 rocketDimensions, const Sprites::SpriteImage rocketSprite, const Sprites::SpriteImage sprite) :
    Gun(rocketRange, rocketDamage, rocketPierces, rocketSpeed, rocketDimensions, rocketSprite, sprite), rocketEffect(rocketEffect), effectRange(effectRange)
{

}

/**
 * Constructor
 * 
 * @param rocketEffect Effect spawned on rocket hit
 * @param effectRange Range of effect spawned on rocket hit
 * @param rocketRange Range of rockets (max travel distance)
 * @param rocketSpeed Speed of the rockets
 * @param rocketDimensions Dimensions of the rockets
 * @param rocketSprite Sprite of spawned rockets
 * @param sprite Sprite of weapon
 */
RocketLauncher::RocketLauncher(const Effect rocketEffect, const qreal effectRange, const qreal rocketRange, const qreal rocketSpeed, const Vector2 rocketDimensions, const Sprites::SpriteImage rocketSprite, const Sprites::SpriteImage sprite) :
    Gun(rocketRange, 0, false, rocketSpeed, rocketDimensions, rocketSprite, sprite), rocketEffect(rocketEffect), effectRange(effectRange)
{

}

/**
 * Destructor
 */
RocketLauncher::~RocketLauncher() { }

/**
 * Clone this rocket launcher
 * 
 * @return Cloned rocket launcher (allocated using new keyword)
 */
Weapon* RocketLauncher::clone() const {
    return new RocketLauncher(*this);
}

/**
 * Use gun attack at given position, towards given direction
 * 
 * @param position Gun position
 * @param direction Gun pointing direction
 * @return Entity spawned during attack. nullptr if no Entity spawned
 */
void RocketLauncher::attack(Vector2 position, Vector2 direction, Teams::Team team) {
    Vector2 velocity = direction.normalized() * bulletSpeed;

    if (!bulletSpawn) {
        bulletSpawn = new Rocket(rocketEffect, effectRange, velocity, bulletRange, position, bulletDimensions, bulletSprite, team);
    }
}