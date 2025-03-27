#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "livingEntity.hpp"
#include "itemType.hpp"

class Player : public LivingEntity {
protected:
    EntityType entityType = EntityType::Player;

public:
    // Constructors/destructors
    Player();
    Player(const Player& other);
    Player(double life, const Vector2 position, const Vector2 dimensions, const Sprite* sprite=nullptr);
    ~Player();
};

#endif   // PLAYER_HPP