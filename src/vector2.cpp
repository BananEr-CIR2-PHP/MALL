#include <cmath>
#include "../include/vector2.hpp"

#define EPSILON 0.0000001

// Constants
const Vector2 Vector2::zero(0, 0);
const Vector2 Vector2::up(0, 1);
const Vector2 Vector2::down(0, -1);
const Vector2 Vector2::right(1, 0);
const Vector2 Vector2::left(-1, 0);

// --- Constructors ---
/**
 * Default constructor
 */
Vector2::Vector2() {
    x = 0;
    y = 0;
}

/**
 * Constructor
 * 
 * @param x, y Coordinates of vector
 */
Vector2::Vector2(double x, double y) : x(x), y(y) {}

/**
 * Copy constructor
 * 
 * @param other The vector to copy
 */
Vector2::Vector2(const Vector2& other) {
    x = other.x;
    y = other.y;
}

/**
 * Destructor
 */
Vector2::~Vector2() {}

// --- Getters ---

/**
 * Get abscissa of vector
 * 
 * @return abscissa of the vector
 */
double Vector2::getX() const {
    return x;
}

/**
 * Get ordinate of vector
 * 
 * @return ordinate of the vector
 */
double Vector2::getY() const {
    return y;
}

// --- Operators ---

/**
 * Sum this vector with another
 * 
 * @param other The other vector
 * @return sum of the two vectors
 */
Vector2 Vector2::operator+(const Vector2& other) const {
    return Vector2(x + other.x, y + other.y);
}

/**
 * Substract this vector with another
 * 
 * @param other The other vector
 * @return This vector minus the other
 */
Vector2 Vector2::operator-(const Vector2& other) const {
    return Vector2(x - other.x, y - other.y);
}

/**
 * Multiplies this vector with a scalar
 * 
 * @param scalar Scalar to multiply this vector with
 * @return This vector multiplied with the scalar
 */
Vector2 Vector2::operator*(double scalar) const {
    return Vector2(x * scalar, y * scalar);
}

/**
 * Divides this vector with a scalar
 * 
 * @param scalar Scalar to divide this vector with
 * @return This vector divided with the scalar. Throws an error if scalar is zero
 */
Vector2 Vector2::operator/(double scalar) const {
    if (scalar == 0) {
        throw std::runtime_error("Erreur : Division par zéro !");
        return Vector2();
    }
    else {
        return Vector2(x/scalar, y/scalar);
    }
}

// --- Operations on magnitude ---

/**
 * Get the magnitude of this vector
 * 
 * @return magnitude of this vector
 */
double Vector2::magnitude() const {
    return std::sqrt(x*x + y*y);
}

/**
 * Get the squared magnitude of this vector (useful for optimizations)
 * 
 * @return squared magnitude of this vector
 */
double Vector2::sqrMagnitude() const {
    return x*x + y*y;
}

/**
 * Normalize a vector
 * 
 * @return this vector, normalized
 */
Vector2 Vector2::normalized() const {
    double mag = magnitude();
    if (mag == 0) {
        // If magnitude is zero, keep vector as it was before
        return *this;
    }
    else {
        return *this / mag;
    }
}

// --- Dot product / Vector product ---

/**
 * Dot product between this vector and another
 * 
 * @param other Another vector
 * @return dot product of this vector and the other
 */
double Vector2::dot(const Vector2& other) const {
    return x * other.x + y * other.y;
}

/**
 * 2D version of cross product
 * 
 * @param other Another vector
 * @return 2D "cross product"
 */
double Vector2::cross(const Vector2& other) const {
    return x * other.y - y * other.x;
}

// --- Comparisons ---

/**
 * Compare this vector and another
 * 
 * @param other Another vector
 * @return whether this vector is equal to the other or not
 */
bool Vector2::operator==(const Vector2& other) const {
    return std::abs(x - other.x) < EPSILON && std::abs(y - other.y);
}

/**
 * Compare this vector and another
 * 
 * @param other Another vector
 * @return whether this vector is different to the other or not
 */
bool Vector2::operator!=(const Vector2& other) const {
    return std::abs(x - other.x) > EPSILON || std::abs(y - other.y) > EPSILON;
}

/**
 * << operator override
 */
std::ostream& operator<<(std::ostream& os, const Vector2& v) {              // From ChatGPT
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

// --- Other ---

/**
 * Rotate this vector with given angle
 * 
 * @param angle Angle to rotate this vector with
 * @return Rotated vector
 */
Vector2 Vector2::rotate(double angle) const {
    double rad = angle * (M_PI / 180.0);        // Convert angle to rad
    double cosAngle = std::cos(rad);
    double sinAngle = std::sin(rad);
    return Vector2(x * cosAngle - y * sinAngle, x * sinAngle + y * cosAngle);
}

/**
 * Project this vector on another vector
 * 
 * @param other Another vector
 * @return Projected vector
 */
Vector2 Vector2::projectOnto(const Vector2& other) const {
    double scalar = dot(other) / other.sqrMagnitude();
    return other * scalar;
}