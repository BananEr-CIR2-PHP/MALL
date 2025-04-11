#include "../include/sprite.hpp"

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
    if (spritesCache->contains(img)) {
        // If the image is in cache, simply take it from cache
        image = spritesCache->take(img);
    }
    else {
        // If the image is not in cache, create a new one and add it to cache
        if (img == Sprites::SpriteImage::None) {
            image = QSharedPointer<QImage>(nullptr);    // No image
        }
        else {
            image = QSharedPointer<QImage>(new QImage(spritesLoc->take(img)));
        }
        spritesCache->insert(img, image);
    }
}

// --- STATIC ---

/**
 * Generate images location map
 */
void Sprite::generateSpritesLoc() {
    spritesLoc = new QMap<Sprites::SpriteImage, QString>();
    spritesLoc->insert(Sprites::SpriteImage::BoomZone, "../res/img/boom.png");
    spritesLoc->insert(Sprites::SpriteImage::Coin, "../res/img/coin.png");
    spritesLoc->insert(Sprites::SpriteImage::Player, "../res/img/player.png");
}

/**
 * Initialize sprite cache
 */
void Sprite::initSpritesCache() {
    spritesCache = new QMap<Sprites::SpriteImage, QSharedPointer<QImage>>();
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