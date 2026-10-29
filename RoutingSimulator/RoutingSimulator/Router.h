#pragma once
#include <iostream>
#include <vector>
#include "Link.h"
#include "RoutingTable.h" // 1. بننادي على ملف الجدول الجديد

class Router {
public:
    int id;
    std::vector<Link*> neighbors;
    RoutingTable table; // 2. كل راوتر بقى عنده جدول خاص بيه (زي ما موجود في الـ UML)

    Router(int id) : id(id) {}

    void addNeighbor(Link* l) {
        neighbors.push_back(l);
    }

    void displayInfo() {
        std::cout << "Router " << id << " has " << neighbors.size() << " neighbors." << std::endl;
    }
};