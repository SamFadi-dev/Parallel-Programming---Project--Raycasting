#ifndef RAYCASTER_H
#define RAYCASTER_H

#include <vector>

#include <Player.h>
#include <DoubleBuffer.h>
#include <Map.h>

/**
 * @brief The Raycaster class is responsible for casting rays and rendering the scene in a 3D environment.
 */
class Raycaster
{
public:
    /**
     * @brief Constructs a Raycaster object.
     * @param player The reference to the player from which the rays are cast.
     * @param windowManager The reference to the WindowManager object for rendering the scene.
     * @param map The reference to the Map object representing the game world.
     */
    Raycaster(Player &player, DoubleBuffer &doubleBuffer, Map &map);

    /**
     * @brief Casts rays to render the floor and ceiling of the scene.
     */
    void castFloorCeiling();

    /**
     * @brief Casts rays to render the walls of the scene.
     */
    void castWalls();

    /**
     * @brief Casts rays to render the sprites in the scene.
     */
    void castSprites();

private:
    Player &player;               // The reference to the Player object.
    DoubleBuffer &doubleBuffer;   // The double buffer for rendering the scene.
    Map &map;                     // The map of the game.

    int screenWidth, screenHeight;        // The screen width and height.
    Texture floorTexture, ceilingTexture; // The textures for the floor and ceiling.

    std::vector<double> zBuffer;        // The buffer for storing the distance of the walls from the player (used for rendering sprites).
    std::vector<int> spriteOrder;       // The order of the sprites for rendering.
    std::vector<double> spriteDistance; // The distances of the sprites from the player.
    int numSprites;                     // The number of sprites in the map.

    /**
     * @brief Sorts the sprites based on their distance from the player.
     */
    void sortSprites();
};

#endif