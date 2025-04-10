#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <QImage>
#include <QSharedPointer>
#include <QString>
#include <QMap>

namespace Sprites {
    enum SpriteImage {
        None,
        Coin,
        Player
    };
}

class Sprite {
protected:
    static QMap<Sprites::SpriteImage, QString>* spritesLoc;
    static QMap<Sprites::SpriteImage, QSharedPointer<QImage>>* spritesCache;
    static inline unsigned int spritesCount = 0;   // Keep track of how much sprites exist

    QSharedPointer<QImage> image;

    static void generateSpritesLoc();
    static void initSpritesCache();

public:
    // Constructor/destructor
    Sprite(Sprites::SpriteImage img = Sprites::SpriteImage::None);
    Sprite(const Sprite& other);
    ~Sprite();

    QSharedPointer<QImage> getImage() const;
    void setImage(Sprites::SpriteImage img);

    static void deleteCachedSprites();
};

// Default static values
inline QMap<Sprites::SpriteImage, QString>* Sprite::spritesLoc = nullptr;
inline QMap<Sprites::SpriteImage, QSharedPointer<QImage>>* Sprite::spritesCache = nullptr;


#endif   // SPRITE_HPP