#include <QRandomGenerator>
#include "../include/mobSpawner.hpp"

#define SPAWNERINFO_PATH "../res/spawner/"

/**
 * Default constructor
 */
MobSpawner::MobSpawner() {
    createMobsCache();
    spawnList = new QList<MobTrigger>();
}

/**
 * Constructor
 * 
 * @param filename Name of spawner file (should look like "foo.json")
 */
MobSpawner::MobSpawner(const QString& filename) {
    createMobsCache();
    createSpawnCache(filename);
}

/**
 * Destructor
 */
MobSpawner::~MobSpawner() {
    qDeleteAll(*mobs);   // This function clears mobs, AND deletes all values. (this does not delete keys, but we don't to do that here)
    delete mobs;
    delete spawnList;
}

/**
 * Initialize the cache: mobs QMap
 */
void MobSpawner::createMobsCache() {
    mobs = Mob::loadAllMobs();
}

/**
 * Initialize the cache: spawnList Qlist
 * 
 * @param filename Name of Json file to load spawns from (should look like "foo.json")
 */
void MobSpawner::createSpawnCache(const QString& filename) {
    delete spawnList;   // Just in case of double calls
    spawnList = new QList<MobTrigger>();

    // Copied from gun.cpp
    // Open file
    QFile file = QFile(SPAWNERINFO_PATH + filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file" << (SPAWNERINFO_PATH + filename);
        return;
    }

    // Parse JSON
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (doc.isNull()) {
        qWarning() << "Failed to parse JSON data.";
        return;
    }

    spawnRange = doc.object()["spawn_radius"].toDouble();

    if (! createSpawnList(doc.object()["spawn"].toArray())) {
        delete spawnList;
        spawnList = new QList<MobTrigger>();
    }
}

/**
 * Create spawn list from the given json array.
 * Important: Json elements should be sorted by their trigger
 * 
 * @param array Spawn list Json array
 * @return Whether spawn list was successfully created or not
 */
bool MobSpawner::createSpawnList(const QJsonArray& array) {
    // For each element in array object ("spawn" array in Json)
    for (qsizetype i=0; i<array.size(); i++) {
        QJsonObject spawnFrame = array[i].toObject();
        QJsonArray frameSpawns = spawnFrame["spawn"].toArray();
        MobTrigger trig;

        // For each element in frame spawn array (also named "spawn" in Json)
        for (qsizetype i_mob=0; i_mob<frameSpawns.size(); i_mob++) {
            // Create trigger object
            QJsonObject mobSpawn = frameSpawns[i_mob].toObject();
            trig.trigger = spawnFrame["trigger"].toInteger();       // Trigger is inside of spawnFrame, not mobSpawn
            trig.mobName = mobSpawn["mob"].toString();
            trig.amount = mobSpawn["amount"].toInteger();

            // Add to list
            spawnList->append(trig);
        }
    }

    return true;
}

/**
 * Get the next mob to spawn. nullptr if no mob to spawn
 * 
 * @param sceneTime time passed since start of level
 * @return The next mob to spawn
 */
Mob* MobSpawner::getSpawned(qint64 sceneTime, Player* target) {
    if (i_nextSpawn < spawnList->size() && spawnList->at(i_nextSpawn).trigger < sceneTime) {
        
        MobTrigger mobTrig = spawnList->at(i_nextSpawn);
        QString mobName = mobTrig.mobName;

        // Update indexes
        i_mobAmount += 1;
        if (i_mobAmount >= mobTrig.amount) {
            i_mobAmount = 0;
            i_nextSpawn += 1;
        }

        // Create mob
        Mob* newMob = new Mob(*(mobs->value(mobName)));
        if (target) {
            qreal angle = QRandomGenerator64::global()->bounded(360.0);
            Vector2 direction = Vector2::right.rotate(angle);
            newMob->setPos(target->getPos() + direction*spawnRange);
            newMob->setTarget(target);
        }
        return newMob;
    }
    else {
        return nullptr;
    }
}