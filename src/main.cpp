#include "../include/Dijkstra.hpp"

using namespace lemon;

#include <iostream>

int main() {
    Dijkstra dijkstra;
    ListGraph G;
    ListGraph::Node a = G.addNode();
    ListGraph::Node b = G.addNode();
    ListGraph::Node c = G.addNode();
    ListGraph::Node d = G.addNode();

    ListGraph::Edge arc_1 = G.addEdge(a, b);
    ListGraph::Edge arc_2 = G.addEdge(a, c);
    ListGraph::Edge arc_3 = G.addEdge(a, d);
    ListGraph::Edge arc_4 = G.addEdge(b, d);
    ListGraph::Edge arc_5 = G.addEdge(c, d);

    ListGraph::EdgeMap<long long> w(G);
    w[arc_1] = 1;
    w[arc_2] = 1;
    w[arc_3] = 3;
    w[arc_4] = 1;
    w[arc_5] = 1;

    for (ListGraph::EdgeIt n(G); n != INVALID; ++n) {
        ListGraph::Edge edge(n);
        std::cout <<  G.id(G.u(edge)) << " <-> " << G.id(G.v(edge)) << ": " << w[edge] << std::endl;
    }

    dijkstra.run(G, w, a);
    std::cout << dijkstra.distance(G, d) << std::endl;

    return 0;
}
