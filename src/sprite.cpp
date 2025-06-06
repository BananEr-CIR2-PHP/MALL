#include <QtDebug>
#include "../include/sprite.hpp"

#define IMAGE_PATH "../res/img/"

// --- Constructor/destructor ---

/**
 * Constructor
 * 
 * @param img Image of the sprite
 */
Sprite::Sprite() {
    spritesCount += 1;

    if (spritesCache == nullptr) {
        initSpritesCache();
    }

    setImage("");
}

/**
 * Constructor
 * 
 * @param fileName File name of sprite (should look like: "foo.png")
 */
Sprite::Sprite(const QString& fileName) {
    spritesCount += 1;

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
 * Change the image texture to the given one
 * 
 * @param fileName Name of image file located in res/img (should look like "foo.png")
 */
void Sprite::setImage(const QString& fileName) {
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
    delete spritesCache;
    spritesCache = nullptr;
}