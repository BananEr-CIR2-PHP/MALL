#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include "livingEntity.hpp"
#include "item.hpp"
#include "../weapon/weapon.hpp"

namespace Inventory {
    enum WeaponSlot {
        WeaponSlot_1,
        WeaponSlot_2
    };
}

class Player : public LivingEntity {
private:
    qreal leftKeyPressed = 0;
    qreal rightKeyPressed = 0;
    qreal upKeyPressed = 0;
    qreal downKeyPressed = 0;
    bool grabKeyPressed = false;
    bool useWeaponKeyPressed = false;
    Vector2 targetDir = Vector2::right;

    Weapon* weapon1 = nullptr;
    Weapon* weapon2 = nullptr;
    Weapon* droppedWeapon = nullptr;
    Inventory::WeaponSlot activeWeaponSlot = Inventory::WeaponSlot_1;

    qint64 weaponDelay = 0;

    qint64 maxEnergy;
    qint64 energy;

    qint64 gold;

    bool dropWeapon(Inventory::WeaponSlot slot);
    bool hasWeapon(Inventory::WeaponSlot slot) const;
    Weapon* getActiveWeapon() const;

public:
    static constexpr qreal DefaultSpeed = 0.1;
    static constexpr qreal DefaultLife = 100;
    static constexpr qint64 DefaultEnergy = 100;

    // Constructors/destructors
    Player();
    Player(const Player& other);
    Player(const qreal life, const qint64 energy, const qint64 gold, const qreal speed, const Vector2 position, const Vector2 dimensions, const QString& sprite = "", Teams::Team team = Teams::None);
    ~Player();

    // Methods
    bool gatherItem(Item* item);
    bool grabWeapon(Weapon* weapon, Inventory::WeaponSlot slot);

    // Getters/Setters
    qint64 getEnergy() const;
    qint64 getMaxEnergy() const;
    qint64 getGold() const;

    void setEnergy(const qint64 newEnergy);
    void consumeEnergy(const qint64 consumedEnergy);
    void setMaxEnergy(const qint64 newMaxEnergy);
    void addGold(const qint64 amount);

    // Inherited methods
    void onDeath() override;
    void onCollide(Entity* other) override;
    bool onUpdate(qint64 deltaTime) override;
    Entity* getSpawned() override;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    QPainterPath shape() const override;

    // Player actions. Actions are reactions to input events
    void actionUseWeapon(Vector2 direction);
    void actionSetLeftMovement(qreal mvt);
    void actionSetRightMovement(qreal mvt);
    void actionSetUpMovement(qreal mvt);
    void actionSetDownMovement(qreal mvt);
    void actionSetGrabPress(bool isGrabbing);
    void actionSetUsingWeapon(const bool isUsingWeapon);
    void actionSetTargetDirection(const Vector2 direction);
    void actionChangeWeapon();
};

#endif   // PLAYER_HPP