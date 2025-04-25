#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <iostream>
#include <QtGlobal>

// Vector2 is immutable
class Vector2 {
private:
    qreal x=0;
    qreal y=0;

public:
    // Public constants
    static const Vector2 zero;
    static const Vector2 up;
    static const Vector2 down;
    static const Vector2 left;
    static const Vector2 right;

    // Constructors
    Vector2();
    Vector2(qreal x, qreal y);
    Vector2(const Vector2& other);

    // Destructor
    ~Vector2();

    // Getters
    qreal getX() const;
    qreal getY() const;

    // Operators
    Vector2 operator+(const Vector2& other) const;
    Vector2 operator-(const Vector2& other) const;
    Vector2 operator*(qreal scalar) const;
    Vector2 operator/(qreal scalar) const;

    // Operation on magnitude
    qreal magnitude() const;
    qreal sqrMagnitude() const;
    Vector2 normalized() const;

    // Dot product / Vector product
    qreal dot(const Vector2& other) const;
    qreal cross(const Vector2& other) const;

    // Comparisons
    bool operator==(const Vector2& other) const;
    bool operator!=(const Vector2& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Vector2& v);    // From chatGPT

    // Other
    Vector2 rotate(qreal angle) const;
    Vector2 projectOnto(const Vector2& other) const;
    Vector2 minimum(Vector2 other) const;
    Vector2 maximum(Vector2 other) const;
};

#endif   // VECTOR2_HPP