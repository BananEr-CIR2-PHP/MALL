#include <QtDebug>
#include "../include/sprite.hpp"

#define IMAGE_PATH "../res/img/"

// --- Constructor/destructor ---

/**
 * Constructor
 * 
 * @param img Image of the sprite
 */
Sprite::Sprite(Sprites::SpriteImage img) {
    spritesCount += 1;

    if (spritesLoc == nullptr) {
        generateSpritesLoc();
    }
    if (spritesCache == nullptr) {
        initSpritesCache();
    }

    setImage(img);
}

/**
 * Constructor
 * 
 * @param fileName File name of sprite (should look like: "foo.png")
 */
Sprite::Sprite(QString fileName) {
    spritesCount += 1;

    if (spritesLoc == nullptr) {
        generateSpritesLoc();
    }
    if (spritesCache == nullptr) {
        initSpritesCache();
    }

    setImage(fileName);
}

/**
 * Copy constructor
 * 
 * @param other Another sprite
 */
Sprite::Sprite(const Sprite& other) {
    spritesCount += 1;
    this->image = other.image;
}

/**
 * Destructor
 */
Sprite::~Sprite() {
    spritesCount -= 1;

    if (spritesCount == 0) {
        deleteCachedSprites();
    }
}

/**
 * Get a shared pointer to the texture image
 * 
 * @return A shared pointer to the image
 */
QSharedPointer<QImage> Sprite::getImage() const {
    return image;
}

/**
 * Set the image texture to the given one
 * 
 * @param img The new image texture
 */
void Sprite::setImage(Sprites::SpriteImage img) {
    if (spritesLoc->contains(img)) {
        QString fileName = spritesLoc->value(img);
        setImage(fileName);
    }
    else {
        qWarning() << "Image location not found";
        image = spritesCache->value("");
    }
}

/**
 * Change the image texture to the given one
 * 
 * @param fileName Name of image file located in res/img (should look like "foo.png")
 */
void Sprite::setImage(QString fileName) {
    // If the image is in cache, simply take it from cache
    if (spritesCache->contains(fileName)) {
        image = spritesCache->value(fileName);
    }
    else {
        // If the image is not in cache, create a new one and add it to cache
        image = QSharedPointer<QImage>(new QImage(IMAGE_PATH + fileName));
        spritesCache->insert(fileName, image);
    }
}

// --- STATIC ---

/**
 * Generate images location map
 */
void Sprite::generateSpritesLoc() {
    spritesLoc = new QMap<Sprites::SpriteImage, QString>();
    spritesLoc->insert(Sprites::SpriteImage::None, "");
    spritesLoc->insert(Sprites::SpriteImage::BoomZone, "boom.png");
    spritesLoc->insert(Sprites::SpriteImage::Coin, "coin.png");
    spritesLoc->insert(Sprites::SpriteImage::Player, "player.png");
}

/**
 * Initialize sprite cache
 */
void Sprite::initSpritesCache() {
    spritesCache = new QMap<QString, QSharedPointer<QImage>>();
    spritesCache->insert("", QSharedPointer<QImage>(nullptr));
}

/**
 * Delete cache of images.
 */
void Sprite::deleteCachedSprites() {
    delete spritesLoc;
    spritesLoc = nullptr;
    delete spritesCache;
    spritesCache = nullptr;
}