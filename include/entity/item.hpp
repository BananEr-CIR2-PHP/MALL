#ifndef ITEM_HPP
#define ITEM_HPP

#include "entity.hpp"
#include <QPainter>

class Item : public Entity {
public:
    enum ItemType {
        None,
        Gold,
        HPPotion
    };

protected:
    ItemType itemType = ItemType::None;
    
public:
    // Constructor/destructor
    Item();
    Item(const Item& other);
    Item(const Vector2 position, const Vector2 dimensions, Sprites::SpriteImage sprite=Sprites::SpriteImage::None);
    ~Item();

    // Inherited methods
    virtual void onCollide(Entity* other);
    virtual bool onUpdate(qint64 deltaTime);
    virtual Entity* getSpawned();
};

#endif   // ITEM_HPP