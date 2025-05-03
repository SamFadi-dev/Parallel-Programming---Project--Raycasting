#ifndef SPRITE_H
#define SPRITE_H

#include <Vector.h>
#include <Texture.h>

/**
 * @brief Represents a sprite in the game.
 */
class Sprite
{
public:
    /**
     * @brief Constructs a Sprite object with the given position and texture.
     *
     * @param position The position of the sprite.
     * @param texture The texture of the sprite.
     */
    Sprite(Vector<double> position, const Texture &texture);

    /**
     * @brief Gets the pixel value at the specified coordinates from the texture of the sprite.
     *
     * @param x The x-coordinate.
     * @param y The y-coordinate.
     * @return The pixel value at the specified coordinates.
     */
    unsigned int get(int x, int y) const;

    /**
     * @brief Gets the width of the sprite.
     *
     * @return The width of the sprite.
     */
    int getWidth() const;

    /**
     * @brief Gets the height of the sprite.
     *
     * @return The height of the sprite.
     */
    int getHeight() const;

    /**
     * @brief Gets the x-coordinate of the sprite's position.
     *
     * @return The x-coordinate of the sprite's position.
     */
    double posX() const;

    /**
     * @brief Gets the y-coordinate of the sprite's position.
     *
     * @return The y-coordinate of the sprite's position.
     */
    double posY() const;

    /**
     * @brief Moves the sprite to the specified position.
     *
     * @param x The x-coordinate of the new position.
     * @param y The y-coordinate of the new position.
     */
    void move(double x, double y);

private:
    Vector<double> position; // The position of the sprite.
    Texture texture;         // The texture of the sprite.
};

#endif