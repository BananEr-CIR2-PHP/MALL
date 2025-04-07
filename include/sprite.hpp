#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <QImage>
#include <QSharedPointer>

class Sprite {
protected:
    QSharedPointer<QImage> image;

    Sprite(const Sprite& other);

public:
    // Constructor/destructor
    Sprite(QImage* img = nullptr);
    ~Sprite();

    QSharedPointer<QImage> getImage() const;
    void setImage(QSharedPointer<QImage> img);
};

#endif   // SPRITE_HPP