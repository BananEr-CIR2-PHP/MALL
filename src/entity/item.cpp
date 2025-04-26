#include "../../include/entity/item.hpp"
#include "../../include/entity/player.hpp"

// --- CONSTRUCTOR/DESTRUCTOR ---

/**
 * Default constructor
 */
Item::Item() {
    itemType = ItemType::None;
}

/**
 * Copy constructor
 * 
 * @param other The entity to copy
 */
Item::Item(const Item& other) : Entity(other), itemType(other.itemType) {
    itemWeapon = other.itemWeapon->clone();
}

/**
 * Constructor
 * 
 * @param position Starting position of entity
 * @param dimensions Collision box dimensions. Box is centered on position.
 * @param itemType Type of item 
 * @param sprite A pointer to a sprite. Warning: given sprite should still be managed and deleted outside of this class.
 */
Item::Item(const Vector2 position, const Vector2 dimensions, ItemType::ItemType itemType, Sprites::SpriteImage sprite) :
    Entity(position, dimensions, sprite), itemType(itemType) 
{
    setName(itemType);
}

/**
 * Destructor
 */
Item::~Item() {
    delete itemWeapon;
}

// --- INHERITED METHODS ---

/**
 * Called when this Entity collides with another
 * 
 * @param other The entity this object collided with
 */
void Item::onCollide(Entity* other) {
    if (Player* player = dynamic_cast<Player*>(other)) {
        touchingPlayer = true;

        // Pickup item by Player is handled by Player class
    }
}

/**
 * Called once per frame
 * 
 * @param deltaTime Time elapsed since last frame, in milliseconds
 * @return Whether this entity wants to spawn another entity
 */
bool Item::onUpdate(qint64 deltaTime) {
    // When player touches item, display item name
    if (showName != touchingPlayer) {
        prepareGeometryChange();
    }
    showName = touchingPlayer;
    if (!touchingPlayer) {
        isNameRectSet = false;
    }
    touchingPlayer = false;     // Prepare next frame: touchingPlayer will stay the same if not touching the player
    return false;
}

/**
 * Get next Entity this entity wants to spawn
 * 
 * @return Pointer to the new entity. nullptr if no other entity to spawn.
 */
Entity* Item::getSpawned() {
    return nullptr;
}

/**
 * Paint item on scene
 * 
 * @param painter Painter to draw entity on
 */
void Item::paint(QPainter *painter, const QStyleOptionGraphicsItem* styleOption, QWidget* widget) {
    Vector2 dims = getDims();
    if (showName) {
        QString displayName = getName();
        if (displayName != "") {    // If has a name
            if (!isNameRectSet) {
                // If name rect not in cache, set it using text size
                prepareGeometryChange();
                QFontMetrics fontMetrics = QFontMetrics(painter->font());
                int textWidth = fontMetrics.horizontalAdvance(displayName);
                int textHeight = fontMetrics.height();
                nameRect = QRectF(-textWidth/2 + dims.getX()/2, -textHeight/2 - nameVerticalSpace, textWidth, textHeight);
                isNameRectSet = true;
            }

            painter->drawText(nameRect, Qt::AlignCenter, displayName);
        }   
    }

    // If item is a weapon, paint weapon sprite instead of item sprite
    if (getType() == ItemType::Weapon && itemWeapon) {
        if (const Sprite* sprite = itemWeapon->getSprite()) {
            QSharedPointer<QImage> image = sprite->getImage();
            if (image != nullptr) {
                painter->drawImage(QRectF(0, 0, dims.getX(), dims.getY()), *image);
            }
        }
    }
    else {
        Entity::paint(painter, styleOption, widget);
    }
}

/**
 * Get boundingRect of this item (relative to item position)
 * 
 * @return bounding rect of this entity
 */
QRectF Item::boundingRect() const {
    Vector2 dims = getDims();
    if (showName && isNameRectSet) {
        // Name is displayed above item
        // We get most top and most left corners, then most bottom and most right corners from text and item original rects
        Vector2 topLeft = Vector2(nameRect.topLeft()).minimum(Vector2::zero);
        Vector2 botRight = Vector2(nameRect.bottomRight()).maximum(dims);
        
        return QRectF(topLeft.getX(), topLeft.getY(), botRight.getX() - topLeft.getX(), botRight.getY() - topLeft.getY());
    }
    else {
        return QRectF(0, 0, dims.getX(), dims.getY());
    }
}

/**
 * Redefine shape of item, as item collision does not include name display
 * 
 * @return shape of the item
 */
QPainterPath Item::shape() const {
    QPainterPath path;
    Vector2 dims = getDims();
    path.addRect(QRectF(0, 0, dims.getX(), dims.getY()));
    return path;
}

// --- GETTERS ---

/**
 * Get the type of this item
 * 
 * @return Type of this item
 */
ItemType::ItemType Item::getType() const {
    return itemType;
}

/**
 * Know whether this item can give a weapon or not.
 * If true, this indicates that this item has a weapon it can give when calling giveWeapon()
 * 
 * @return Whether this item can give a weapon or not.
 */
bool Item::hasWeapon() const {
    return (itemWeapon!=nullptr);
}

/**
 * Get the weapon contained in this item.
 * The weapon becomes managed by the caller of this method.
 * 
 * @return The weapon contained in this item.
 */
Weapon* Item::takeWeapon() {
    prepareGeometryChange();
    Weapon* givenWeapon = itemWeapon;
    itemWeapon = nullptr;
    return givenWeapon;
}

/**
 * Get the name of the item
 * 
 * @return name of the item
 */
QString Item::getName() const {
    if (getType() == ItemType::ItemType::Weapon && itemWeapon) {
        return itemWeapon->getName();
    }
    else {
        return name;
    }
}

// --- SETTERS ---

/**
 * Set a new weapon for this item to contain.
 * If this item already contained a weapon, the previous weapon is deleted.
 * The given weapon becomes managed by this item.
 * 
 * @param newWeapon The new weapon to contain in this item
 */
void Item::setWeapon(Weapon* newWeapon) {
    if (getType() == ItemType::Weapon) {
        prepareGeometryChange();
        if (itemWeapon) {
            delete itemWeapon;
        }
        itemWeapon = newWeapon;
        setDims(itemWeapon->getDims());
    }
}

/**
 * Set the name of the weapon based on item type
 * 
 * @param itemType Type of item
 */
void Item::setName(ItemType::ItemType itemType) {
    switch (itemType) {
        case ItemType::HPPotion:
            name = "HP Potion";
            break;
        case ItemType::EnergyPotion:
            name = "Energy Potion";
            break;
        default:
            name = "";
            break;
    }
}