#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "vector2.hpp"

class Entity {
private:
    Vector2 position;
public:
    // Constructor/destructor
    Entity();
    Entity(const Entity& other);
    Entity(const Vector2 position);
    ~Entity();

    // Getters
    Vector2 getPos() const;

    // Setters
    void setPos(const Vector2 pos);
};

#endif   // ENTITY_HPP