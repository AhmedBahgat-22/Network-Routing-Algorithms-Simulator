
#pragma once

#include "IRoutingAlgorithm.h"
#include <map>
#include <set>
#include <limits>

class DijkstraAlgorithm : public IRoutingAlgorithm {
public:
    std::string getName() override {
        return "Shortest Path First (Dijkstra)";
    }

    void buildRoutingTables(std::vector<Router*>& routers) override {
        constexpr int INF = (std::numeric_limits<int>::max)();
        for (auto source : routers) {
            source->table.clear();

            std::map<int, int> min_cost;
            std::map<int, int> next_hop;
            std::map<int, Router*> router_by_id;

            for (auto r : routers) {
                min_cost[r->id] = INF;
                router_by_id[r->id] = r;
            }

            min_cost[source->id] = 0;

            std::set<std::pair<int, int>> active_nodes;
            active_nodes.insert({ 0, source->id });

            while (!active_nodes.empty()) {
                auto current = *active_nodes.begin();
                active_nodes.erase(active_nodes.begin());

                int curr_dist = current.first;
                int curr_id = current.second;

                if (curr_dist != min_cost[curr_id])
                    continue;

                Router* curr_router = router_by_id.at(curr_id);

                for (auto link : curr_router->neighbors) {
                    int neighbor_id = link->destination->id;
                    int weight = link->cost;

                    if (weight < 0)
                        continue;

                    if (min_cost.find(neighbor_id) == min_cost.end())
                        continue;

                    if (curr_dist > INF - weight)
                        continue;

                    int new_dist = curr_dist + weight;

                    if (new_dist < min_cost[neighbor_id]) {
                        active_nodes.erase({
                            min_cost[neighbor_id], neighbor_id
                            });

                        min_cost[neighbor_id] = new_dist;

                        next_hop[neighbor_id] =
                            (curr_id == source->id)
                            ? neighbor_id
                            : next_hop.at(curr_id);

                        active_nodes.insert({
                            new_dist, neighbor_id
                            });
                    }
                }
            }

            for (const auto& [dest_id, cost] : min_cost) {
                if (dest_id != source->id &&
                    cost != INF &&
                    next_hop.count(dest_id)) {

                    source->table.addEntry(
                        dest_id,
                        next_hop.at(dest_id),
                        cost
                    );
                }
            }
        }
    }
};
