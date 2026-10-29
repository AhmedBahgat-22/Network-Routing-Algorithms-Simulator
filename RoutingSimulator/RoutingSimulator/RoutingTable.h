
#pragma once
#include <vector>
#include "RoutingEntry.h"

class RoutingTable {
public:
    std::vector<RoutingEntry> entries; // قائمة بكل المسارات اللي الراوتر يعرفها

    void addEntry(int dest, int next, int cost) {
        entries.push_back(RoutingEntry(dest, next, cost));
    }

    void clear() {
        entries.clear();
    }
};