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
 * Constructor
 * 
 * @param jsonObject Json object containing all rocket launcher infos
 */
RocketLauncher::RocketLauncher(const QJsonObject& jsonObject) {
    if (! loadFromJSON(jsonObject)) {
        qWarning() << "Failed to load Json rocket launcher object";
        initValuesDefault();
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
 * @param name Name of weapon
 * @param energyConsumption Energy this weapon consumes on use
 * @param delay Delay between two uses of this weapon (in milliseconds)
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
RocketLauncher::RocketLauncher(const QString name, const qint64 energyConsumption, const qint64 delay, const Effect rocketEffect, const qreal effectRange, const qreal rocketRange, const qreal rocketDamage, const bool rocketPierces, const qreal rocketSpeed, const Vector2 rocketDimensions, const QString rocketSprite, Vector2 dimensions, const Sprites::SpriteImage sprite) :
    Gun(name, energyConsumption, delay, rocketRange, rocketDamage, rocketPierces, rocketSpeed, rocketDimensions, rocketSprite, dimensions, sprite), rocketEffect(rocketEffect), effectRange(effectRange)
{

}

/**
 * Constructor
 * 
 * @param name Name of weapon
 * @param energyConsumption Energy this weapon consumes on use
 * @param delay Delay between two uses of this weapon (in milliseconds)
 * @param rocketEffect Effect spawned on rocket hit
 * @param effectRange Range of effect spawned on rocket hit
 * @param rocketRange Range of rockets (max travel distance)
 * @param rocketSpeed Speed of the rockets
 * @param rocketDimensions Dimensions of the rockets
 * @param rocketSprite Sprite of spawned rockets
 * @param dimensions Dimensions of weapon
 * @param sprite Sprite of weapon
 */
RocketLauncher::RocketLauncher(const QString name, const qint64 energyConsumption, const qint64 delay, const Effect rocketEffect, const qreal effectRange, const qreal rocketRange, const qreal rocketSpeed, const Vector2 rocketDimensions, const QString rocketSprite, Vector2 dimensions, const Sprites::SpriteImage sprite) :
    Gun(name, energyConsumption, delay, rocketRange, 0, false, rocketSpeed, rocketDimensions, rocketSprite, dimensions, sprite), rocketEffect(rocketEffect), effectRange(effectRange)
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

    return loadFromJSON(doc.object());
}

/**
 * Load rocket launcher informations from JSON object
 * 
 * @param jsonObject JSON object to load info from
 * @return True if succeeded, false otherwise
 */
bool RocketLauncher::loadFromJSON(const QJsonObject& jsonObject) {
    // Load attributes
    name = jsonObject["name"].toString();
    energyConsumption = jsonObject["energy_consumption"].toInteger();
    delay = jsonObject["delay"].toInteger();
    bulletRange = jsonObject["bullet_range"].toDouble();
    bulletDamage = jsonObject["bullet_damage"].toDouble();
    bulletPierces = jsonObject["bullet_pierces"].toBool();
    bulletSpeed = jsonObject["bullet_speed"].toDouble();
    bulletDimensions = Vector2(jsonObject["bullet_dims_X"].toDouble(), jsonObject["bullet_dims_Y"].toDouble());
    bulletSprite = jsonObject["bullet_sprite"].toString();
    setSprite(jsonObject["sprite"].toString());
    dimensions = Vector2(jsonObject["dims_X"].toDouble(), jsonObject["dims_Y"].toDouble());
    rocketEffect = Effect(
        jsonObject["effect_type"].toString(),
        jsonObject["effect_strength"].toDouble(),
        jsonObject["effect_duration"].toInteger()
    );
    effectRange = jsonObject["effect_range"].toDouble();

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