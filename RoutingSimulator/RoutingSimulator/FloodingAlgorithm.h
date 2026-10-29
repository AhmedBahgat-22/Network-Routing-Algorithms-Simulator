#pragma once
#include "IRoutingAlgorithm.h"
#include <iostream>

// خوارزمية الغمر بتورث من الواجهة الموحدة
class FloodingAlgorithm : public IRoutingAlgorithm {
public:
    // بنحدد اسم الخوارزمية
    std::string getName() override {
        return "Flooding (Ghamr)";
    }

    // في الـ Flooding، مفيش جداول توجيه معقدة، هو بيبعت لكل الجيران
    void buildRoutingTables(std::vector<Router*>& routers) override {
        std::cout << "Flooding: Building simple broadcast rules for " << routers.size() << " routers." << std::endl;

        for (auto router : routers) {
            router->table.clear(); // بنمسح القديم
            for (auto neighborLink : router->neighbors) {
                // بنضيف قاعدة: عشان تروح للجار ده، ابعتله مباشرة
                router->table.addEntry(neighborLink->destination->id, neighborLink->destination->id, neighborLink->cost);
            }
        }
    }
};