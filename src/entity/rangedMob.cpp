#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include "../../include/entity/rangedMob.hpp"
#include "../../include/entity/rocket.hpp"

#define RANGEDMOBSINFO_FILE "../res/mob/ranged_mobs.json"

// --- CONSTRUCTOR / DESTRUCTOR ---

/**
 * Default constructor
 */
RangedMob::RangedMob() {
    initDefaultValues();
}

/**
 * Copy constructor
 * 
 * @param other Another ranged mob
 */
RangedMob::RangedMob(const RangedMob& other) : Mob(other), fireCooldown(other.fireCooldown), missileSpeed(other.missileSpeed),
    minShootingDistance(other.minShootingDistance), maxShootingDistance(other.maxShootingDistance), shootStateActive(other.shootStateActive)
{
    bulletCache = other.bulletCache->copy();
    bulletSpawn = nullptr;
    delay = 0;
}

/**
 * Constructor
 * Construct ranged mob from a Json object
 * 
 * @param mobObject Json object to construct the ranged mob from
 */
RangedMob::RangedMob(const QJsonObject& mobObject, Player* target) {
    if (! loadFromJson(mobObject)) {    // Code reuse from mob
        initDefaultValues();
    }

    this->target = target;
}

/**
 * Destructor
 */
RangedMob::~RangedMob() {
    delete bulletCache;
    delete bulletSpawn;
}

/**
 * Copy missile on a new pointer
 * 
 * @return This missile, copied in a new pointer
 */
Mob* RangedMob::copy() const {
    return new RangedMob(*this);
}

// --- JSON CONSTRUCTION ---

/**
 * Load a mob from the given Json object
 * 
 * @param mobObject A Json object representing a mob
 * @return True if succeeded, false otherwise
 */
bool RangedMob::loadFromJson(const QJsonObject& mobObject) {
    setLife(mobObject["life"].toDouble());  // Code reuse from mob
    damage = mobObject["damage"].toDouble();
    setSpeed(mobObject["speed"].toDouble());
    setDims(Vector2(mobObject["dims_X"].toDouble(), mobObject["dims_Y"].toDouble()));
    setSprite(mobObject["sprite"].toString());

    fireCooldown = mobObject["fire_cooldown"].toInteger();
    minShootingDistance = mobObject["min_shoot_distance"].toDouble();
    maxShootingDistance = mobObject["max_shoot_distance"].toDouble();
    team = Teams::Ennemy;

    // Load the bullet
    QString bulletType = mobObject["bullet_type"].toString();
    QJsonObject bullet = mobObject["bullet"].toObject();
    if (bulletType == "missile") {
        bulletCache = new Missile(
            Vector2::zero,
            bullet["range"].toDouble(),
            bullet["damage"].toDouble(),
            bullet["pierces"].toBool(),
            Vector2::zero,
            Vector2(bullet["dims_X"].toDouble(), bullet["dims_Y"].toDouble()),
            bullet["sprite"].toString(),
            Teams::Ennemy
        );
    }
    else if (bulletType == "rocket") {
        bulletCache = new Rocket(
            Effect(
                bullet["effect_type"].toString(),
                bullet["effect_strength"].toDouble(),
                bullet["effect_duration"].toInteger()
            ),
            bullet["effect_range"].toDouble(),
            Vector2::zero,
            bullet["range"].toDouble(),
            bullet["damage"].toDouble(),
            bullet["pierces"].toBool(),
            Vector2::zero,
            Vector2(bullet["dims_X"].toDouble(), bullet["dims_Y"].toDouble()),
            bullet["sprite"].toString(),
            Teams::Ennemy
        );
    }
    missileSpeed = bullet["speed"].toDouble();

    return true;
}

/**
 * Initialize default values for this mob
 */
void RangedMob::initDefaultValues() {
    fireCooldown = 0;
    missileSpeed = 0;
    minShootingDistance = 0;
    maxShootingDistance = 0;
    bulletCache = new Missile();
    bulletSpawn = nullptr;

    Mob::initDefaultValues();
}

/**
 * Static method. Load all mobs from the mobs json file
 * 
 * @return A map containing mobs ready to be copied
 */
void RangedMob::loadAllMobs(QMap<QString, Mob*>* mobs) {
    // Open file
    QFile file = QFile(RANGEDMOBSINFO_FILE);      // Code reuse from gun.cpp
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file" << (RANGEDMOBSINFO_FILE);
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
        mobs->insert(key, new RangedMob(mobsArray[i].toObject()));
    }
}

// -- INHERITED METHODS ---

/**
 * Called once per frame
 * 
 * @param deltaTime Time elapsed since last frame, in milliseconds
 * @return Whether this entity wants to spawn another entity or not
 */
bool RangedMob::onUpdate(qint64 deltaTime)  {
    if (delay > 0) {
        delay -= deltaTime;
    }

    if (target) {
        Vector2 centerPos = getCenterPos();
        qreal targetDistance = centerPos.distanceWith(target->getCenterPos());
        // Mob should get closer to player until reaching min shooting distance,
        // Then shooting continuously until leaving max shooting distance, and then get closer again...
        if (targetDistance > maxShootingDistance || (!shootStateActive && targetDistance > minShootingDistance)) {
            moveTowardTarget(deltaTime);
            shootStateActive = false;
        }
        else {
            shootStateActive = true;
            if (!bulletSpawn && delay <= 0) {
                // Shoot a bullet towards target
                delay = fireCooldown;
                bulletSpawn = bulletCache->copy();
                bulletSpawn->setPos(centerPos - bulletSpawn->getDims()/2);
                bulletSpawn->setSpeed((target->getCenterPos() - centerPos).normalized()*missileSpeed);
            }
        }
    }

    return bulletSpawn || LivingEntity::onUpdate(deltaTime);
}

/**
 * Get next Entity this entity wants to spawn
 * /!\ Important: Receiver of the Entity becomes responsible of it.
 * 
 * @return Pointer to the new entity. nullptr if no other entity to spawn.
 */
Entity* RangedMob::getSpawned() {
    Missile* newMissile = bulletSpawn;
    bulletSpawn = nullptr;
    return newMissile;
}