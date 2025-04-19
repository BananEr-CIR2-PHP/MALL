#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "../sprite.hpp"
#include "../entity/entity.hpp"

class Weapon {
private:
    const Sprite* sprite = nullptr;     // Sprite object cannot be modified but pointer can
    
protected:
    Vector2 dimensions;
    qint64 energyConsumption;

    Weapon(const Weapon& other);
    void setSprite(Sprites::SpriteImage sprite = Sprites::SpriteImage::None);
    void setSprite(const QString spriteName = "");
    
public:
    Weapon(const qint64 energyConsumption = 0, const Vector2 dimensions = Vector2::zero, Sprites::SpriteImage sprite = Sprites::SpriteImage::None);
    virtual ~Weapon();

    virtual Weapon* clone() const = 0;
    virtual void attack(Vector2 position, Vector2 direction, Teams::Team team) = 0;
    virtual Entity* getSpawned() = 0;
    virtual bool wantSpawn() = 0;
    virtual void destroySpawned() = 0;

    const Sprite* getSprite() const;
    Vector2 getDims() const;
    qint64 getConsumption() const;
};

#endif   // WEAPON_HPP