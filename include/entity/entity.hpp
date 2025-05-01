#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <QtGlobal>
#include <QPainter>
#include <QGraphicsItem>
#include <iostream>

#include "../vector2.hpp"
#include "../sprite.hpp"
#include "teams.hpp"

class Entity : public QGraphicsItem {
private:
    Vector2 position;
    Vector2 dimensions;

protected:
    const Sprite* sprite = nullptr;     // sprite object cannot be modified but pointer can
    bool isDeleted = false;     // Set to true when entity is deleted. Ensures entity exists until not needed anymore.
    Teams::Team team = Teams::None;

    Entity(const Entity& other);

public:
    // Constructor/destructor
    Entity();
    Entity(const Vector2 position, const Vector2 dimensions, Sprites::SpriteImage sprite=Sprites::SpriteImage::None, Teams::Team team = Teams::None);
    Entity(const Vector2 position, const Vector2 dimensions, const QString sprite = "", Teams::Team team = Teams::None);
    virtual ~Entity();

    // Getters
    Vector2 getPos() const;
    Vector2 getDims() const;
    bool getDeleted() const;
    Teams::Team getTeam() const;

    // Setters
    void setPos(const Vector2 pos);
    void setDims(const Vector2 dims);
    void setDeleted(const bool del);
    void setSprite(const QString& filename);

    // --- GRAPHICS METHODS ---
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    // Abstract methods
    virtual void onCollide(Entity* other) = 0;
    virtual bool onUpdate(qint64 deltaTime) = 0;
    virtual Entity* getSpawned() = 0;
};

#endif   // ENTITY_HPP