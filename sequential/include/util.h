#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

/**
 * @brief Converts the given RGB values to a single unsigned integer.
 *
 * @param r The red component of the RGB color (0-255).
 * @param g The green component of the RGB color (0-255).
 * @param b The blue component of the RGB color (0-255).
 * @return The RGB color represented as an unsigned integer.
 */
unsigned int rgb(int r, int g, int b);

/**
 * @brief Represents the network data.
 */
struct NetworkData
{
    int listeningPort;                                // The port on which the server listens for incoming connections.
    std::vector<std::pair<std::string, int>> ipPorts; // The list of IP addresses and ports.
};

/**
 * @brief Parses the IP addresses and ports from the specified file.
 * The file should contain the listening port on the first line, followed by the IP addresses and ports on subsequent lines.
 * Example:
 * 12345
 * 127.0.0.1 12346
 * 127.0.0.1 12347
 *
 * @param path The path to the file containing the IP addresses and ports.
 * @return The network data.
 */
NetworkData parseIPs(std::string path);

#endif