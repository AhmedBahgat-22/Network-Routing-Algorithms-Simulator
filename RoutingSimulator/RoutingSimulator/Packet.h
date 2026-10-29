#pragma once
#include <vector>

class Packet {
public:
    int srcId;
    int destId;
    int currentRouterId;    // ده المتغير اللي كان ناقص وعمل المشكلة
    int hopCount;
    int totalCost;
    std::vector<int> path;

    Packet(int src, int dest)
        : srcId(src), destId(dest), currentRouterId(src), hopCount(0), totalCost(0) {
        path.push_back(src); // بنسجل نقطة البداية
    }

    // دالة لتحديث مكان الباكيت لما تنتقل لراوتر جديد
    void update(int nextRouterId, int stepCost) {
        currentRouterId = nextRouterId; // تحديث المكان الحالي
        path.push_back(nextRouterId);
        hopCount++;
        totalCost += stepCost;
    }
};