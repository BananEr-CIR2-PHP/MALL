#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include "../../include/entity/mob.hpp"

#define MOBSINFO_FILE "../res/mob/mobs.json"

// --- CONSTRUCTORS/DESTRUCTORS ---

/**
 * Default constructor
 */
Mob::Mob() {
    initDefaultValues();
}

/**
 * Constructor
 * Construct mob from a Json object
 * 
 * @param mobObject Json object to construct the mob from
 */
Mob::Mob(const QJsonObject& mobObject, Player* target) {
    if (! loadFromJson(mobObject)) {
        initDefaultValues();
    }

    this->target = target;
}

/** Copy constructor
 * 
 * @param other Another Mob
 */
Mob::Mob(const Mob& other) : LivingEntity(other), damage(other.damage), target(other.target), lootTable(other.lootTable), scoreValue(other.scoreValue) { }

/**
 * Constructor
 * 
 * @param life Starting life of entity
 * @param damage Melee damage this mob deals
 * @param speed Speed of mob
 * @param position Starting position of entity
 * @param dimensions Collision box dimensions. Box is centered on position.
 * @param sprite A pointer to a sprite. Warning: given sprite should still be managed and deleted outside of this class.
 * @param team The team this entity belongs to
 * @param score The score value of this mob
 * @param lootTable Loot table to pick loots from
 * @param playerTarget The target of this mob
 */
Mob::Mob(const qreal life, const qreal damage, const qreal speed, const Vector2 position, const Vector2 dimensions, const QString& sprite, Teams::Team team, const qint64 score, const QString& lootTable, Player* playerTarget) :
    LivingEntity(life, speed, position, dimensions, sprite, team), damage(damage), scoreValue(score), lootTable(lootTable), target(playerTarget)
{

}

/**
 * Destructor
 */
Mob::~Mob() {
    delete loot;
}

/**
 * Copy mob on a new pointer
 * 
 * @return This mob, copied in a new pointer
 */
Mob* Mob::copy() const {
    return new Mob(*this);
}

// --- INHERITED METHODS ---

/**
 * Called on death of this entity
 */
void Mob::onDeath() {
    // Can't die multiple times
    if (! isDeleted) {
        setDeleted(true);
        delete loot;
        loot = getRandomLoot();
        if (loot->isEmpty()) {
            delete loot;
            loot = nullptr;
        }
    }
}

/**
 * Called when this Entity collides with another
 * 
 * @param other The entity this object collided with
 */
void Mob::onCollide(Entity* other) {
    // If colliding with a player, change target
    if (Player* player = dynamic_cast<Player*>(other)) {
        target = player;
    }
}

/**
 * Called once per frame
 * 
 * @param deltaTime Time elapsed since last frame, in milliseconds
 * @return Whether this entity wants to spawn another entity or not
 */
bool Mob::onUpdate(qint64 deltaTime) {
    moveTowardTarget(deltaTime);
    return LivingEntity::onUpdate(deltaTime) || loot;
}

/**
 * Get next Entity this entity wants to spawn
 * 
 * @return Pointer to the new entity. nullptr if no other entity to spawn.
 */
Entity* Mob::getSpawned() {
    if (loot) {
        Item* newItem = loot;
        loot = nullptr;
        return newItem;
    }
    else {
        return nullptr;
    }
}

/**
 * Get melee damage of this mob
 * 
 * @return Melee damage of this mob
 */
qreal Mob::getDamage() const {
    return damage;
}

/**
 * Get a random loot picked from the loot table
 * 
 * @return The looted item. Receiver becomes responsible of it.
 */
Item* Mob::getRandomLoot() const {
    return Item::create(
        LootTables::getRandomLoot(lootTable),
        getPos()
    );
}

/**
 * Get deletion state of this entity
 * True if it should be deleted within the game loop, false otherwise.
 * Note: deletion state only applies to game logic. Entity may need to be deleted even though deletion state is false.
 * 
 * @return Whether this entity should be deleted or not
 */
bool Mob::getDeleted() const {
    // Delay deletion: mob death timing may cause the item to fail looting
    return LivingEntity::getDeleted() && !loot;
}

/**
 * Get score value of this mob
 * 
 * @return The score value of this mob
 */
qint64 Mob::getScoreValue() const {
    return scoreValue;
}

/**
 * Set a new target for this mob.
 * Mobs always exclusively attack towards their target
 * 
 * @param newTarget The new target for this mob
 */
void Mob::setTarget(Player* newTarget) {
    target = newTarget;
}

/**
 * Set a new loot table for this mob
 * 
 * @param lootTable the new loot table
 */
void Mob::setLootTable(const QString& lootTable) {
    this->lootTable = lootTable;
}

/**
 * Set the score value of this mob
 * 
 * @param value The new score value
 */
void Mob::setScoreValue(const qint64 value) {
    scoreValue = value;
}

// --- Json construction ---

/**
 * Load a mob from the given Json object
 * 
 * @param mobObject A Json object representing a mob
 * @return True if succeeded, false otherwise
 */
bool Mob::loadFromJson(const QJsonObject& mobObject) {
    setLife(mobObject["life"].toDouble());
    damage = mobObject["damage"].toDouble();
    setSpeed(mobObject["speed"].toDouble());
    setDims(Vector2(mobObject["dims_X"].toDouble(), mobObject["dims_Y"].toDouble()));
    setSprite(mobObject["sprite"].toString());
    setScoreValue(mobObject["score"].toInteger());
    setLootTable(mobObject["loot_table"].toString());

    return true;
}

/**
 * Initialize default values for this mob
 */
void Mob::initDefaultValues() {
    damage = 0;
    target = nullptr;
    lootTable = "";
    scoreValue = 0;
}

/**
 * Static method. Load all mobs from the mobs json file
 * 
 * @param mobs Adds all mobs, ready to be copied, to this map
 */
void Mob::loadAllMobs(QMap<QString, Mob*>* mobs) {
    // Open file
    QFile file = QFile(MOBSINFO_FILE);      // Code reuse from gun.cpp
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file" << (MOBSINFO_FILE);
        return;    // Abort loading
    }

    // Parse JSON
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (doc.isNull()) {
        qWarning() << "Failed to parse JSON data.";
        return;    // Abort loading
    }

    // Load all mobs
    QJsonArray mobsArray = doc.object()["mobs"].toArray();
    for (qsizetype i=0; i<mobsArray.size(); i++) {
        QJsonObject mobObject = mobsArray[i].toObject();
        QString key = mobObject["name"].toString();
        mobs->insert(key, new Mob(mobsArray[i].toObject()));
    }
}

/**
 * Move the mob towards the target, if any
 * 
 * @param deltaTime Time elapsed since last frame, in milliseconds
 */
void Mob::moveTowardTarget(qint64 deltaTime) {
    if (target) {
        Vector2 movement = (target->getCenterPos() - getCenterPos()).normalized();
        movement = movement * getSpeedMultiplier() * getSpeed() * deltaTime;
        setPos(getPos() + movement);
    }
}