#ifndef DOUBLEBUFFER_H
#define DOUBLEBUFFER_H

#include <vector>

#include <Texture.h>

/**
 * @brief The DoubleBuffer class represents a double buffer which can be used to draw to a window.
 *
 * This class contains a front buffer, on which drawing operations are performed, and a back buffer, which can
 * be used to display content on a window. The front and back buffers can be swapped to display the content
 * drawn on the front buffer.
 */
class DoubleBuffer
{
public:
    /**
     * @brief Constructs a DoubleBuffer object, which consist of a front and back buffer.
     *
     * @param width The width of the buffer.
     * @param height The height of the buffer.
     */
    DoubleBuffer(int width, int height);

    /**
     * @brief Returns the back buffer.
     *
     * @return The back buffer.
     */
    const std::vector<int> &getBackBuffer() const;

    /**
     * @brief Gets the width of the window.
     *
     * @return The width of the window.
     */
    int getWidth() const;

    /**
     * @brief Gets the height of the window.
     *
     * @return The height of the window.
     */
    int getHeight() const;

    /**
     * @brief Draws a vertical line on the front buffer.
     *
     * @param x The x-coordinate of the line on the buffer.
     * @param yStart The starting y-coordinate of the line on the buffer.
     * @param yEnd The ending y-coordinate of the line on the buffer.
     * @param lineHeight The height of the line.
     * @param texture The texture to use for drawing the line.
     * @param texX The x-coordinate of the texture to start drawing from.
     * @param darken Whether to darken the line or not.
     */
    void drawVertLine(int x, int yStart, int yEnd, int lineHeight, Texture &texture, int texX, bool darken);

    /**
     * @brief Draws a pixel on the front buffer.
     *
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @param color The color of the pixel.
     */
    void drawPixel(int x, int y, unsigned int color);

    /**
     * @brief Swaps the front and back buffers.
     */
    void swap();

private:
    int width;
    int height;
    std::vector<int> frontBuffer;
    std::vector<int> backBuffer;
};

#endif