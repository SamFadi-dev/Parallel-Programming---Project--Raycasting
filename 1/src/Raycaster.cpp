/**
 * The following implementation is mainly based on the tutorial by Lode Vandevenne: https://lodev.org/cgtutor/raycasting.html
 */

#include <cmath>
#include <algorithm>

#include <Raycaster.h>

Raycaster::Raycaster(Player &player, DoubleBuffer &doubleBuffer, Map &map) : player(player),
                                                                             doubleBuffer(doubleBuffer),
                                                                             map(map),
                                                                             screenWidth(doubleBuffer.getWidth()),
                                                                             screenHeight(doubleBuffer.getHeight()),
                                                                             floorTexture(map.getFloorTexture()),
                                                                             ceilingTexture(map.getCeilingTexture()),
                                                                             zBuffer(screenWidth),
                                                                             spriteOrder(map.getSprites().size()),
                                                                             spriteDistance(map.getSprites().size()),
                                                                             numSprites(map.getSprites().size())
{
}

void Raycaster::castFloorCeiling()
{
    #pragma omp parallel num_threads(4)
    {
        Vector<double> rayDir0 = {0, 0}, rayDir1 = {0, 0};
        // Vertical position of the camera.
        double posZ = 0.5 * screenHeight;

        #pragma omp for 
        for (int y = screenHeight / 2; y < screenHeight; y++)
        {
            // rayDir for leftmost ray (x = 0) and rightmost ray (x = w)
            rayDir0 = {player.dirX() - player.camX(), player.dirY() - player.camY()};
            rayDir1 = {player.dirX() + player.camX(), player.dirY() + player.camY()};

            // Current y position compared to the center of the screen (the horizon)
            int p = y - screenHeight / 2;

            // Horizontal distance from the camera to the floor for the current row.
            // 0.5 is the z position exactly in the middle between floor and ceiling.
            double rowDistance = posZ / p;

            // calculate the real world step vector we have to add for each x (parallel to camera plane)
            // adding step by step avoids multiplications with a weight in the inner loop
            double floorStepX = rowDistance * (rayDir1.x() - rayDir0.x()) / screenWidth;
            double floorStepY = rowDistance * (rayDir1.y() - rayDir0.y()) / screenWidth;

            // real world coordinates of the leftmost column
            double floorXBasis = player.posX() + rowDistance * rayDir0.x();
            double floorYBasis = player.posY() + rowDistance * rayDir0.y();
            for (int x = 0; x < screenWidth; ++x)
            {
                double floorX = floorXBasis + x * floorStepX;
                double floorY = floorYBasis + x * floorStepY;

                // the cell coord is simply got from the integer parts of floorX and floorY
                int cellX = int(floorX);
                int cellY = int(floorY);

                // get the texture coordinate from the fractional part
                int tx = int(floorTexture.getWidth() * (floorX - cellX)) & (floorTexture.getWidth() - 1);
                int ty = int(floorTexture.getHeight() * (floorY - cellY)) & (floorTexture.getHeight() - 1);

                unsigned int color;

                // floor
                color = floorTexture.get(tx, ty);
                color = (color >> 1) & 8355711; // make a bit darker
                doubleBuffer.drawPixel(x, y, color);

                // ceiling (symmetrical, at screenHeight - y - 1 instead of y)
                color = ceilingTexture.get(tx, ty);
                color = (color >> 1) & 8355711; // make a bit darker
                doubleBuffer.drawPixel(x, screenHeight - y - 1, color);
            }
        }
    }
}

void Raycaster::castWalls()
{
    #pragma omp parallel num_threads(4)
    {
        #pragma omp for
        for (int x = 0; x < screenWidth; x++)
        {
            // calculate ray position and direction
            double cameraX = 2 * x / double(screenWidth) - 1; // x-coordinate in camera space
            Vector<double> ray = player.generateRay(cameraX);
            // which box of the map we're in
            int mapX = int(player.posX());
            int mapY = int(player.posY());

            // length of ray from current position to next x or y-side
            double sideDistX;
            double sideDistY;

            // length of ray from one x or y-side to next x or y-side
            // these are derived as:
            // deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX))
            // deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY))
            // which can be simplified to abs(|rayDir| / rayDirX) and abs(|rayDir| / rayDirY)
            // where |rayDir| is the length of the vector (rayDirX, rayDirY). Its length,
            // unlike (dirX, dirY) is not 1, however this does not matter, only the
            // ratio between deltaDistX and deltaDistY matters, due to the way the DDA
            // stepping further below works. So the values can be computed as below.
            //  Division through zero is prevented, even though technically that's not
            //  needed in C++ with IEEE 754 floating point values.
            double deltaDistX = (ray.x() == 0) ? 1e30 : std::abs(1 / ray.x());
            double deltaDistY = (ray.y() == 0) ? 1e30 : std::abs(1 / ray.y());

            double perpWallDist;

            // what direction to step in x or y-direction (either +1 or -1)
            int stepX;
            int stepY;

            int hit = 0; // was there a wall hit?
            int side;    // was a NS or a EW wall hit?
            // calculate step and initial sideDist
            if (ray.x() < 0)
            {
                stepX = -1;
                sideDistX = (player.posX() - mapX) * deltaDistX;
            }
            else
            {
                stepX = 1;
                sideDistX = (mapX + 1.0 - player.posX()) * deltaDistX;
            }
            if (ray.y() < 0)
            {
                stepY = -1;
                sideDistY = (player.posY() - mapY) * deltaDistY;
            }
            else
            {
                stepY = 1;
                sideDistY = (mapY + 1.0 - player.posY()) * deltaDistY;
            }
            // perform DDA
            while (hit == 0)
            {
                // jump to next map square, either in x-direction, or in y-direction
                if (sideDistX < sideDistY)
                {
                    sideDistX += deltaDistX;
                    mapX += stepX;
                    side = 0;
                }
                else
                {
                    sideDistY += deltaDistY;
                    mapY += stepY;
                    side = 1;
                }
                // Check if ray has hit a wall
                if (map.get(mapX, mapY) > 0)
                    hit = 1;
            }
            // Calculate distance projected on camera direction. This is the shortest distance from the point where the wall is
            // hit to the camera plane. Euclidean to center camera point would give fisheye effect!
            // This can be computed as (mapX - posX + (1 - stepX) / 2) / rayDirX for side == 0, or same formula with Y
            // for size == 1, but can be simplified to the code below thanks to how sideDist and deltaDist are computed:
            // because they were left scaled to |rayDir|. sideDist is the entire length of the ray above after the multiple
            // steps, but we subtract deltaDist once because one step more into the wall was taken above.
            if (side == 0)
                perpWallDist = (sideDistX - deltaDistX);
            else
                perpWallDist = (sideDistY - deltaDistY);

            int lineHeight = int(screenHeight / perpWallDist);

            int drawStart = -lineHeight / 2 + screenHeight / 2;
            if (drawStart < 0)
                drawStart = 0;
            int drawEnd = lineHeight / 2 + screenHeight / 2;
            if (drawEnd >= screenHeight)
                drawEnd = screenHeight - 1;

            Texture texture = map.getTexture(mapX, mapY);

            // calculate value of wallX
            double wallX; // where exactly the wall was hit
            if (side == 0)
                wallX = player.posY() + perpWallDist * ray.y();
            else
                wallX = player.posX() + perpWallDist * ray.x();
            wallX -= floor(wallX);

            // x coordinate on the texture
            int texX = int(wallX * double(texture.getWidth()));
            if (side == 0 && ray.x() > 0)
                texX = texture.getWidth() - texX - 1;
            if (side == 1 && ray.y() < 0)
                texX = texture.getWidth() - texX - 1;

            doubleBuffer.drawVertLine(x, drawStart, drawEnd, lineHeight, texture, texX, side == 1);

            zBuffer[x] = perpWallDist;
        }
    }
}

void Raycaster::castSprites()
{
    #pragma omp parallel num_threads(4)
    {
        std::vector<Sprite> sprites = map.getSprites();

        int screenWidth = doubleBuffer.getWidth();
        int screenHeight = doubleBuffer.getHeight();

        // sort sprites from far to close
        #pragma omp for
        for (int i = 0; i < numSprites; i++)
        {
            spriteOrder[i] = i;
            Sprite sprite = sprites[i];
            spriteDistance[i] = pow(player.posX() - sprite.posX(), 2) + pow(player.posY() - sprite.posY(), 2); // sqrt not taken, unneeded
        }

        sortSprites();

        // after sorting the sprites, do the projection and draw them
        #pragma omp for
        for (int i = 0; i < numSprites; i++)
        {
            Sprite sprite = sprites[spriteOrder[i]];

            // translate sprite position to relative to camera
            double spriteX = sprite.posX() - player.posX();
            double spriteY = sprite.posY() - player.posY();

            // transform sprite with the inverse camera matrix
            //  [ planeX   dirX ] -1                                       [ dirY      -dirX ]
            //  [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
            //  [ planeY   dirY ]                                          [ -planeY  planeX ]

            double invDet = 1.0 / (player.camX() * player.dirY() - player.dirX() * player.camY()); // required for correct matrix multiplication

            double transformX = invDet * (player.dirY() * spriteX - player.dirX() * spriteY);
            double transformY = invDet * (-player.camY() * spriteX + player.camX() * spriteY); // this is actually the depth inside the screen, that what Z is in 3D

            int spriteScreenX = int((screenWidth / 2) * (1 + transformX / transformY));

            // calculate height of the sprite on screen
            int spriteHeight = abs(int(screenHeight / (transformY))); // using 'transformY' instead of the real distance prevents fisheye
            // calculate lowest and highest pixel to fill in current stripe
            int drawStartY = -spriteHeight / 2 + screenHeight / 2;
            if (drawStartY < 0)
                drawStartY = 0;
            int drawEndY = spriteHeight / 2 + screenHeight / 2;
            if (drawEndY >= screenHeight)
                drawEndY = screenHeight - 1;

            // calculate width of the sprite
            int spriteWidth = abs(int(screenHeight / (transformY)));
            int drawStartX = -spriteWidth / 2 + spriteScreenX;
            if (drawStartX < 0)
                drawStartX = 0;
            int drawEndX = spriteWidth / 2 + spriteScreenX;
            if (drawEndX >= screenWidth)
                drawEndX = screenWidth - 1;

            // loop through every vertical stripe of the sprite on screen
            for (int stripe = drawStartX; stripe < drawEndX; stripe++)
            {
                int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * sprite.getWidth() / spriteWidth) / 256;
                // the conditions in the if are:
                // 1) it's in front of camera plane so you don't see things behind you
                // 2) it's on the screen (left)
                // 3) it's on the screen (right)
                // 4) ZBuffer, with perpendicular distance
                if (transformY > 0 && stripe > 0 && stripe < screenWidth && transformY < zBuffer[stripe])
                    for (int y = drawStartY; y < drawEndY; y++) // for every pixel of the current stripe
                    {
                        int d = (y) * 256 - screenHeight * 128 + spriteHeight * 128; // 256 and 128 factors to avoid floats
                        int texY = ((d * sprite.getHeight()) / spriteHeight) / 256;
                        unsigned int color = sprite.get(texX, texY); // get current color from the texture
                        if ((color & 0x00FFFFFF) != 0)
                            doubleBuffer.drawPixel(stripe, y, color); // paint pixel if it isn't black, black is the invisible color
                    }
            }
        }
    }
}

void Raycaster::sortSprites()
{
    std::vector<std::pair<double, int>> sprites(numSprites);
    for (int i = 0; i < numSprites; i++)
    {
        sprites[i].first = spriteDistance[i];
        sprites[i].second = spriteOrder[i];
    }
    std::sort(sprites.begin(), sprites.end());
    // restore in reverse order to go from farthest to nearest
    for (int i = 0; i < numSprites; i++)
    {
        spriteDistance[i] = sprites[numSprites - i - 1].first;
        spriteOrder[i] = sprites[numSprites - i - 1].second;
    }
}