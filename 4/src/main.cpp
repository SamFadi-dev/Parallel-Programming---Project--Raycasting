#include <chrono>
#include <iostream>
#include <map>
#include <memory>

#include <Average.h>
#include <Player.h>
#include <Map.h>
#include <WindowManager.h>
#include <Raycaster.h>
#include <UDPReceiver.h>
#include <UDPSender.h>
#include <DoubleBuffer.h>
#include <util.h>

#include <thread>
#include <atomic>
#include <condition_variable>
#include <mutex>

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

void notifyPosition()
{

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

    size_t nbPlayers = udpSenders.size();
    Map map = Map::generateMap(nbPlayers);

    Player player({22, 11.5}, {-1, 0}, {0, 0.66}, 5, 3, map);

    // Indexes used to identify other players
    int nextPlayerIndex = 0;
    std::map<std::string, int> playersIndexes; // Maps IP addresses and ports to player indexes

    DoubleBuffer doubleBuffer(screenWidth, screenHeight);
    WindowManager windowManager(doubleBuffer);
    Raycaster raycaster(player, doubleBuffer, map);

    std::chrono::time_point<std::chrono::system_clock> time = std::chrono::system_clock::now(), oldTime;

    std::mutex sendMutex;
    std::condition_variable sendCondVar;
    std::atomic<bool> running(true);
    bool positionChanged = false;

    std::thread senderThread([&]() {
        std::unique_lock<std::mutex> lock(sendMutex);
        while (running) {
            sendCondVar.wait(lock, [&]() { return positionChanged || !running; });
            if (!running) break;
    
            for (auto &udpSender : udpSenders)
                udpSender->send(player.posX(), player.posY());
    
            positionChanged = false;
        }
    });

    Average fpsCounter(1.0);

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
        {
            player.move(frameTime);
            
            std::lock_guard<std::mutex> lock(sendMutex);
            positionChanged = true;
            sendCondVar.notify_one();
        }
        if (keys & WindowManager::KEY_DOWN)
        {
            player.move(-frameTime);

            std::lock_guard<std::mutex> lock(sendMutex);
            positionChanged = true;
            sendCondVar.notify_one();
        }
        if (keys & WindowManager::KEY_RIGHT)
        {
            player.turn(-frameTime);
            
            std::lock_guard<std::mutex> lock(sendMutex);
            positionChanged = true;
            sendCondVar.notify_one();
        }
        if (keys & WindowManager::KEY_LEFT)
        {
            player.turn(frameTime);
            
            std::lock_guard<std::mutex> lock(sendMutex);
            positionChanged = true;
            sendCondVar.notify_one();
        }
        if (keys & WindowManager::KEY_ESC)
        {
            running = false;
            // Release the thread
            sendCondVar.notify_one();
            senderThread.join();

            break;
        }

        // Receive other players' positions and update them
        for (size_t i = 0; i < nbPlayers; i++)
        {
            UDPData data = udpReceiver.receive();
            if (!data.valid)
                break;

            // Update the player's index if it is the first time we receive data from them
            if (playersIndexes.find(data.sender) == playersIndexes.end())
            {
                playersIndexes[data.sender] = nextPlayerIndex++;
                nextPlayerIndex %= nbPlayers;
            }
            int index = playersIndexes[data.sender];
            map.movePlayer(index, data.position.x(), data.position.y());
        }
    }
}
