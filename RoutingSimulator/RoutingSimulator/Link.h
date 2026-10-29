#pragma once

class Router; // تعريف مسبق عشان الكود يفهم إن فيه راوتر

class Link {
public:
    Router* source;
    Router* destination;
    int cost;

    Link(Router* src, Router* dest, int c)
        : source(src), destination(dest), cost(c) {
    }
};