#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <QtGlobal>
#include <QPainter>
#include <QGraphicsItem>
#include <iostream>

#include "vector2.hpp"
#include "sprite.hpp"

class Entity : public QGraphicsItem {
public:
    enum class EntityType {
        None,
        Item,
        Mob,
        Player,
        Missile,
        EffectZone
    };

private:
    Vector2 position;
    Vector2 dimensions;

protected:
    const Sprite* sprite = nullptr;     // sprite object cannot be modified but pointer can
    EntityType entityType = EntityType::None;

    Entity(const Entity& other);

public:
    // Constructor/destructor
    Entity();
    Entity(const Vector2 position, const Vector2 dimensions, Sprite* sprite=nullptr);
    ~Entity();

    // Getters
    Vector2 getPos() const;
    Vector2 getDims() const;
    EntityType getType() const;

    // Setters
    void setPos(const Vector2 pos);
    void setDims(const Vector2 dims);

    // --- GRAPHICS METHODS ---
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    // Abstract methods
    virtual void onCollide(Entity& other) = 0;
    virtual void onUpdate(qint64 deltaTime) = 0;
};

#endif   // ENTITY_HPP