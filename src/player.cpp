#include "../include/player.hpp"

// --- CONSTRUCTORS/DESTRUCTORS ---

/**
 * Default constructor
 */
Player::Player() {
    entityType = EntityType::Player;
}

/** Copy constructor
 * 
 * @param other Another Player
 */
Player::Player(const Player& other) : LivingEntity(other) {
    entityType = EntityType::Player;
}

/**
 * Constructor
 * 
 * @param life Starting life of entity
 * @param position Starting position of entity
 * @param dimensions Collision box dimensions. Box is centered on position.
 * @param sprite A pointer to a sprite. Warning: given sprite should still be managed and deleted outside of this class.
 */
Player::Player(double life, const Vector2 position, const Vector2 dimensions, const Sprite* sprite) : LivingEntity(life, position, dimensions, sprite) {
    entityType = EntityType::Player;
}

/**
 * Destructor
 */
Player::~Player() { }

void Player::gatherItem(Item::ItemType type) {
    std::cout << type << std::endl;
}

// --- INHERITED METHODS ---

/**
 * Called on death of this entity
 */
void Player::onDeath() { }

/**
 * Called when this Entity collides with another
 * 
 * @param other The entity this object collided with
 */
void Player::onCollide(Entity& other) { }

/**
 * Called once per frame
 * 
 * @param deltaTime Time elapsed since last frame, in milliseconds
 */
void Player::onUpdate(qint64 deltaTime) { }