#include <sys/socket.h>
#include <stdexcept>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>

#include <UDPSender.h>

UDPSender::UDPSender(std::string ip, int port)
{
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        throw std::runtime_error("Failed to create socket");

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
}

UDPSender::~UDPSender()
{
    close(sockfd);
}

void UDPSender::send(double x, double y)
{
    buffer[0] = x;
    buffer[1] = y;

    sendto(sockfd, buffer, bufferSize, MSG_CONFIRM, (sockaddr *)&addr, sizeof(addr));
}