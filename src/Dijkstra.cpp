#include <climits>

#include "../include/Dijkstra.hpp"

Dijkstra::Dijkstra() {}

void Dijkstra::reset() {
    parents.clear();
    distances.clear();
}

void Dijkstra::init(lemon::ListGraph& G, lemon::ListGraph::Node i) {
    this->reset();

    for (lemon::ListGraph::NodeIt n(G); n != lemon::INVALID; ++n) {
        if (G.id(i) == G.id(n)) {
            heap.insert_key(G.id(n), 0);
        } else {
            heap.insert_key(G.id(n), LLONG_MAX);
        }
    }
}

void Dijkstra::run(
        lemon::ListGraph& G, 
        lemon::ListGraph::EdgeMap<long long>& weights, 
        lemon::ListGraph::Node i) {

    this->init(G, i);

    while (!heap.is_empty()) {
        NodeW front = heap.extract_min();

        int id = front.first;
        lemon::ListGraph::Node node = G.nodeFromId(id);
        distances[id] = front.second;

        for (lemon::ListGraph::IncEdgeIt a(G, node); 
                a != lemon::INVALID; ++a) {

            lemon::ListGraph::Edge edge(a);
            int ng = G.id(G.oppositeNode(node, edge));

            if (!heap.exists(ng)) continue;
            if (distances[id] + weights[edge] < heap[ng]) {
                parents[ng] = id;
                heap.decrease_key(ng, distances[id] + weights[edge]);
            }     
        }
    }
}

long long Dijkstra::distance(lemon::ListGraph& G, lemon::ListGraph::Node to) {
    return distances[G.id(to)];
}

Dijkstra::Path Dijkstra::get_path(lemon::ListGraph& G, lemon::ListGraph::Node) {
    return Dijkstra::Path();
}
