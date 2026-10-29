#pragma once
#include "IRoutingAlgorithm.h"
#include <map>
#include <vector>
#include <string>
#include <iostream> // ضفنا دي عشان جملة الطباعة تشتغل

class DistanceVectorAlgorithm : public IRoutingAlgorithm {
public:
    std::string getName() override { return "Distance Vector (Bellman-Ford)"; }

    void buildRoutingTables(std::vector<Router*>& routers) override {

        // ==========================================
        // تعريف العدادات (Metrics)
        // ==========================================
        int convergenceSteps = 0;   // عدد اللفات لحد ما الشبكة استقرت
        int controlMessages = 0;    // عدد الرسائل المتبادلة (Overhead)

        // ==========================================
        // 1. خطوة البداية (Initialization)
        // ==========================================
        for (auto r : routers) {
            // بنمسح الجدول القديم عشان لو بنعيد المحاكاة
            r->table.entries.clear();
            // بنضيف مدخل: الهدف=أنا، الخطوة الجاية=أنا، التكلفة=0
            r->table.addEntry(r->id, r->id, 0);
        }

        // ==========================================
        // 2. اللوب الرئيسية (Bellman-Ford Logic)
        // ==========================================
        bool changed = true;

        while (changed) {
            changed = false;
            convergenceSteps++; // بنزود خطوة (لفة جديدة)

            for (auto r : routers) {

                // في كل لفة، الراوتر بيبعت جدوله لكل جيرانه
                // فبنزود عدد الرسايل بعدد الجيران
                controlMessages += r->neighbors.size();

                // كل راوتر بيبص على وصلاته وجيرانه
                for (auto link : r->neighbors) {
                    Router* neighbor = link->destination;

                    // بيشوف الطرق اللي عند جاره
                    for (auto& entry : neighbor->table.entries) {
                        int dest = entry.destinationId;
                        int new_cost = link->cost + entry.cost;

                        // لو لقى طريق أرخص للهدف ده، بيحدث جدوله
                        bool found = false;
                        for (auto& myEntry : r->table.entries) {
                            if (myEntry.destinationId == dest) {
                                found = true;
                                if (new_cost < myEntry.cost) {
                                    myEntry.cost = new_cost;
                                    myEntry.nextHopId = neighbor->id;
                                    changed = true;
                                }
                            }
                        }

                        // لو الوجهة دي مش عندي أصلاً، بضيفها
                        if (!found && dest != r->id) {
                            r->table.addEntry(dest, neighbor->id, new_cost);
                            changed = true;
                        }
                    }
                }
            }
        }

        // ==========================================
        // 3. طباعة الإحصائيات (Metrics Output)
        // ==========================================
        std::cout << "   [Metrics] Convergence Steps: " << convergenceSteps << "\n";
        std::cout << "   [Metrics] Control Messages (Overhead): " << controlMessages << "\n";
    }
};