#ifndef UDPSENDER_H
#define UDPSENDER_H

#include <netinet/in.h>
#include <string>

/**
 * @brief The UDPSender class is responsible for sending position data using the UDP protocol.
 */
class UDPSender
{
public:
    /**
     * @brief Constructs a UDPSender object with the specified destination IP address and port.
     *
     * @param ip The IP address to send the packets to.
     * @param port The port number to send the packets to.
     */
    UDPSender(std::string ip, int port);

    /**
     * @brief Destroys the UDPSender object and closes the socket.
     */
    ~UDPSender();

    /**
     * @brief Sends the given x and y coordinates as a UDP packet.
     *
     * @param x The x coordinate to send.
     * @param y The y coordinate to send.
     */
    void send(double x, double y);

private:
    int sockfd;                          // The socket file descriptor.
    double buffer[2];                    // The buffer to store the coordinates.
    sockaddr_in addr;                    // The address structure for the destination IP and port.
    int bufferSize = 2 * sizeof(double); // The size of the buffer.
};

#endif