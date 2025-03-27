#include "../include/player.hpp"

// --- CONSTRUCTORS/DESTRUCTORS ---

/**
 * Default constructor
 */
Player::Player() { }

/** Copy constructor
 * 
 * @param other Another Player
 */
Player::Player(const Player& other) : LivingEntity(other) { }

/**
 * Constructor
 * 
 * @param life Starting life of entity
 * @param position Starting position of entity
 * @param dimensions Collision box dimensions. Box is centered on position.
 * @param sprite A pointer to a sprite. Warning: given sprite should still be managed and deleted outside of this class.
 */
Player::Player(double life, const Vector2 position, const Vector2 dimensions, const Sprite* sprite) : LivingEntity(life, position, dimensions, sprite) { }

/**
 * Destructor
 */
Player::~Player() { }