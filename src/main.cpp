#include "../include/TJoinShortestPath.hpp"
#include "../include/Dijkstra.hpp"

using namespace lemon;

#include <iostream>
#include <lemon/full_graph.h>

int main() {
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
    w[arc_1] =  2;
    w[arc_2] = -1;
    w[arc_3] =  3;
    w[arc_4] =  1;
    w[arc_5] = -2;

    TJoinShortestPath tjoin(G, w);
    auto edges = tjoin.run(G.nodeFromId(0), G.nodeFromId(1));

    for (auto e : edges) {
        std::cout << G.id(G.u(e)) << " " << G.id(G.v(e)) << " | ";
    }
    std::cout << std::endl << tjoin.calculateJoinWeight(edges) << std::endl;

    return 0;
}
