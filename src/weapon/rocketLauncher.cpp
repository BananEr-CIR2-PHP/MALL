#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include "../../include/weapon/rocketLauncher.hpp"
#include "../../include/entity/rocket.hpp"

#define ROCKETLAUNCHER_INFO_PATH "../res/weapon/rocket_launcher/"

// --- CONSTRUCTOR / DESTRUCTOR ---

/**
 * Default constructor
 */
RocketLauncher::RocketLauncher() {
    initValuesDefault();
}

/**
 * Constructor
 * 
 * @param weaponType Type of rocket launcher
 */
RocketLauncher::RocketLauncher(const WeaponType::RocketLauncherType::RocketLauncherType weaponType) {
    QString fileName;
    switch (weaponType) {
        case WeaponType::RocketLauncherType::Bazooka:
            fileName = "bazooka.json";
            break;

        default:
            fileName = "";
            break;
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
 * @param energyConsumption Energy this weapon consumes on use
 * @param rocketEffect Effect spawned on rocket hit
 * @param effectRange Range of effect spawned on rocket hit
 * @param rocketRange Range of rocket (max travel distance)
 * @param rocketDamage Damage of rocket on direct hit
 * @param rocketPierces Whether if rocket explodes on first hit or not
 * @param rocketSpeed Speed of the rocket
 * @param rocketDimensions Dimensions of the rocket
 * @param rocketSprite Sprite of spawned rocket
 * @param dimensions Dimensions of weapon
 * @param sprite Sprite of weapon
 */
RocketLauncher::RocketLauncher(const qint64 energyConsumption, const Effect rocketEffect, const qreal effectRange, const qreal rocketRange, const qreal rocketDamage, const bool rocketPierces, const qreal rocketSpeed, const Vector2 rocketDimensions, const QString rocketSprite, Vector2 dimensions, const Sprites::SpriteImage sprite) :
    Gun(energyConsumption, rocketRange, rocketDamage, rocketPierces, rocketSpeed, rocketDimensions, rocketSprite, dimensions, sprite), rocketEffect(rocketEffect), effectRange(effectRange)
{

}

/**
 * Constructor
 * 
 * @param energyConsumption Energy this weapon consumes on use
 * @param rocketEffect Effect spawned on rocket hit
 * @param effectRange Range of effect spawned on rocket hit
 * @param rocketRange Range of rockets (max travel distance)
 * @param rocketSpeed Speed of the rockets
 * @param rocketDimensions Dimensions of the rockets
 * @param rocketSprite Sprite of spawned rockets
 * @param dimensions Dimensions of weapon
 * @param sprite Sprite of weapon
 */
RocketLauncher::RocketLauncher(const qint64 energyConsumption, const Effect rocketEffect, const qreal effectRange, const qreal rocketRange, const qreal rocketSpeed, const Vector2 rocketDimensions, const QString rocketSprite, Vector2 dimensions, const Sprites::SpriteImage sprite) :
    Gun(energyConsumption, rocketRange, 0, false, rocketSpeed, rocketDimensions, rocketSprite, dimensions, sprite), rocketEffect(rocketEffect), effectRange(effectRange)
{

}

/**
 * Destructor
 */
RocketLauncher::~RocketLauncher() { }

/**
 * Initialize all attributes to default values
 * Typically called when failed to retrieve some values
 */
void RocketLauncher::initValuesDefault() {
    rocketEffect = Effect();
    effectRange = 0;
    Gun::initValuesDefault();
}

/**
 * Load gun informations from JSON file
 * 
 * @param fileName Name of JSON file without path (should look like "foo.json")
 * @return True if succeeded, false otherwise
 */
bool RocketLauncher::loadFromJSON(const QString& fileName) {
    // Open file
    QFile file = QFile(ROCKETLAUNCHER_INFO_PATH + fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file" << (ROCKETLAUNCHER_INFO_PATH + fileName);
        qDebug() << "Error:" << file.errorString();
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
    rocketEffect = Effect(
        rootObject["effect_type"].toString(),
        rootObject["effect_strength"].toDouble(),
        rootObject["effect_duration"].toInteger()
    );
    effectRange = rootObject["effect_range"].toDouble();

    return true;
}

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
 * @param position Gun muzzle position
 * @param direction Gun pointing direction
 * @return Entity spawned during attack. nullptr if no Entity spawned
 */
void RocketLauncher::attack(Vector2 position, Vector2 direction, Teams::Team team) {
    Vector2 velocity = direction.normalized() * bulletSpeed;

    if (!bulletSpawn) {
        Vector2 bulletPosition;
        if (direction.getX() >= 0) {
            bulletPosition = Vector2(position.getX(), position.getY() - bulletDimensions.getY()/2);
        }
        else {
            bulletPosition = Vector2(position.getX() - bulletDimensions.getX(), position.getY() - bulletDimensions.getY()/2);
        }
        bulletSpawn = new Rocket(rocketEffect, effectRange, velocity, bulletRange, bulletPosition, bulletDimensions, bulletSprite, team);
    }
}