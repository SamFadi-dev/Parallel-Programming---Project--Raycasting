#include <Map.h>
#include <util.h>
#include <textures.h>

Map::Map(
    int width, int height,
    const Texture &floorTexture,
    const Texture &ceilingTexture,
    std::initializer_list<Texture> textures,
    std::vector<Sprite> &sprites)
    : width(width),
      height(height),
      map(width * height),
      sprites(sprites),
      floorTexture(floorTexture),
      ceilingTexture(ceilingTexture)
{
    this->textures.reserve(textures.size());
    for (const Texture &texture : textures)
        this->textures.push_back(texture);
}

int Map::get(int x, int y) const { return map[x + y * width]; }
const Texture &Map::getFloorTexture() const { return floorTexture; }
const Texture &Map::getCeilingTexture() const { return ceilingTexture; }
const std::vector<Sprite> &Map::getSprites() const { return sprites; }

bool Map::hasWall(int x, int y) const
{
    return x < 0 || x >= width ||
           y < 0 || y >= height ||
           map[x + y * width] > 0;
}

const Texture &Map::getTexture(int x, int y) const
{
    return textures[map[x + y * width] - 1];
}

Map Map::generateMap(int nbPlayers)
{
    int width = 24, height = 24;

    int tiles[width][height] =
        {
            {8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 4, 6, 4, 4, 6, 4, 6, 4, 4, 4, 6, 4},
            {8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
            {8, 0, 3, 3, 0, 0, 0, 0, 0, 8, 8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
            {8, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
            {8, 0, 3, 3, 0, 0, 0, 0, 0, 8, 8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
            {8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 4, 0, 0, 0, 0, 0, 6, 6, 6, 0, 6, 4, 6},
            {8, 8, 8, 8, 0, 8, 8, 8, 8, 8, 8, 4, 4, 4, 4, 4, 4, 6, 0, 0, 0, 0, 0, 6},
            {7, 7, 7, 7, 0, 7, 7, 7, 7, 0, 8, 0, 8, 0, 8, 0, 8, 4, 0, 4, 0, 6, 0, 6},
            {7, 7, 0, 0, 0, 0, 0, 0, 7, 8, 0, 8, 0, 8, 0, 8, 8, 6, 0, 0, 0, 0, 0, 6},
            {7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 6, 0, 0, 0, 0, 0, 4},
            {7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 6, 0, 6, 0, 6, 0, 6},
            {7, 7, 0, 0, 0, 0, 0, 0, 7, 8, 0, 8, 0, 8, 0, 8, 8, 6, 4, 6, 0, 6, 6, 6},
            {7, 7, 7, 7, 0, 7, 7, 7, 7, 8, 8, 4, 0, 6, 8, 4, 8, 3, 3, 3, 0, 3, 3, 3},
            {2, 2, 2, 2, 0, 2, 2, 2, 2, 4, 6, 4, 0, 0, 6, 0, 6, 3, 0, 0, 0, 0, 0, 3},
            {2, 2, 0, 0, 0, 0, 0, 2, 2, 4, 0, 0, 0, 0, 0, 0, 4, 3, 0, 0, 0, 0, 0, 3},
            {2, 0, 0, 0, 0, 0, 0, 0, 2, 4, 0, 0, 0, 0, 0, 0, 4, 3, 0, 0, 0, 0, 0, 3},
            {1, 0, 0, 0, 0, 0, 0, 0, 1, 4, 4, 4, 4, 4, 6, 0, 6, 3, 3, 0, 0, 0, 3, 3},
            {2, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 1, 2, 2, 2, 6, 6, 0, 0, 5, 0, 5, 0, 5},
            {2, 2, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 2, 2, 0, 5, 0, 5, 0, 0, 0, 5, 5},
            {2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 5, 0, 5, 0, 5, 0, 5, 0, 5},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
            {2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 5, 0, 5, 0, 5, 0, 5, 0, 5},
            {2, 2, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 2, 2, 0, 5, 0, 5, 0, 0, 0, 5, 5},
            {2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 5, 5, 5, 5, 5, 5, 5, 5, 5}};

    Texture greenLight(64, 64, textures::greenlight, true);
    Texture pillar(64, 64, textures::pillar, true);
    Texture barrel(64, 64, textures::barrel, true);

    std::vector<Sprite> sprites;
    for (int i = 0; i < nbPlayers; i++)
        sprites.push_back(Sprite({-1, -1}, barrel));
    
    sprites.insert(
        sprites.end(),
        {
            Sprite({20.5, 11.5}, greenLight),
            Sprite({18.5, 4.5}, greenLight),
            Sprite({10.0, 4.5}, greenLight),
            Sprite({10.0, 12.5}, greenLight),
            Sprite({3.5, 6.5}, greenLight),
            Sprite({3.5, 20.5}, greenLight),
            Sprite({3.5, 14.5}, greenLight),
            Sprite({14.5, 20.5}, greenLight),

            Sprite({18.5, 10.5}, pillar),
            Sprite({18.5, 11.5}, pillar),
            Sprite({18.5, 12.5}, pillar),

            Sprite({21.5, 1.5}, barrel),
            Sprite({15.5, 1.5}, barrel),
            Sprite({16.0, 1.8}, barrel),
            Sprite({16.2, 1.2}, barrel),
            Sprite({3.5, 2.5}, barrel),
            Sprite({9.5, 15.5}, barrel),
            Sprite({10.0, 15.1}, barrel),
            Sprite({10.5, 15.8}, barrel),
        });

    Map map(
        width, height,
        Texture(64, 64, textures::greystone, false),
        Texture(64, 64, textures::wood, false),
        {
            Texture(64, 64, textures::eagle, true),
            Texture(64, 64, textures::redbrick, true),
            Texture(64, 64, textures::purplestone, true),
            Texture(64, 64, textures::greystone, true),
            Texture(64, 64, textures::bluestone, true),
            Texture(64, 64, textures::mossy, true),
            Texture(64, 64, textures::wood, true),
            Texture(64, 64, textures::colorstone, true),
        }, sprites);

    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++)
            map.map[x + y * width] = tiles[x][y];

    return map;
}

void Map::movePlayer(int index, double x, double y)
{
    sprites[index].move(x, y);
}