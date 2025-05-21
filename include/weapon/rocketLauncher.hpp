#ifndef ROCKETLAUNCHER_HPP
#define ROCKETLAUNCHER_HPP

#include "gun.hpp"
#include "../entity/effect.hpp"

class RocketLauncher : public Gun {
private:
    bool loadFromJSON(const QString& fileName);
    bool loadFromJSON(const QJsonObject& jsonRocketLauncher);
    
protected:
    Effect rocketEffect;
    qreal effectRange;

    RocketLauncher(const RocketLauncher& other);
    void initValuesDefault();
    
public:
    RocketLauncher();
    RocketLauncher(const QJsonObject& jsonRocketLauncher);
    RocketLauncher(const QString& name, const qint64 energyConsumption, const qint64 delay, const Effect rocketEffect, const qreal effectRange, const qreal rocketRange, const qreal rocketDamage, const bool rocketPierces, const qreal rocketSpeed, const Vector2 rocketDimensions, const QString& rocketSprite = "", Vector2 dimensions = Vector2::zero, const QString& sprite = "");
    RocketLauncher(const QString& name, const qint64 energyConsumption, const qint64 delay, const Effect rocketEffect, const qreal effectRange, const qreal rocketRange, const qreal rocketSpeed, const Vector2 rocketDimensions, const QString& rocketSprite = "", Vector2 dimensions = Vector2::zero, const QString& sprite = "");
    ~RocketLauncher();

    Weapon* clone() const override;
    void attack(Vector2 position, Vector2 direction, Teams::Team team) override;
};

#endif   // ROCKETLAUNCHER_HPP