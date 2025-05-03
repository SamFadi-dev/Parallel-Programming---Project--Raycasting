#include <Player.h>

Player::Player(
    Vector<double> position,
    Vector<double> direction,
    Vector<double> camera,
    double moveSpeed,
    double rotSpeed,
    Map &map) : position(position),
                direction(direction),
                camera(camera),
                moveSpeed(moveSpeed),
                rotSpeed(rotSpeed),
                map(map)
{
}

double Player::posX() const { return position.x(); }
double Player::posY() const { return position.y(); }
double Player::dirX() const { return direction.x(); }
double Player::dirY() const { return direction.y(); }
double Player::camX() const { return camera.x(); }
double Player::camY() const { return camera.y(); }

void Player::move(double modifier)
{
    double x = position.x();
    double y = position.y();
    double nextX = x + direction.x() * moveSpeed * modifier;
    double nextY = y + direction.y() * moveSpeed * modifier;

    if (!map.hasWall(int(nextX), int(y)))
        moveX(modifier);
    if (!map.hasWall(int(x), int(nextY)))
        moveY(modifier);
}

void Player::moveX(double modifier)
{
    position += {direction.x() * moveSpeed * modifier, 0};
}

void Player::moveY(double modifier)
{
    position += {0, direction.y() * moveSpeed * modifier};
}

void Player::turn(double modifier)
{
    double rot = rotSpeed * modifier;
    direction.rotate(rot);
    camera.rotate(rot);
}

Vector<double> Player::generateRay(double cameraX) const
{
    return {direction.x() + camera.x() * cameraX,
            direction.y() + camera.y() * cameraX};
}
