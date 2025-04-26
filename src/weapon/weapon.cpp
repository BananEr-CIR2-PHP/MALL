#include "../../include/weapon/weapon.hpp"

/**
 * Default constructor
 * 
 * @param name Name of weapon
 * @param energyConsumption Energy this weapon consumes on use
 * @param sprite Sprite of weapon
 * @param dimensions Dimensions of weapon
 */
Weapon::Weapon(const QString name, const qint64 energyConsumption, const Vector2 dimensions, Sprites::SpriteImage sprite) {
    this->name = name;
    this->energyConsumption = energyConsumption;
    this->dimensions = dimensions;
    this->sprite = new Sprite(sprite);
}

/**
 * Copy constructor
 * 
 * @param other Another weapon
 */
Weapon::Weapon(const Weapon& other) {
    this->dimensions = other.dimensions;
    this->sprite = new Sprite(*other.sprite);
}

/**
 * Destructor
 */
Weapon::~Weapon() {
    delete sprite;
}

// --- METHODS ---

/**
 * Set a new sprite for the weapon
 * 
 * @param sprite The new weapon sprite
 */
void Weapon::setSprite(Sprites::SpriteImage sprite) {
    this->sprite = new Sprite(sprite);
}

/**
 * Set a new sprite for the weapon
 * 
 * @param spriteName Name of sprite file (should look like "foo.png")
 */
void Weapon::setSprite(const QString spriteName) {
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