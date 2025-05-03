#include <stdexcept>
#include <arpa/inet.h>
#include <unistd.h>

#include <UDPReceiver.h>

UDPReceiver::UDPReceiver(int port)
{
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        throw std::runtime_error("Failed to create socket");

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 100000;

    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (sockaddr *)&addr, sizeof(addr)) < 0)
        throw std::runtime_error("Failed to bind socket");
}

UDPReceiver::~UDPReceiver()
{
    close(sockfd);
}

UDPData UDPReceiver::receive()
{
    socklen_t len = sizeof(addr);
    int read = recvfrom(sockfd, buffer, bufferSize, MSG_WAITALL, (sockaddr *)&addr, &len);

    if (read != bufferSize)
        return {false, "", {0, 0}};

    return {
        true,
        std::string(inet_ntoa(addr.sin_addr)) + std::to_string(addr.sin_port),
        {buffer[0], buffer[1]}};
}
