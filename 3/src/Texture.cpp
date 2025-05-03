#include <Texture.h>

Texture::Texture(int width, int height, bool isVertical) : width(width), height(height), pixels(width * height), isVertical(isVertical)
{
}

Texture::Texture(int width, int height, const unsigned int *pixels, bool isVertical) : width(width), height(height), isVertical(isVertical)
{
    this->pixels.resize(width * height);
    if (isVertical)
        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++)
                this->pixels[y + x * height] = pixels[x + y * width];
    else
        for (int i = 0; i < width * height; i++)
            this->pixels[i] = pixels[i];
}

unsigned int Texture::get(int x, int y) const
{
    x &= width - 1;
    y &= height - 1;
    return isVertical ? pixels[y + x * height] : pixels[x + y * width];
}

int Texture::getWidth() const { return width; }
int Texture::getHeight() const { return height; }