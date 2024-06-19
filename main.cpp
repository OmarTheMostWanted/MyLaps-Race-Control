#include <iostream>
#include <iomanip>
#include "Server.h"

int main() {
    Server server(8080);
    server.run();
    return 0;
}

//int main() {
//    Race race;
//    race.readData("../TestData/karttimes.csv");
//    race.calculateBestLap();
//    race.calculateAverageLap();
//
//    // Sort drivers based on best lap time
//    std::sort(race.drivers.begin(), race.drivers.end(), [](const Driver& a, const Driver& b) {
//        return a.bestLap < b.bestLap;
//    });
//
//    // Print winner
//    std::cout << "Winner (Driver " << race.drivers[0].number << ") Best Lap Time: " << std::fixed << std::setprecision(2) << race.drivers[0].bestLap.count() / 100.0 << "s\n";
//
//    // Print 2nd and 3rd place
//    for (int i = 1; i < race.drivers.size() && i < race.drivers.size(); ++i) {
//        double delta = (race.drivers[i].bestLap.count() - race.drivers[0].bestLap.count()) / 100.0;
//        std::cout << i+1 << " Place (Driver " << race.drivers[i].number << ") Best Lap Time: " << std::fixed << std::setprecision(2) << race.drivers[i].bestLap.count() / 100.0 << "s Delta: " << delta << "s\n";
//    }
//
//    return 0;
//}