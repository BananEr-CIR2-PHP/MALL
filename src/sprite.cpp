#include "../include/sprite.hpp"
#include <iostream>

// --- Constructor/destructor ---

/**
 * Constructor
 * 
 * @param img Image of the sprite
 */
Sprite::Sprite(QImage* img) : image(img) { }

/**
 * Copy constructor
 * 
 * @param other Another sprite
 */
Sprite::Sprite(const Sprite& other) {
    this->image = other.image;
}

/**
 * Destructor
 */
Sprite::~Sprite() { }

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
void Sprite::setImage(QSharedPointer<QImage> img) {
    image = img;
}