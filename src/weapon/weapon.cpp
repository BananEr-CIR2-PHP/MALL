#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include "../../include/weapon/weapon.hpp"
#include "../../include/weapon/gun.hpp"
#include "../../include/weapon/rocketLauncher.hpp"

#define WEAPONINFO_PATH "../res/weapon/"

/**
 * Default constructor
 * 
 * @param name Name of weapon
 * @param energyConsumption Energy this weapon consumes on use
 * @param delay Delay between two uses of this weapon (in milliseconds)
 * @param sprite Sprite of weapon
 * @param dimensions Dimensions of weapon
 */
Weapon::Weapon(const QString& name, const qint64 energyConsumption, const qint64 delay, const Vector2 dimensions, const QString& sprite) {
    this->name = name;
    this->energyConsumption = energyConsumption;
    this->delay = (delay > 0) ? delay : 1;
    this->dimensions = dimensions;
    this->sprite = new Sprite(sprite);
}

/**
 * Copy constructor
 * 
 * @param other Another weapon
 */
Weapon::Weapon(const Weapon& other) : dimensions(other.dimensions), energyConsumption(other.energyConsumption), delay(other.delay), name(other.name) {
    this->sprite = new Sprite(*other.sprite);
}

/**
 * Destructor
 */
Weapon::~Weapon() {
    delete sprite;
}

/**
 * Pattern factory.
 * 
 * @param filename Weapon json file name
 * @return A new weapon. nullptr if failed.
 */
Weapon* Weapon::create(const QString& filename) {
    // Open file
    QFile file = QFile(WEAPONINFO_PATH + filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file" << (WEAPONINFO_PATH + filename);
        return nullptr;
    }

    // Parse JSON
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (doc.isNull()) {
        qWarning() << "Failed to parse JSON data.";
        return nullptr;
    }

    QJsonObject obj = doc.object();
    QString type = obj["type"].toString();
    // Create weapon depending on type
    if (type == "Gun") {
        return new Gun(obj);
    }
    else if (type == "RocketLauncher") {
        return new RocketLauncher(obj);
    }
    else {
        return nullptr;
    }
}

// --- METHODS ---

/**
 * Set a new sprite for the weapon
 * 
 * @param spriteName Name of sprite file (should look like "foo.png")
 */
void Weapon::setSprite(const QString& spriteName) {
    this->sprite = new Sprite(spriteName);
}

/**
 * Get the sprite of this weapon
 * 
 * @return Sprite of this weapon
 */
const Sprite* Weapon::getSprite() const {
    return sprite;
}

/**
 * Get the dimensions of the sprite of this weapon
 * 
 * @return dimensions of the sprite of this weapon
 */
Vector2 Weapon::getDims() const {
    return dimensions;
}

/**
 * Get energy consumption of this weapon
 * 
 * @return Energy consumption
 */
qint64 Weapon::getConsumption() const {
    return energyConsumption;
}

/**
 * Get the name of weapon
 * 
 * @return Name of weapon
 */
QString Weapon::getName() const {
    return name;
}

/**
 * Get the delay of this weapon / amount of time between 2 attacks
 * 
 * @return Delay of weapon
 */
qint64 Weapon::getDelay() const {
    return delay;
}

/**
 * Know whether this weapon object is empty or not.
 * Useful to know if weapon has successfully loaded or not
 * 
 * @return Whether this weapon is empty or not
 */
bool Weapon::isEmpty() const {
    return name == "";
}