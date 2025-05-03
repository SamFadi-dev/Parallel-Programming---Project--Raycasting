#ifndef VECTOR_H
#define VECTOR_H

template <typename T>

/**
 * @brief Represents a 2D vector.
 */
class Vector
{
public:
    /**
     * @brief Constructs a Vector object with the given x and y coordinates.
     *
     * @param x The x coordinate of the vector.
     * @param y The y coordinate of the vector.
     */
    Vector(T x, T y);

    /**
     * @brief Constructs a Vector object by copying another Vector object.
     *
     * @param v The Vector object to be copied.
     */
    Vector(const Vector<T> &v);

    /**
     * @brief Gets the x coordinate of the vector.
     *
     * @return The x coordinate of the vector.
     */
    T x() const;

    /**
     * @brief Gets the y coordinate of the vector.
     *
     * @return The y coordinate of the vector.
     */
    T y() const;

    /**
     * @brief Rotates the vector by the specified angle.
     *
     * @param angle The angle (in radians) by which to rotate the vector.
     */
    void rotate(double angle);

    /**
     * @brief Adds another vector to this vector.
     *
     * @param v The vector to be added.
     * @return A reference to this vector after the addition.
     */
    Vector &operator+=(const Vector<T> &v);

    /**
     * @brief Assigns the value of another vector to this vector.
     *
     * @param v The vector to be assigned.
     * @return A reference to this vector after the assignment.
     */
    Vector &operator=(const Vector<T> &v);

private:
    T _x, _y; // The x and y coordinates of the vector.
};

#endif