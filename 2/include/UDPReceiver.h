#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H

#include <netinet/in.h>
#include <string>

#include <Vector.h>

/**
 * @brief The UDPData struct represents the data received from a UDP packet.
 * It contains a flag indicating if the data is valid, the sender's IP address concatenated with the port number, and the received position.
 */
typedef struct
{
    bool valid;
    std::string sender;
    Vector<double> position;
} UDPData;

/**
 * @brief The UDPReceiver class is responsible for receiving position data using the UDP protocol.
 */
class UDPReceiver
{
public:
    /**
     * @brief Constructs a UDPReceiver object with the specified port.
     * @param port The port number to listen on.
     */
    UDPReceiver(int port);

    /**
     * @brief Destroys the UDPReceiver object and closes the socket.
     */
    ~UDPReceiver();

    /**
     * @brief Receives a UDP packet and returns the received position.
     * @return A UDPData object containing the received position. If no data is received, the valid flag is set to false and the sender's information is empty.
     */
    UDPData receive();

private:
    int sockfd;                          // The socket file descriptor.
    double buffer[2];                    // The buffer to store the received data.
    int bufferSize = 2 * sizeof(double); // The size of the buffer.
    sockaddr_in addr;                    // The address structure for the socket.
};

#endif