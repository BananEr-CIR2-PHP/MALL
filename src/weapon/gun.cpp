#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include "../../include/weapon/gun.hpp"

#define GUNINFO_PATH "../res/weapon/gun/"

// --- CONSTRUCTOR/DESTRUCTOR ---

/**
 * Default constructor
 */
Gun::Gun() {
    initValuesDefault();
}

/**
 * Constructor
 * 
 * @param gunType Type of gun
 */
Gun::Gun(const WeaponType::GunType::GunType gunType) {
    QString fileName;
    switch (gunType) {
        case WeaponType::GunType::DesertEagle:
            fileName = "desert_eagle.json";
            break;
        default:
            fileName = "";
    }

    if (fileName == "") {
        initValuesDefault();
    }
    else {
        if (! loadFromJSON(fileName)) {
            initValuesDefault();
        }
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
 * @param energyConsumption Energy this weapon consumes on use
 * @param bulletRange Range of the bullets the gun shoots
 * @param bulletDamage Damage of the bullets the gun shoots
 * @param bulletPierces Whether if the bullets despawn on first hit or not
 * @param bulletSpeed Speed of the bullets
 * @param bulletDimensions Dimensions of the bullets
 * @param bulletSprite Sprite of spawned bullets
 * @param dimensions Dimensions of weapon
 * @param sprite Sprite of weapon
 */
Gun::Gun(const qint64 energyConsumption, const qreal bulletRange, const qreal bulletDamage, const bool bulletPierces, const qreal bulletSpeed, const Vector2 bulletDimensions, const QString bulletSprite, Vector2 dimensions, const Sprites::SpriteImage sprite) :
    Weapon(energyConsumption, dimensions, sprite), bulletRange(bulletRange), bulletDamage(bulletDamage), bulletPierces(bulletPierces),
    bulletSpeed(bulletSpeed), bulletDimensions(bulletDimensions), bulletSprite(bulletSprite)
{

}

/**
 * Destructor
 */
Gun::~Gun() { }

/**
 * Initialize all attributes to default values
 * Typically called when failed to retrieve some values
 */
void Gun::initValuesDefault() {
    bulletRange = 0;
    bulletDamage = 0;
    bulletPierces = false;
    bulletSpeed = 0;
    bulletDimensions = Vector2::zero;
    bulletSprite = "";
    setSprite(Sprites::SpriteImage::None);
    dimensions = Vector2::zero;
}

/**
 * Load gun informations from JSON file
 * 
 * @param fileName Name of JSON file without path (should look like "foo.json")
 * @return True if succeeded, false otherwise
 */
bool Gun::loadFromJSON(const QString& fileName) {
    // Open file
    QFile file = QFile(GUNINFO_PATH + fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file" << (GUNINFO_PATH + fileName);
        return false;
    }

    // Parse JSON
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (doc.isNull()) {
        qWarning() << "Failed to parse JSON data.";
        return false;
    }

    // Load attributes
    QJsonObject rootObject = doc.object();
    // TODO: Weapon name?
    energyConsumption = rootObject["energy_consumption"].toInteger();
    bulletRange = rootObject["bullet_range"].toDouble();
    bulletDamage = rootObject["bullet_damage"].toDouble();
    bulletPierces = rootObject["bullet_pierces"].toBool();
    bulletSpeed = rootObject["bullet_speed"].toDouble();
    bulletDimensions = Vector2(rootObject["bullet_dims_X"].toDouble(), rootObject["bullet_dims_Y"].toDouble());
    bulletSprite = rootObject["bullet_sprite"].toString();
    setSprite(rootObject["sprite"].toString());
    dimensions = Vector2(rootObject["dims_X"].toDouble(), rootObject["dims_Y"].toDouble());

    return true;
}

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
 * @param position Gun muzzle position
 * @param direction Gun pointing direction
 * @return Entity spawned during attack. nullptr if no Entity spawned
 */
void Gun::attack(Vector2 position, Vector2 direction, Teams::Team team) {
    Vector2 velocity = direction.normalized() * bulletSpeed;

    // If no bullet is waiting for spawn
    if (!bulletSpawn) {
        Vector2 bulletPosition;
        if (direction.getX() >= 0) {
            bulletPosition = Vector2(position.getX(), position.getY() - bulletDimensions.getY()/2);
        }
        else {
            bulletPosition = Vector2(position.getX() - bulletDimensions.getX(), position.getY() - bulletDimensions.getY()/2);
        }
        bulletSpawn = new Missile(velocity, bulletRange, bulletDamage, bulletPierces, bulletPosition, bulletDimensions, bulletSprite, team);
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