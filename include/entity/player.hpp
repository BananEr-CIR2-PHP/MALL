#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <QKeyEvent>
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
    bool leftKeyPressed = false;
    bool rightKeyPressed = false;
    bool upKeyPressed = false;
    bool downKeyPressed = false;
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
    void changeActiveWeapon();

    // Inherited methods
    virtual void onDeath();
    virtual void onCollide(Entity* other);
    virtual bool onUpdate(qint64 deltaTime);
    virtual Entity* getSpawned();

    // Input events
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    void focusOutEvent(QFocusEvent *event);
};

#endif   // PLAYER_HPP