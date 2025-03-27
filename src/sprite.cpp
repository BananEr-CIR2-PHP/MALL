#include <iostream>
#include "../include/sprite.hpp"

// TODO: this class entirely

// --- Constructor/destructor
Sprite::Sprite() {}
Sprite::Sprite(const Sprite& other) {}
Sprite::~Sprite() {}

void Sprite::render() const {
    std::cout << "Rendering sprite.";
}