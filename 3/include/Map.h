#ifndef MAP_H
#define MAP_H

#include <vector>

#include <Texture.h>
#include <Sprite.h>
#include <mutex>

/**
 * @brief Represents a game map.
 */
class Map
{
public:
    /**
     * @brief Constructs a Map object with the specified width, height, floor texture, ceiling texture, textures, and sprites.
     *
     * @param width The width of the map.
     * @param height The height of the map.
     * @param floorTexture The texture for the floor.
     * @param ceilingTexture The texture for the ceiling.
     * @param textures The list of textures for the walls.
     * @param sprites The list of sprites in the map.
     */
    Map(int width, int height,
        const Texture &floorTexture,
        const Texture &ceilingTexture,
        std::initializer_list<Texture> textures,
        std::vector<Sprite> &sprites);

    /**
     * @brief Gets the value at the specified position in the map.
     *
     * @param x The x-coordinate of the position.
     * @param y The y-coordinate of the position.
     * @return The value at the specified position.
     */
    int get(int x, int y) const;

    /**
     * @brief Gets the floor texture of the map.
     *
     * @return The floor texture.
     */
    const Texture &getFloorTexture() const;

    /**
     * @brief Gets the ceiling texture of the map.
     *
     * @return The ceiling texture.
     */
    const Texture &getCeilingTexture() const;

    /**
     * @brief Gets the list of sprites in the map.
     *
     * @return The list of sprites.
     */
    const std::vector<Sprite> &getSprites() const;

    /**
     * @brief Checks if there is a wall at the specified position in the map.
     *
     * @param x The x-coordinate of the position.
     * @param y The y-coordinate of the position.
     * @return True if there is a wall, false otherwise.
     */
    bool hasWall(int x, int y) const;

    /**
     * @brief Gets the texture at the specified position in the map.
     *
     * @param x The x-coordinate of the position.
     * @param y The y-coordinate of the position.
     * @return The texture at the specified position.
     */
    const Texture &getTexture(int x, int y) const;

    /**
     * @brief Moves the sprite of the player at the specified index to the specified position.
     *
     * @param index The index of the player.
     * @param x The x-coordinate of the position.
     * @param y The y-coordinate of the position.
     */
    void movePlayer(int index, double x, double y);

    /**
     * @brief Generates a map with the specified number of players.
     *
     * @param nbPlayers The number of players.
     * @return The generated map.
     */
    static Map generateMap(int nbPlayers);

private:
    int width, height;                    // The width and height of the map.
    std::vector<int> map;                 // The map data.
    std::vector<Sprite> sprites;          // The list of sprites in the map.
    std::vector<Texture> textures;        // The list of textures for the walls.
    Texture floorTexture, ceilingTexture; // The textures for the floor and ceiling.
};

#endif