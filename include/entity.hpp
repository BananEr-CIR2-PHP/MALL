#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "vector2.hpp"
#include "sprite.hpp"

class Entity {
protected:
    Vector2 position;
    const Sprite* sprite = nullptr;     // sprite object cannot be modified but pointer can
public:
    // Constructor/destructor
    Entity();
    Entity(const Entity& other);
    Entity(const Vector2 position, const Sprite* sprite=nullptr);
    ~Entity();

    // Getters
    Vector2 getPos() const;

    // Setters
    void setPos(const Vector2 pos);
};

#endif   // ENTITY_HPP