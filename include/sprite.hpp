#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <QImage>
#include <QSharedPointer>
#include <QString>
#include <QMap>

class Sprite {
protected:
    static QMap<QString, QSharedPointer<QImage>>* spritesCache;
    static inline unsigned int spritesCount = 0;   // Keep track of how much sprites exist

    QSharedPointer<QImage> image;

    static void initSpritesCache();

public:
    // Constructor/destructor
    Sprite();
    Sprite(const QString& fileName);
    Sprite(const Sprite& other);
    ~Sprite();

    QSharedPointer<QImage> getImage() const;
    void setImage(const QString& fileName);

    static void deleteCachedSprites();
};

// Default static values
inline QMap<QString, QSharedPointer<QImage>>* Sprite::spritesCache = nullptr;


#endif   // SPRITE_HPP