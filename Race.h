//
// Created by tmw on 6/19/24.
//

#ifndef MYLAPS_RACE_H
#define MYLAPS_RACE_H

#include <vector>
#include <chrono>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <iomanip>

struct Driver {
    int number;
    std::vector<std::chrono::time_point<std::chrono::system_clock>> lapTimes;
    std::vector<std::chrono::duration<double, std::ratio<1, 100>>> calculatedLapTimes;
    std::chrono::duration<double, std::ratio<1, 100>> bestLap;
    std::chrono::duration<double, std::ratio<1, 100>> averageLap;
};

class Race {
public:
    std::vector<Driver> drivers;

public:
    void readData(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;
        std::getline(file, line); // skip header
        while (std::getline(file, line)) {
            std::istringstream ss(line);
            std::string token;
            int number;
            std::getline(ss, token, ',');
            number = std::stoi(token);
            std::getline(ss, token, ',');
            std::tm tm = {};
            std::istringstream ssTime(token);
            ssTime >> std::get_time(&tm, "%H:%M:%S");
            auto timePoint = std::chrono::system_clock::from_time_t(std::mktime(&tm));
            auto it = std::find_if(drivers.begin(), drivers.end(), [&](const Driver& d) { return d.number == number; });
            if (it == drivers.end()) {
                Driver driver;
                driver.number = number;
                driver.lapTimes.push_back(timePoint);
                drivers.push_back(driver);
            } else {
                it->lapTimes.push_back(timePoint);
                if (it->lapTimes.size() > 1) {
                    auto lapTime = std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1, 100>>>(it->lapTimes.back() - it->lapTimes[it->lapTimes.size() - 2]);
                    it->calculatedLapTimes.emplace_back(lapTime);
                }
            }
        }
    }

    void calculateBestLap() {
        for (auto& driver : drivers) {
            driver.bestLap = *std::min_element(driver.calculatedLapTimes.begin(), driver.calculatedLapTimes.end());
        }
    }

    void calculateAverageLap() {
        for (auto& driver : drivers) {
            std::chrono::duration<double, std::ratio<1, 100>> totalLapTime = std::accumulate(driver.calculatedLapTimes.begin(), driver.calculatedLapTimes.end(), std::chrono::duration<double, std::ratio<1, 100>>(0));
            driver.averageLap = totalLapTime / driver.calculatedLapTimes.size();
        }
    }
};

#endif //MYLAPS_RACE_H
