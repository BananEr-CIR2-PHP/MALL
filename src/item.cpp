#include "../include/item.hpp"


Item::Item() { }

Item::Item(const Item& other) { }

Item::Item(const Vector2 position, const Vector2 dimensions, const Sprite* sprite) : Entity(position, dimensions, sprite) {

}

Item::~Item() { }

void Item::onCollide(Entity& other) { }