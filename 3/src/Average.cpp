#include <limits>

#include <Average.h>

Average::Average(double secondsBeforeUpdate) : lastAvg(0.0),
                                               sum(0.0),
                                               n(0),
                                               secondsBeforeUpdate(secondsBeforeUpdate),
                                               lastUpdate(std::chrono::system_clock::now())
{
}

void Average::update(double value)
{
    sum += value;
    n++;
}

double Average::get() 
{
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = now - lastUpdate;
    if (elapsed.count() > secondsBeforeUpdate)
    {
        lastAvg = sum / (n + std::numeric_limits<double>::epsilon());
        sum = 0;
        n = 0;
        lastUpdate = now;
    }
    return lastAvg;
}