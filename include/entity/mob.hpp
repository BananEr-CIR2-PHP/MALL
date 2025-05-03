#ifndef MOB_HPP
#define MOB_HPP

#include <QJsonObject>

#include "livingEntity.hpp"
#include "player.hpp"
#include "../lootTables.hpp"

class Mob : public LivingEntity {
protected:
    qreal damage;
    Player* target;         // Safe, since players can't be deleted until end of scene
    QString lootTable;

    Item* loot = nullptr;

public:
    // Constructors/destructors
    Mob();
    Mob(const Mob& other);
    Mob(const QJsonObject& mobObject, Player* target = nullptr);
    Mob(const qreal life, const qreal damage, const qreal speed, const Vector2 position, const Vector2 dimensions, Sprites::SpriteImage sprite = Sprites::SpriteImage::None, Teams::Team team = Teams::None, const QString& lootTable = "", Player* playerTarget = nullptr);
    ~Mob();

    virtual Mob* copy() const;

    // Inherited methods
    void onDeath() override;
    void onCollide(Entity* other) override;
    bool onUpdate(qint64 deltaTime) override;
    Entity* getSpawned() override;

    qreal getDamage() const;
    Item* getRandomLoot() const;
    bool getDeleted() const override;
    void setTarget(Player* newTarget);
    void setLootTable(const QString& lootTable);

    // Json contruction
    bool loadFromJson(const QJsonObject& mobObject);
    void initDefaultValues();
    static void loadAllMobs(QMap<QString, Mob*>* mobs);

    void moveTowardTarget(qint64 deltaTime);
};

#endif   // MOB_HPP