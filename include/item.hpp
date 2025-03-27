#ifndef ITEM_HPP
#define ITEM_HPP

#include "entity.hpp"


class Item : public Entity {
protected:
    /* data */
public:
    // Constructor/destructor
    Item();
    Item(const Item& other);
    Item(const Vector2 position, const Vector2 dimensions, const Sprite* sprite = nullptr);
    ~Item();

    virtual void onCollide(Entity& other);
};

#endif   // ITEM_HPP