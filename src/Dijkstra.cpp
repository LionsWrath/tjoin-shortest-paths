#include "../include/Dijkstra.hpp"

#include <climits>

Dijkstra::Dijkstra() {}

void Dijkstra::reset() {
    parents.clear();
    distances.clear();
}

void Dijkstra::init(const Graph& G, Graph::Node i) {
    this->reset();

    for (Graph::NodeIt n(G); n != lemon::INVALID; ++n) {
        if (G.id(i) == G.id(n)) {
            heap.insert_key(G.id(n), 0);
        } else {
            heap.insert_key(G.id(n), LLONG_MAX);
        }
    }
}

void Dijkstra::run(const Graph& G, Graph::EdgeMap<Value>& weights, 
        Graph::Node i) {

    this->init(G, i);

    while (!heap.is_empty()) {
        NodeW front = heap.extract_min();

        int id = front.first;
        Graph::Node node = G.nodeFromId(id);
        distances[id] = front.second;

        for (Graph::IncEdgeIt a(G, node); 
                a != lemon::INVALID; ++a) {

            Graph::Edge edge(a);
            int ng = G.id(G.oppositeNode(node, edge));

            if (!heap.exists(ng)) continue;
            if (distances[id] + weights[edge] < heap[ng]) {
                parents[ng] = edge;
                heap.decrease_key(ng, distances[id] + weights[edge]);
            }     
        }
    }
}

Dijkstra::Value Dijkstra::distance(const Graph& G, Graph::Node to) {
    return distances[G.id(to)];
}

Dijkstra::Path Dijkstra::getPath(const Graph& G, Graph::Node node) {
    
    Path path;

    int idx = G.id(node);

    while (parents.find(idx) != parents.end()) {
        path.push_front(parents[idx]);
        idx = G.id(G.oppositeNode(G.nodeFromId(idx), parents[idx]));
    }    

    return path;
}
