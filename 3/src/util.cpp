#include <fstream>

#include <util.h>

unsigned int rgb(int r, int g, int b)
{
    return r << 16 | g << 8 | b;
}

NetworkData parseIPs(std::string path)
{
    NetworkData data;
    std::ifstream f(path);
    if (!f.is_open())
        throw std::runtime_error("Failed to open file");

    std::string tmp;    
    std::getline(f, tmp);
    data.listeningPort = std::stoi(tmp);
    
    while (!f.eof())
    {
        std::getline(f, tmp, ' ');
        std::string ip = tmp;
     
        std::getline(f, tmp);
        int port = std::stoi(tmp);
     
        data.ipPorts.push_back({ip, port});
    }

    return data;
}