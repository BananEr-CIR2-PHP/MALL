#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <iostream>

// Vector2 is immutable
class Vector2 {
private:
    double x=0;
    double y=0;

public:
    // Constructors
    Vector2();
    Vector2(double x, double y);
    Vector2(const Vector2& other);

    // Destructor
    ~Vector2();

    // Getters
    double getX() const;
    double getY() const;

    // Operators
    Vector2 operator+(const Vector2& other) const;
    Vector2 operator-(const Vector2& other) const;
    Vector2 operator*(double scalar) const;
    Vector2 operator/(double scalar) const;

    // Operation on magnitude
    double magnitude() const;
    double sqrMagnitude() const;
    Vector2 normalized() const;

    // Dot product / Vector product
    double dot(const Vector2& other) const;
    double cross(const Vector2& other) const;

    // Comparisons
    bool operator==(const Vector2& other) const;
    bool operator!=(const Vector2& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Vector2& v);    // From chatGPT

    // Other
    Vector2 rotate(double angle) const;
    Vector2 projectOnto(const Vector2& other) const;
};

#endif   // VECTOR2_HPP