#ifndef ITEM_HPP
#define ITEM_HPP

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QPainter>
#include "entity.hpp"
#include "../weapon/weapon.hpp"

namespace ItemType {
    enum ItemType {
        None,
        Gold,
        HPPotion,
        EnergyPotion,
        Weapon
    };
}

class Item : public Entity {
private:
    const qreal nameVerticalSpace = 17;
    static QMap<QString, Item*>* itemsCache;
    static inline unsigned int itemsCount = 0;

    bool isInCache = false;
    bool showName = false;
    bool touchingPlayer = false;
    bool isNameRectSet = false;
    QRectF nameRect;
    QString name = "";
    QString weaponTable = "";   // Used by weapons in cache

    void loadDefaultValues();

protected:
    ItemType::ItemType itemType = ItemType::None;
    qint64 itemStrength = 0;
    Weapon* itemWeapon = nullptr;
    QString getName() const;
    
public:
    // Constructor/destructor
    Item(bool belongsToCache = false);
    Item(const Item& other);
    virtual Item* copy() const;
    Item(const QJsonObject& jsonItem, bool belongsToCache = false);
    Item(const Vector2 position, const Vector2 dimensions, ItemType::ItemType itemType, const QString& sprite="", const QString& itemName = "", const qint64 itemStrength=0, bool belongsToCache = false);
    ~Item();
    static Item* create(const QString& itemName, Vector2 position);

    // Inherited methods
    virtual void onCollide(Entity* other);
    virtual bool onUpdate(qint64 deltaTime);
    virtual Entity* getSpawned();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    // Getters
    ItemType::ItemType getType() const;
    bool hasWeapon() const;
    Weapon* takeWeapon();
    qint64 getStrength() const;
    bool isEmpty() const;

    // Setters
    void setWeapon(Weapon* newWeapon);
    void setType(const QString& typeName);

    static void deleteCache();
    static void generateCache();
};

inline QMap<QString, Item*>* Item::itemsCache = nullptr;

#endif   // ITEM_HPP