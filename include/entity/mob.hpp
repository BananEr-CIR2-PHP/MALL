#ifndef MOB_HPP
#define MOB_HPP

#include <QJsonObject>

#include "livingEntity.hpp"
#include "player.hpp"

class Mob : public LivingEntity {
protected:
    qreal damage;
    Player* target;         // Safe, since players can't be deleted until end of scene

public:
    // Constructors/destructors
    Mob();
    Mob(const Mob& other);
    Mob(const QJsonObject& mobObject, Player* target = nullptr);
    Mob(const qreal life, const qreal damage, const qreal speed, const Vector2 position, const Vector2 dimensions, Sprites::SpriteImage sprite = Sprites::SpriteImage::None, Teams::Team team = Teams::None, Player* playerTarget = nullptr);
    ~Mob();

    virtual Mob* copy() const;

    // Inherited methods
    void onDeath() override;
    void onCollide(Entity* other) override;
    bool onUpdate(qint64 deltaTime) override;
    Entity* getSpawned() override;

    qreal getDamage() const;
    void setTarget(Player* newTarget);

    // Json contruction
    bool loadFromJson(const QJsonObject& mobObject);
    void initDefaultValues();
    static void loadAllMobs(QMap<QString, Mob*>* mobs);

    void moveTowardTarget(qint64 deltaTime);
};

#endif   // MOB_HPP