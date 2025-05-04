#include "../../include/entity/entity.hpp"

// --- CONSTRUCTORS/DESTRUCTORS ---

/**
 * Default constructor
 */
Entity::Entity() {
    setPos(Vector2::zero);
    dimensions = Vector2::zero;
    sprite = new Sprite(Sprites::SpriteImage::None);
    team = Teams::None;
}

/**
 * Copy constructor
 * 
 * @param other The entity to copy
 */
Entity::Entity(const Entity& other) {
    setPos(other.position);
    dimensions = other.dimensions;
    sprite = new Sprite(*other.sprite);
    team = other.team;
}

/**
 * Constructor
 * 
 * @param position Starting position of entity
 * @param dimensions Collision box dimensions. Box is centered on position.
 * @param sprite Sprite image type
 * @param team The team this entity belongs to
 */
Entity::Entity(const Vector2 position, const Vector2 dimensions, Sprites::SpriteImage sprite, Teams::Team team) : dimensions(dimensions), team(team) {
    setPos(position);
    this->sprite = new Sprite(sprite);
}

/**
 * Constructor
 * 
 * @param position Starting position of entity
 * @param dimensions Collision box dimensions. Box is centered on position.
 * @param sprite Sprite image name (should look like "foo.png")
 * @param team The team this entity belongs to
 */
Entity::Entity(const Vector2 position, const Vector2 dimensions, const QString sprite, Teams::Team team) : dimensions(dimensions), team(team) {
    setPos(position);
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
 * Get position of entity (position of top left corner)
 * 
 * @return Position of the entity
 */
Vector2 Entity::getPos() const {
    return position;
}

/**
 * Get position of the center of the entity
 * 
 * @return Position of center of the entity
 */
Vector2 Entity::getCenterPos() const {
    return position + dimensions/2;
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

/**
 * Get the team of this entity
 */
Teams::Team Entity::getTeam() const {
    return team;
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
        prepareGeometryChange();
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

/**
 * Set a new sprite for this entity
 * 
 * @param filename Filename of the new sprite (should look like "foo.png")
 */
void Entity::setSprite(const QString& filename) {
    delete sprite;
    sprite = new Sprite(filename);
}

// --- GRAPHICS METHODS ---

/**
 * Get boundingRect of this entity (relative to entity position)
 * 
 * @return bounding rect of this entity
 */
QRectF Entity::boundingRect() const {
    return QRectF(0, 0, dimensions.getX(), dimensions.getY());
}

/**
 * Paint object on scene
 * 
 * @param painter Painter to draw entity on
 */
void Entity::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    // Draw sprite if it exists
    if (sprite != nullptr) {
        QSharedPointer<QImage> image = sprite->getImage();
        if (image != nullptr) {
            painter->drawImage(boundingRect(), *image);
        }
    }
}