#pragma once
#include <iostream>
#include <vector>
#include "Router.h"
#include "Packet.h"
#include "IRoutingAlgorithm.h"

class SimulationEngine {
public:
    std::vector<Router*> routers;

    // بنضيف الراوترات للمحرك عشان يقدر يتحكم فيها
    void addRouter(Router* r) {
        routers.push_back(r);
    }

    // دي أهم وظيفة: بتشغل خوارزمية معينة وتعمل اختبار
    void runExperiment(IRoutingAlgorithm* algo, int srcId, int destId) {
        std::cout << "\n========================================\n";
        std::cout << "Running Algorithm: " << algo->getName() << std::endl;
        std::cout << "========================================\n";

        // 1. تنظيف الجداول القديمة (عشان نبدأ على نظافة)
        for (auto r : routers) r->table.clear();

        // 2. بناء جداول التوجيه باستخدام الخوارزمية المختارة
        algo->buildRoutingTables(routers);

        // 3. محاكاة إرسال حزمة بيانات (Packet)
        Router* srcRouter = nullptr;
        for (auto r : routers) if (r->id == srcId) srcRouter = r;

        if (!srcRouter) {
            std::cout << "Error: Source router not found!" << std::endl;
            return;
        }

        Packet p(srcId, destId);
        std::cout << "Sending Packet from Router " << srcId << " to " << destId << "...\n";

        // بنحرك الباكيت خطوة بخطوة بناءً على الجدول
        int steps = 0;
        int maxSteps = 20; // عشان لو فيه Loop لانهائية نوقف
        bool reached = false;

        while (steps < maxSteps) {
            Router* current = nullptr;
            for (auto r : routers) if (r->id == p.currentRouterId) current = r;

            if (!current) { std::cout << "Invalid router in route\n"; break; }
            if (p.currentRouterId == destId) {
                reached = true;
                break;
            }

            // بندور في الجدول هنسلم الباكيت لمين
            bool routeFound = false;
            for (auto& entry : current->table.entries) {
                if (entry.destinationId == destId) {
                    // لقينا الطريق، نحرك الباكيت للي بعده
                    p.update(entry.nextHopId, entry.cost); // تحديث مكان الباكيت
                    routeFound = true;
                    std::cout << " -> Hop to Router " << entry.nextHopId << " (Cost: " << entry.cost << ")\n";
                    break;
                }
            }

            if (!routeFound) {
                std::cout << " -> Dead End! No route to destination.\n";
                break;
            }
            steps++;
        }

        // 4. طباعة تقرير الأداء (المقارنة)
        if (reached) {
            std::cout << "\n--- RESULTS ---\n";
            std::cout << "Status: DELIVERED\n";
            std::cout << "Total Hops: " << p.hopCount << "\n";
            std::cout << "Total Cost: " << p.totalCost << "\n";
        }
        else {
            std::cout << "\n--- RESULTS ---\n";
            std::cout << "Status: FAILED (Packet Lost)\n";
        }
    }
};