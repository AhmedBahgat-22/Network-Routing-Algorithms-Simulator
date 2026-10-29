#pragma once
#include "IRoutingAlgorithm.h"
#include "DijkstraAlgorithm.h" // هنستخدم ديكسترا اللي عملناه قبل كده

class LinkStateAlgorithm : public IRoutingAlgorithm {
public:
    std::string getName() override { return "Link State (OSPF Style)"; }

    void buildRoutingTables(std::vector<Router*>& routers) override {
        // في الـ Link State، كل راوتر بيبني "خريطة كاملة" (Global View)
        // وبما إننا في المحاكي، إحنا بنستخدم الـ Global Topology ونشغل عليها Dijkstra
        DijkstraAlgorithm solver;
        solver.buildRoutingTables(routers);
        std::cout << "Link State: Map built and Dijkstra executed for all routers." << std::endl;
    }
};