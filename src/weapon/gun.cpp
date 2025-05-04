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
 * Constructor
 * 
 * @param jsonGun Json object containing all gun infos
 */
Gun::Gun(const QJsonObject& jsonGun) {
    if (! loadFromJSON(jsonGun)) {
        qWarning() << "Failed to load gun json infos";
        initValuesDefault();
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
 * @param name Name of weapon
 * @param energyConsumption Energy this weapon consumes on use
 * @param delay Delay between two uses of this weapon (in milliseconds)
 * @param bulletRange Range of the bullets the gun shoots
 * @param bulletDamage Damage of the bullets the gun shoots
 * @param bulletPierces Whether if the bullets despawn on first hit or not
 * @param bulletSpeed Speed of the bullets
 * @param bulletDimensions Dimensions of the bullets
 * @param bulletSprite Sprite of spawned bullets
 * @param dimensions Dimensions of weapon
 * @param sprite Sprite of weapon
 */
Gun::Gun(const QString name, const qint64 energyConsumption, const qint64 delay, const qreal bulletRange, const qreal bulletDamage, const bool bulletPierces, const qreal bulletSpeed, const Vector2 bulletDimensions, const QString bulletSprite, Vector2 dimensions, const Sprites::SpriteImage sprite) :
    Weapon(name, energyConsumption, delay, dimensions, sprite), bulletRange(bulletRange), bulletDamage(bulletDamage), bulletPierces(bulletPierces),
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
    name = "";
    bulletRange = 0;
    bulletDamage = 0;
    bulletPierces = false;
    bulletSpeed = 0;
    bulletDimensions = Vector2::zero;
    bulletSprite = "";
    setSprite(Sprites::SpriteImage::None);
    dimensions = Vector2::zero;
    energyConsumption = 0;
    delay = 1;
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
    return loadFromJSON(doc.object());
}

/**
 * Load gun informations from JSON object
 * 
 * @param jsonGun JSON object to load info from
 * @return True if succeeded, false otherwise
 */
bool Gun::loadFromJSON(const QJsonObject& jsonGun) {
    name = jsonGun["name"].toString();
    energyConsumption = jsonGun["energy_consumption"].toInteger();
    delay = jsonGun["delay"].toInteger();
    bulletRange = jsonGun["bullet_range"].toDouble();
    bulletDamage = jsonGun["bullet_damage"].toDouble();
    bulletPierces = jsonGun["bullet_pierces"].toBool();
    bulletSpeed = jsonGun["bullet_speed"].toDouble();
    bulletDimensions = Vector2(jsonGun["bullet_dims_X"].toDouble(), jsonGun["bullet_dims_Y"].toDouble());
    bulletSprite = jsonGun["bullet_sprite"].toString();
    setSprite(jsonGun["sprite"].toString());
    dimensions = Vector2(jsonGun["dims_X"].toDouble(), jsonGun["dims_Y"].toDouble());

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