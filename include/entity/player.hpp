#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include "livingEntity.hpp"
#include "item.hpp"
#include "../weapon/weapon.hpp"

#define PLAYER_SPEED 0.05

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

    Weapon* weapon1 = nullptr;
    Weapon* weapon2 = nullptr;
    Weapon* droppedWeapon = nullptr;
    Inventory::WeaponSlot activeWeaponSlot = Inventory::WeaponSlot_1;
    
    void initFlags();
    bool grabWeapon(Weapon* weapon, Inventory::WeaponSlot slot);
    bool dropWeapon(Inventory::WeaponSlot slot);
    bool hasWeapon(Inventory::WeaponSlot slot) const;

public:
    // Constructors/destructors
    Player();
    Player(const Player& other);
    Player(qreal life, const Vector2 position, const Vector2 dimensions, Sprites::SpriteImage sprite = Sprites::SpriteImage::None);
    ~Player();

    // Methods
    bool gatherItem(Item* item);

    // Inherited methods
    virtual void onDeath();
    virtual void onCollide(Entity* other);
    virtual bool onUpdate(qint64 deltaTime);
    virtual Entity* getSpawned();

    // Player actions. Actions are reactions to input events
    void actionUseWeapon(Vector2 direction);
    void actionSetLeftMovement(qreal mvt);
    void actionSetRightMovement(qreal mvt);
    void actionSetUpMovement(qreal mvt);
    void actionSetDownMovement(qreal mvt);
    void actionSetGrabPress(bool isGrabbing);
    void actionChangeWeapon();
};

#endif   // PLAYER_HPP