#include <DoubleBuffer.h>

DoubleBuffer::DoubleBuffer(int width, int height) : width(width), height(height), frontBuffer(width * height), backBuffer(width * height)
{
}

const std::vector<int> &DoubleBuffer::getBackBuffer() const { return backBuffer; }
int DoubleBuffer::getWidth() const { return width; }
int DoubleBuffer::getHeight() const { return height; }

void DoubleBuffer::drawVertLine(int x, int yStart, int yEnd, int lineHeight, Texture &texture, int texX, bool darken)
{
    double step = double(texture.getHeight()) / lineHeight;
    double texY = (yStart - height / 2 + lineHeight / 2) * step;
    for (int y = yStart; y <= yEnd; y++)
    {
        unsigned int color = texture.get(texX, int(texY));
        texY += step;
        if (darken)
            color = (color >> 1) & 8355711;
        frontBuffer[x + y * width] = color;
    }
}

void DoubleBuffer::drawPixel(int x, int y, unsigned int color)
{
    frontBuffer[x + y * width] = color;
}

void DoubleBuffer::swap()
{
    frontBuffer.swap(backBuffer);
}