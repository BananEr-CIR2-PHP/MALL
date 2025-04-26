#ifndef ITEM_HPP
#define ITEM_HPP

#include "entity.hpp"
#include "../weapon/weapon.hpp"
#include <QPainter>

namespace ItemType {
    enum ItemType {
        None,
        Gold,
        HPPotion,
        EnergyPotion,
        Weapon
    };
}

class Item : public Entity {
private:
    const qreal nameVerticalSpace = 17;

    bool showName = false;
    bool touchingPlayer = false;
    bool isNameRectSet = false;
    QRectF nameRect;
    QString name = "";

protected:
    ItemType::ItemType itemType = ItemType::None;
    Weapon* itemWeapon = nullptr;
    QString getName() const;
    
public:
    // Constructor/destructor
    Item();
    Item(const Item& other);
    Item(const Vector2 position, const Vector2 dimensions, ItemType::ItemType itemType, Sprites::SpriteImage sprite=Sprites::SpriteImage::None);
    ~Item();

    // Inherited methods
    virtual void onCollide(Entity* other);
    virtual bool onUpdate(qint64 deltaTime);
    virtual Entity* getSpawned();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    // Getters
    ItemType::ItemType getType() const;
    bool hasWeapon() const;
    Weapon* takeWeapon();

    // Setters
    void setWeapon(Weapon* newWeapon);
    void setName(ItemType::ItemType itemType);
};

#endif   // ITEM_HPP