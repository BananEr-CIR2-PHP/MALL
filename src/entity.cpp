#include "../include/entity.hpp"

// --- CONSTRUCTORS/DESTRUCTORS ---

/**
 * Default constructor
 */
Entity::Entity() {
    position = Vector2::zero;
    dimensions = Vector2::zero;
    sprite = new Sprite(Sprites::SpriteImage::None);
}

/**
 * Copy constructor
 * 
 * @param other The entity to copy
 */
Entity::Entity(const Entity& other) {
    position = other.position;
    dimensions = other.dimensions;
    sprite = new Sprite(*other.sprite);
}

/**
 * Constructor
 * 
 * @param position Starting position of entity
 * @param dimensions Collision box dimensions. Box is centered on position.
 * @param sprite Sprite image type
 */
Entity::Entity(const Vector2 position, const Vector2 dimensions, Sprites::SpriteImage sprite) : position(position), dimensions(dimensions) {
    this->sprite = new Sprite(sprite);
}

/**
 * Destructor
 */
Entity::~Entity() {
    delete sprite;
}


// --- GETTERS ---

/**
 * Get position of entity
 * 
 * @return Position of the entity
 */
Vector2 Entity::getPos() const {
    return position;
}

/**
 * Get dimensions of collision box.
 * Collision box is centered on entity position
 * 
 * @return Dimensions of the collision box
 */
Vector2 Entity::getDims() const {
    return dimensions;
}

/**
 * Get deletion state of this entity
 * True if it should be deleted within the game loop, false otherwise.
 * Note: deletion state only applies to game logic. Entity may need to be deleted even though deletion state is false.
 * 
 * @return Whether this entity should be deleted or not
 */
bool Entity::getDeleted() const {
    return isDeleted;
}

// --- SETTERS ---

/**
 * Set position of entity
 * 
 * @param pos New position of the entity
 */
void Entity::setPos(const Vector2 pos) {
    position = pos;
    QGraphicsItem::setPos(position.getX(), position.getY());
}

/**
 * Set dimensions of entity (bounding rect size)
 * 
 * @param dims New dimensions of the entity
 */
void Entity::setDims(const Vector2 dims) {
    if (dims.getX() >= 0 && dims.getY() >= 0) {
        dimensions = dims;
    }
    else {
        throw std::runtime_error("Dimensions cannot be negative!");
    }
}

/**
 * Set deletion state of this entity
 * Marking this entity as deleted informs the game loop that this entity should be deleted soon.
 * Use this instead of directly deleting it if you want to ensure this entity lives for the rest of current loop
 * 
 * @param del New deletion state
 */
void Entity::setDeleted(const bool del) {
    isDeleted = del;
}

// --- GRAPHICS METHODS ---

/**
 * Get boundingRect of this entity
 */
QRectF Entity::boundingRect() const {
    return QRectF(position.getX(), position.getY(), dimensions.getX(), dimensions.getY());
}

/**
 * Paint object on scene
 */
void Entity::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    // Draw sprite if it exists
    if (sprite != nullptr) {
        painter->drawImage(boundingRect(), *(sprite->getImage()));
    }
}