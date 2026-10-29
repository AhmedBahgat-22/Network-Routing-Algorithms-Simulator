#pragma once

class RoutingEntry {
public:
    int destinationId; // الراوتر الهدف
    int nextHopId;     // الراوتر اللي هنسلمه البيانات (القفزة الجاية)
    int cost;          // تكلفة الوصول

    RoutingEntry(int dest, int next, int c)
        : destinationId(dest), nextHopId(next), cost(c) {
    }
};