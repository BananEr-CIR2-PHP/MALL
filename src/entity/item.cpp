#include "../../include/entity/item.hpp"
#include "../../include/entity/player.hpp"

#define ITEMSINFO_FILE "../res/items.json"

// --- CONSTRUCTOR/DESTRUCTOR ---

/**
 * Default constructor
 * @param belongsToCache Whether this item belongs to cache or not
 */
Item::Item(bool belongsToCache) : isInCache(belongsToCache) {
    if (!belongsToCache) {
        itemsCount += 1;
    }
    loadDefaultValues();
}

/**
 * Copy constructor
 * 
 * @param other The entity to copy
 */
Item::Item(const Item& other) : Entity(other), itemType(other.itemType), itemStrength(other.itemStrength), name(other.name) {
    if (other.itemWeapon) {
        itemWeapon = other.itemWeapon->clone();
    }
    else {
        itemWeapon = nullptr;
    }
    isInCache = false;      // A copied item never belongs to cache
    itemsCount += 1;
}

/**
 * Copy self
 * 
 * @return self, copied
 */
Item* Item::copy() const {
    return new Item(*this);
}

/**
 * Constructor
 * 
 * @param position Starting position of entity
 * @param dimensions Collision box dimensions. Box is centered on position.
 * @param itemType Type of item 
 * @param sprite A pointer to a sprite. Warning: given sprite should still be managed and deleted outside of this class.
 * @param name Name of the item. Will be displayed when player collides with the item
 * @param strength Strength of the item. Effect depends on the item type
 * @param belongsToCache Whether this item belongs to cache or not
 */
Item::Item(const Vector2 position, const Vector2 dimensions, ItemType::ItemType itemType, Sprites::SpriteImage sprite, const QString& name, const qint64 strength, bool belongsToCache) :
    Entity(position, dimensions, sprite), itemType(itemType), itemStrength(strength), name(name), isInCache(belongsToCache)
{
    if (!belongsToCache) {
        itemsCount += 1;
    }
}

/**
 * Destructor
 */
Item::~Item() {
    delete itemWeapon;
    if (!isInCache) {
        itemsCount -= 1;
        if (itemsCount == 0) {
            deleteCache();
        }
    }
}

/**
 * Constructor. Build item based on a json object
 * @param jsonItem Json item that should contain following informations:
 * Only "type" if object is a weapon. You will need to call setWeapon() later. In other cases:
 * "name": Name of item
 * "type": Type of item
 * "strength": strength ofitem. Effect depends on the type of item
 * "sprite": Sprite image name of the item (should look like "foo.png")
 * "dims_Y" and "dims_Y": dimensions of the item
 * @param belongsToCache Whether this item belongs to cache or not
 */
Item::Item(const QJsonObject& jsonItem, bool belongsToCache) : isInCache(belongsToCache) {
    if (!belongsToCache) {
        itemsCount += 1;
    }
    setType(jsonItem["type"].toString());
    if (itemType == ItemType::Weapon) {
        name = "";
        itemStrength = 0;
    }
    else {
        name = jsonItem["name"].toString();
        itemStrength = jsonItem["strength"].toInteger();
        setSprite(jsonItem["sprite"].toString());
        setDims(Vector2(jsonItem["dims_X"].toDouble(), jsonItem["dims_Y"].toDouble()));
    }
}

/**
 * Pattern factory. Loads informations from the cache.
 * Calling this for the first time or after deleting the cache can be slow.
 * See generateCache() to prebuild the cache
 * 
 * @param itemName Name of the item.
 */
Item* Item::create(const QString& itemName, const Vector2 position) {
    if (itemsCache == nullptr) {
        generateCache();
    }
    Item* product;
    if (itemsCache->contains(itemName)) {
        product = itemsCache->value(itemName)->copy();
        product->setPos(position);
        return product;
    }
    else {
        qWarning() << "Item " << itemName << " is unknown";
        product = new Item();   // Default item
        return product;
    }
}

void Item::loadDefaultValues() {
    itemType = ItemType::None;
    itemStrength = 0;
    name = "";
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

    // Paint name if any and if item should show its name
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
        if (sprite != nullptr) {
            QSharedPointer<QImage> image = sprite->getImage();
            if (image != nullptr) {
                painter->drawImage(Entity::boundingRect(), *image);
            }
        }
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

/**
 * Get the strength of the item. Effect depends on the type of item.
 * e.g. can be the strength of a potion or the amount of gold
 * This contains any value that needs to be associated with the item
 * 
 * @return Strength of the item
 */
qint64 Item::getStrength() const {
    return itemStrength;
}

/**
 * Know if this item is empty or not.
 * 
 * @return Whether this item is empty or not
 */
bool Item::isEmpty() const {
    return itemType == ItemType::None;
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
 * Convert a string into an item type
 * 
 * @param typeName Type of item as a string
 */
void Item::setType(const QString& typeName) {
    if (typeName == "HP Potion") {
        itemType = ItemType::HPPotion;
    }
    else if (typeName == "Energy Potion") {
        itemType = ItemType::EnergyPotion;
    }
    else if (typeName == "Gold") {
        itemType = ItemType::Gold;
    }
    else if (typeName == "Weapon") {
        itemType = ItemType::Weapon;
    }
    else {
        itemType = ItemType::None;
    }
}

/**
 * Generate item cache
 */
void Item::generateCache() {
    itemsCache = new QMap<QString, Item*>();
    // Open file
    QFile file = QFile(ITEMSINFO_FILE);    // Copied from gun.cpp
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file" << (ITEMSINFO_FILE);
        return;
    }

    // Parse JSON
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (doc.isNull()) {
        qWarning() << "Failed to parse JSON data.";
        return;
    }
    // Load items
    QJsonArray jsonItems = doc.object()["items"].toArray();
    for (qsizetype i=0; i<jsonItems.size(); i++) {
        QJsonObject jsonObj = jsonItems[i].toObject();
        itemsCache->insert(jsonObj["name"].toString(), new Item(jsonObj, true));
    }
}

/**
 * Delete item cache
 */
void Item::deleteCache() {
    if (itemsCache) {
        qDeleteAll(*itemsCache);    // This function deletes all values, but not keys. Useful in this case.
    }
    delete itemsCache;
    itemsCache = nullptr;
}