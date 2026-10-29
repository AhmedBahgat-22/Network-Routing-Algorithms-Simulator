#pragma once
#include <string>
#include <vector>
#include "Router.h"

// دي الواجهة اللي كل الخوارزميات لازم تنفذها (Template)
class IRoutingAlgorithm {
public:
    virtual std::string getName() = 0; // عشان نعرف اسم الخوارزمية (مثلاً Flooding)

    // الوظيفة الأساسية لبناء جداول التوجيه
    virtual void buildRoutingTables(std::vector<Router*>& routers) = 0;

    virtual ~IRoutingAlgorithm() {} // Destructor افتراضي للأمان
};