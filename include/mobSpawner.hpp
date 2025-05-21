#ifndef MOBSPAWNER_HPP
#define MOBSPAWNER_HPP

#include <QtGlobal>
#include <QMap>
#include <QList>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "entity/mob.hpp"


class MobSpawner {
private:
    struct MobTrigger {
        qint64 trigger;
        QString mobName;
        qint64 amount;
    };

    QMap<QString, Mob*>* mobs = nullptr;            // Cache to spawn mobs from the name
    QList<MobTrigger>* spawnList = nullptr;        // Ordered list by trigger.
    qsizetype i_nextSpawn = 0;      // Index of next mob to spawn
    qsizetype i_mobAmount = 0;
    qreal spawnRange = 0;
    qint64 allWavesDuration = 0;
    qint64 substractSceneTime = 0;

    void createMobsCache();
    void createSpawnCache(const QString& filename);
    bool createSpawnList(const QJsonArray& array);
    void activateLoopReset();

public:
    MobSpawner();
    MobSpawner(const QString& filename);
    ~MobSpawner();

    Mob* getSpawned(qint64 sceneTime, Player* target = nullptr);
};

#endif   // MOBSPAWNER_HPP