#include "../../include/entity/missile.hpp"
#include "../../include/entity/livingEntity.hpp"

// --- CONSTRUCTOR/DESTRUCTOR ---

/**
 * Default constructor
 */
Missile::Missile() {
    velocity = Vector2::zero;
    lifetime = 0;
    damage = 0;
    pierceEntities = false;
}

/**
 * Copy constructor
 * 
 * @param other The missile to copy
 */
Missile::Missile(const Missile& other) : Entity(other), velocity(other.velocity), lifetime(other.lifetime), damage(other.damage), pierceEntities(other.pierceEntities) { }

/**
 * Constructor
 * 
 * @param velocity Speed and direction this missile moves to
 * @param range Max distance to travel before despawn
 * @param damage Damage this missile deals when hitting a living entity
 * @param pierceEntities Whether this missile despawns on first entity hit or not
 * @param position Starting position of entity
 * @param dimensions Collision box dimensions. Box is centered on position.
 * @param sprite Sprite image type
 * @param team The team this entity belongs to
 */
Missile::Missile(const Vector2 velocity, const qreal range, const qreal damage, const bool pierceEntities, const Vector2 position, const Vector2 dimensions, Sprites::SpriteImage sprite, Teams::Team team) : 
    Entity(position, dimensions, sprite, team), velocity(velocity), lifetime(range), damage(damage), pierceEntities(pierceEntities) { }

/**
 * Constructor
 * 
 * @param velocity Speed and direction this missile moves to
 * @param range Max distance to travel before despawn
 * @param damage Damage this missile deals when hitting a living entity
 * @param pierceEntities Whether this missile despawns on first entity hit or not
 * @param position Starting position of entity
 * @param dimensions Collision box dimensions. Box is centered on position.
 * @param sprite Sprite image name (should look like "foo.png")
 * @param team The team this entity belongs to
 */
Missile::Missile(const Vector2 velocity, const qreal range, const qreal damage, const bool pierceEntities, const Vector2 position, const Vector2 dimensions, const QString sprite, Teams::Team team) :
    Entity(position, dimensions, sprite, team), velocity(velocity), lifetime(range), damage(damage), pierceEntities(pierceEntities)
{

}

/**
 * Destructor
 */
Missile::~Missile() { }

/**
 * Copy missile on a new pointer
 * 
 * @return This missile, copied in a new pointer
 */
Missile* Missile::copy() const {
    return new Missile(*this);
}

// --- GETTERS ---

/**
 * Get velocity of missile
 * 
 * @return Velocity of the missile
 */
Vector2 Missile::getSpeed() const {
    return velocity;
}

// --- SETTERS ---

/**
 * Set velocity of missile
 * 
 * @param speed Velocity of the missile
 */
void Missile::setSpeed(const Vector2 speed) {
    prepareGeometryChange();
    velocity = speed;
}

// --- INHERITED METHODS ---

/**
 * Called when this Entity collides with another
 * 
 * @param other The entity this object collided with
 */
void Missile::onCollide(Entity* other) {
    if (LivingEntity* entity = dynamic_cast<LivingEntity*>(other)) {
        if (entity->getTeam() != getTeam()) {
            entity->takeDamage(damage);

            // If does not pierce entities, delete this entity
            if (!pierceEntities) {
                setDeleted(true);
            }
        }
    }
}

/**
 * Called once per frame
 * 
 * @param deltaTime Time elapsed since last frame, in milliseconds
 * @return Whether this entity wants to spawn another entity or not
 */
bool Missile::onUpdate(qint64 deltaTime) {
    Vector2 travel = velocity*deltaTime;
    setPos(getPos() + travel);

    // Handle max missile travel distance
    lifetime -= travel.magnitude();
    if (lifetime < 0) {
        setDeleted(true);
    }
    return false;
}

/**
 * Get next Entity this entity wants to spawn
 * 
 * @return Pointer to the new entity. nullptr if no other entity to spawn.
 */
Entity* Missile::getSpawned() {
    return nullptr;
}

/**
 * Paint missile on scene
 * 
 * @param painter Painter to draw entity on
 */
void Missile::paint(QPainter *painter, const QStyleOptionGraphicsItem* styleOption, QWidget* widget) {
    // Draw sprite if it exists
    if (sprite != nullptr) {
        QSharedPointer<QImage> image = sprite->getImage();
        if (image != nullptr) {
            painter->save();

            // Tried to refactor code here, using getRotationTF() and painter->setTransform()
            // But it was buggy and was taking too long. Feel free to change if you know how to do.
            qreal angle = -velocity.angleWith(Vector2::right);
            QRectF originalRect = baseBoundingRect();
            painter->translate(originalRect.center());
            painter->rotate(angle);
            painter->translate(-originalRect.center());

            painter->drawImage(originalRect, *image);

            painter->restore();
        }
    }
}

QRectF Missile::boundingRect() const {
    QRectF originalRect = baseBoundingRect();
    QPointF rectCenter = originalRect.center();

    // Create a new transformation to apply a rotation
    QTransform tf = getRotationTF(rectCenter);

    // Find new corners of rect
    QPointF topLeft = tf.map(originalRect.topLeft());
    QPointF topRight = tf.map(originalRect.topRight());
    QPointF botLeft = tf.map(originalRect.bottomLeft());
    QPointF botRight = tf.map(originalRect.bottomRight());

    // We need to find smaller and bigger values on each coordinate (if inverted)
    qreal minX = qMin(qMin(topLeft.x(), topRight.x()), qMin(botLeft.x(), botRight.x()));
    qreal minY = qMin(qMin(topLeft.y(), topRight.y()), qMin(botLeft.y(), botRight.y()));
    qreal maxX = qMax(qMax(topLeft.x(), topRight.x()), qMax(botLeft.x(), botRight.x()));
    qreal maxY = qMax(qMax(topLeft.y(), topRight.y()), qMax(botLeft.y(), botRight.y()));

    return QRectF(minX, minY, maxX - minX, maxY - minY);
}

/**
 * Get base bounding rect of missile (before rotation)
 * 
 * @return Bounding rect, without taking rotation into account
 */
QRectF Missile::baseBoundingRect() const {
    Vector2 dims = getDims();
    return QRectF(0, 0, dims.getX(), dims.getY());
}

/**
 * Redefine shape of missile (rotated ellipse)
 * 
 * @return shape of the missile
 */
QPainterPath Missile::shape() const {
    QPainterPath path;

    QRectF originalRect = baseBoundingRect();
    QTransform tf = getRotationTF(originalRect.center());

    path.addEllipse(originalRect);
    path = tf.map(path);

    return path;
}

/**
 * Get rotation transform of this missile
 * 
 * @param rectCenter Center of rotation (usually center of bounding rect)
 * @return Rotation transform. Use transform.map() to apply.
 */
QTransform Missile::getRotationTF(QPointF rectCenter) const {
    // Rotation around center
    QTransform tf;
    tf.translate(rectCenter.x(), rectCenter.y());
    tf.rotate(-velocity.angleWith(Vector2::right));
    tf.translate(-rectCenter.x(), -rectCenter.y());
    return tf;
}