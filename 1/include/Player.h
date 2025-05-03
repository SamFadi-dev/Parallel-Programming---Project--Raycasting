#ifndef PLAYER_H
#define PLAYER_H

#include <Vector.h>
#include <Map.h>

/**
 * @brief Represents the playable character in the game.
 */
class Player
{
public:
    /**
     * @brief Constructs a Player object.
     *
     * @param pos The initial position of the player.
     * @param dir The initial direction of the player.
     * @param cam The initial camera vector of the player.
     * @param moveSpeed The movement speed (unit/s) of the player.
     * @param rotSpeed The rotation speed (rad/s) of the player.
     * @param map The map object representing the game world.
     */
    Player(Vector<double> pos, Vector<double> dir, Vector<double> cam, double moveSpeed, double rotSpeed, Map &map);

    /**
     * @brief Get the x-coordinate of the player's position.
     *
     * @return The x-coordinate of the player's position.
     */
    double posX() const;

    /**
     * @brief Get the y-coordinate of the player's position.
     *
     * @return The y-coordinate of the player's position.
     */
    double posY() const;

    /**
     * @brief Get the x-component of the player's direction vector.
     *
     * @return The x-component of the player's direction vector.
     */
    double dirX() const;

    /**
     * @brief Get the y-component of the player's direction vector.
     *
     * @return The y-component of the player's direction vector.
     */
    double dirY() const;

    /**
     * @brief Get the x-component of the player's camera vector.
     *
     * @return The x-component of the player's camera vector.
     */
    double camX() const;

    /**
     * @brief Get the y-component of the player's camera vector.
     *
     * @return The y-component of the player's camera vector.
     */
    double camY() const;

    /**
     * @brief Move the player in the game world.
     *
     * @param modifier The modifier multiplied to the movement speed.
     */
    void move(double modifier);

    /**
     * @brief Turn the player's direction in the game world.
     *
     * @param modifier The modifier multiplied to the rotation speed.
     */
    void turn(double modifier);

    /**
     * @brief Generate a ray from the player's camera for raycasting.
     *
     * @param cameraX The x-coordinate in camera space.
     * @return The ray vector.
     */
    Vector<double> generateRay(double cameraX) const;

private:
    Vector<double> position;  // The position of the player.
    Vector<double> direction; // The direction vector of the player.
    Vector<double> camera;    // The camera vector of the player.
    double moveSpeed;         // The movement speed of the player.
    double rotSpeed;          // The rotation speed of the player.
    Map &map;                 // The map object representing the game world.

    /**
     * @brief Move the player along the x-axis.
     *
     * @param modifier The modifier multiplied to the movement speed.
     */
    void moveX(double modifier);

    /**
     * @brief Move the player along the y-axis.
     *
     * @param modifier The modifier multiplied to the movement speed.
     */
    void moveY(double modifier);
};

#endif