#include <Sprite.h>

Sprite::Sprite(Vector<double> position, const Texture &texture) : position(position), texture(texture)
{
}

unsigned int Sprite::get(int x, int y) const { return texture.get(x, y); }
int Sprite::getWidth() const { return texture.getWidth(); }
int Sprite::getHeight() const { return texture.getHeight(); }
double Sprite::posX() const { return position.x(); }
double Sprite::posY() const { return position.y(); }

void Sprite::move(double x, double y)
{
    position = {x, y};
}