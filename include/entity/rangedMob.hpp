#ifndef RANGED_MOB_HPP
#define RANGED_MOB_HPP

#include <QtGlobal>
#include "mob.hpp"
#include "missile.hpp"

class RangedMob : public Mob {
protected:
    qint64 fireCooldown;
    qreal missileSpeed = 0;
    qreal minShootingDistance;
    qreal maxShootingDistance;

    Missile* bulletCache;     // Easy bullet copy from this instance
    Missile* bulletSpawn = nullptr;     // Store a bullet to shoot soon
    qint64 delay = 0;
    bool shootStateActive = false;
    

public:
    RangedMob();
    RangedMob(const RangedMob& other);
    RangedMob(const QJsonObject& mobObject, Player* target = nullptr);
    ~RangedMob();

    Mob* copy() const override;

    // Inherited methods
    bool onUpdate(qint64 deltaTime) override;
    Entity* getSpawned() override;

    // Json construction
    bool loadFromJson(const QJsonObject& mobObject);
    void initDefaultValues();
    static void loadAllMobs(QMap<QString, Mob*>* mobs);
};

#endif   // RANGED_MOB_HPP