#include <chrono>
#include <iostream>
#include <map>
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>

#include <Average.h>
#include <Player.h>
#include <Map.h>
#include <WindowManager.h>
#include <Raycaster.h>
#include <UDPReceiver.h>
#include <UDPSender.h>
#include <DoubleBuffer.h>
#include <util.h>

struct ProgramArguments
{
    int screenWidth;
    int screenHeight;
    std::string ipsPath;
};

ProgramArguments parseArgs(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " <screenWidth> <screenHeight> <ipsPath>" << std::endl;
        std::cerr << "  screenWidth: The width of the screen." << std::endl;
        std::cerr << "  screenHeight: The height of the screen." << std::endl;
        std::cerr << "  ipsPath: The path to the file containing the IP addresses and ports of the players." << std::endl;
        std::cerr << "Example: " << argv[0] << " 1920 1080 ips.txt" << std::endl;
        exit(1);
    }

    ProgramArguments args;
    args.screenWidth = std::stoi(argv[1]);
    args.screenHeight = std::stoi(argv[2]);
    args.ipsPath = argc == 4 ? argv[3] : "";
    return args;
}

int main(int argc, char *argv[])
{
    ProgramArguments args = parseArgs(argc, argv);
    const int screenWidth = args.screenWidth;
    const int screenHeight = args.screenHeight;

    std::vector<std::unique_ptr<UDPSender>> udpSenders;
    NetworkData data = parseIPs(args.ipsPath);
    UDPReceiver udpReceiver(data.listeningPort);

    for (auto ipPort : data.ipPorts)
        udpSenders.push_back(std::unique_ptr<UDPSender>(new UDPSender(ipPort.first, ipPort.second)));

    // Indexes used to identify other players
    int nextPlayerIndex = 0;
    std::map<std::string, int> playersIndexes; // Maps IP addresses and ports to player indexes

    size_t nbPlayers = udpSenders.size();
    Map map = Map::generateMap(nbPlayers);
    Player player({22, 11.5}, {-1, 0}, {0, 0.66}, 5, 3, map);
    DoubleBuffer doubleBuffer(screenWidth, screenHeight);
    WindowManager windowManager(doubleBuffer);
    Raycaster raycaster(player, doubleBuffer, map);

    std::chrono::time_point<std::chrono::system_clock> time = std::chrono::system_clock::now(), oldTime;

    Average fpsCounter(1.0);

    std::atomic<bool> running(true);
    std::mutex playersMutex;

    // Receive other players' positions and update them in a separated thread
    std::thread receiverThread([&]() {
        while (running) {
            UDPData received = udpReceiver.receive();
            if (received.valid) {
                std::lock_guard<std::mutex> lock(playersMutex);
                if (playersIndexes.find(received.sender) == playersIndexes.end()) {
                    playersIndexes[received.sender] = nextPlayerIndex++;
                    nextPlayerIndex %= nbPlayers;
                }
                int index = playersIndexes[received.sender];
                map.movePlayer(index, received.position.x(), received.position.y());
            }
        }
    });

    while (true)
    {
        raycaster.castFloorCeiling();
        raycaster.castWalls();
        raycaster.castSprites();

        doubleBuffer.swap();

        oldTime = time;
        time = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed = time - oldTime;
        double frameTime = elapsed.count();

        fpsCounter.update(1.0 / frameTime);
        std::cout << "\r" << std::to_string(int(fpsCounter.get())) << " FPS" << std::flush;

        windowManager.updateDisplay();
        windowManager.updateInput();

        unsigned int keys = windowManager.getKeysPressed();
        if (keys & WindowManager::KEY_UP)
            player.move(frameTime);
        if (keys & WindowManager::KEY_DOWN)
            player.move(-frameTime);
        if (keys & WindowManager::KEY_RIGHT)
            player.turn(-frameTime);
        if (keys & WindowManager::KEY_LEFT)
            player.turn(frameTime);
        if (keys & WindowManager::KEY_ESC)
        {
            running = false;
            receiverThread.join();
            break;
        }

        // Send position to other players
        for (auto &udpSender : udpSenders)
            udpSender->send(player.posX(), player.posY());
    }
}
