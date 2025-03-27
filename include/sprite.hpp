#ifndef SPRITE_HPP
#define SPRITE_HPP

// TODO: this class entirely

class Sprite {
protected:
    double example = 56;    // Example attribute so the class stores at least a bit of data

public:
    // Constructor/destructor
    Sprite();
    Sprite(const Sprite& other);
    ~Sprite();

    void render() const;
};

#endif   // SPRITE_HPP