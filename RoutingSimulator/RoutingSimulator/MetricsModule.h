#pragma once
#include <vector>
#include <iostream>

class MetricsModule {
public:
    int totalControlMessages; // عدد الرسائل اللي الخوارزمية استهلكتها

    MetricsModule() : totalControlMessages(0) {}

    void logControlMessage() { totalControlMessages++; }

    void printSummary(std::string algName, int hops, int cost) {
        std::cout << "--- " << algName << " Results ---" << std::endl;
        std::cout << "Hop Count: " << hops << " | Path Cost: " << cost << std::endl;
        std::cout << "Control Messages: " << totalControlMessages << std::endl;
        std::cout << "---------------------------------" << std::endl;
    }
};