#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>

/**
 * @brief The Texture class represents a texture.
 */
class Texture
{
public:
    /**
     * @brief Constructs a Texture object with the specified width, height, and orientation.
     *
     * @param width The width of the texture.
     * @param height The height of the texture.
     * @param isVertical Whether the texture is stored vertically.
     * Depending on the use case, this can prevent cache misses and improve performance.
     */
    Texture(int width, int height, bool isVertical);

    /**
     * @brief Constructs a Texture object with the specified width, height, pixels, and orientation.
     *
     * @param width The width of the texture.
     * @param height The height of the texture.
     * @param pixels An array of pixels representing the texture. These values are copied to the texture.
     * @param isVertical Whether the texture is stored vertically.
     * Depending on the use case, this can prevent cache misses and improve performance.
     */
    Texture(int width, int height, const unsigned int *pixels, bool isVertical);

    /**
     * @brief Gets the pixel value at the specified coordinates.
     *
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @return The pixel value at the specified coordinates.
     */
    unsigned int get(int x, int y) const;

    /**
     * @brief Gets the width of the texture.
     *
     * @return The width of the texture.
     */
    int getWidth() const;

    /**
     * @brief Gets the height of the texture.
     *
     * @return The height of the texture.
     */
    int getHeight() const;

private:
    int width;                        // The width of the texture.
    int height;                       // The height of the texture.
    std::vector<unsigned int> pixels; // The array of pixels representing the texture.
    bool isVertical;                  // Whether the texture is stored vertically.
};

#endif