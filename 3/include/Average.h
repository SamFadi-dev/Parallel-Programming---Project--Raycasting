#ifndef AVERAGE_H
#define AVERAGE_H

#include <queue>
#include <chrono>

/**
 * @brief A class that calculates the average of a series of values.
 */
class Average
{
public:
    /**
     * @brief Constructs an Average object with the specified time interval before updating the average.
     *
     * @param secondsBeforeUpdate The time interval in seconds before updating the average.
     */
    Average(double secondsBeforeUpdate);

    /**
     * @brief Aggregates a new value to be included in the average calculation.
     *
     * @param value The new value to be included in the average calculation.
     */
    void update(double value);

    /**
     * @brief Gets the current average value. If the time interval has passed since the last call to this method, the average is recalculated.
     *
     * @return The current average value.
     */
    double get();

private:
    double lastAvg;                                                // The last calculated average value
    double sum;                                                    // The sum of all values to be included in the average calculation
    int n;                                                         // The number of values
    double secondsBeforeUpdate;                                    // The time interval in seconds before updating the average
    std::chrono::time_point<std::chrono::system_clock> lastUpdate; // The timestamp of the last update
};

#endif