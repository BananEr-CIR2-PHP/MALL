#include "../../include/weapon/weapon.hpp"

/**
 * Default constructor
 */
Weapon::Weapon(Sprites::SpriteImage sprite) {
    this->sprite = new Sprite(sprite);
}

/**
 * Copy constructor
 * 
 * @param other Another weapon
 */
Weapon::Weapon(const Weapon& other) : sprite(other.sprite) {
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