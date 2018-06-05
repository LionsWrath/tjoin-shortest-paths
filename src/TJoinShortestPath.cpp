#include "../include/TJoinShortestPath.hpp"
#include "../include/Dijkstra.hpp"

#include <cmath>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <unordered_map>

TJoinShortestPath::TJoinShortestPath(
        Graph& g, 
        Graph::EdgeMap<long long>& weights) {

    WO = new Graph::EdgeMap<long long>(G);

    NR = new Graph::NodeMap<Graph::Node>(g);
    ER = new Graph::EdgeMap<Graph::Edge>(G);

    lemon::graphCopy(g, G)
        .edgeMap(weights, *WO)
        .nodeRef(*NR)
        .edgeCrossRef(*ER)
        .run();

    W  = new Graph::EdgeMap<long long>(G);
    N  = new Graph::EdgeMap<bool>(G, false);
    _T = new Graph::NodeMap<bool>(G, false);

    for(Graph::EdgeIt e(G); e != lemon::INVALID; ++e) {
        if ((*WO)[e] < 0) {
            (*N)[e] = true;
            (*W)[e] = std::llabs((*WO)[e]);
        } else (*W)[e] = (*WO)[e];
    }

    SubGraph sg(G, *N);

    for (SubGraph::NodeIt n(sg); n != lemon::INVALID; ++n) {
        int dg = 0;
        for (SubGraph::IncEdgeIt e(sg, n); e != lemon::INVALID; ++e) {
            dg++;
        }

        if (0 == dg) continue;
        if (1 == dg % 2) (*_T)[n] = true; 
    }

    if (this->checkNegativeWeightCycle())
        throw std::logic_error("This Graph contain a negative cycle!");
}

TJoinShortestPath::EdgeSet TJoinShortestPath::symDiff(EdgeSet l, EdgeSet r) {
    std::vector<Graph::Edge> res(l.size() + r.size());

    auto it = std::set_symmetric_difference(
            l.begin(), l.end(), 
            r.begin(), r.end(),
            res.begin());

    res.resize(it - res.begin());

    return EdgeSet(res.begin(), res.end());
}

TJoinShortestPath::NodeSet TJoinShortestPath::symDiff(NodeSet l, NodeSet r) {
    std::vector<Graph::Node> res(l.size() + r.size());

    auto it = std::set_symmetric_difference(
            l.begin(), l.end(), 
            r.begin(), r.end(),
            res.begin());

    res.resize(it - res.begin());

    return NodeSet(res.begin(), res.end());
}

TJoinShortestPath::EdgeSet TJoinShortestPath::run(
        Graph::Node u, Graph::Node v) {

    NodeSet T;
    T.insert((*NR)[u]);
    T.insert((*NR)[v]);

    NodeSet _J   = this->symDiff(T, this->nodeMapToNodeSet(*_T));
    EdgeSet join = this->calculateMinimumJoin(_J);
    EdgeSet J    = this->symDiff(join, this->edgeMapToEdgeSet(*N));

    EdgeSet res;

    std::transform(
            J.begin(), J.end(),
            std::inserter(res, res.begin()),
            [this](Graph::Edge e) -> Graph::Edge { return (*ER)[e]; });

    return res;
}

TJoinShortestPath::EdgeSet TJoinShortestPath::edgeMapToEdgeSet(
        Graph::EdgeMap<bool>& map) {

    EdgeSet cvt;

    for (Graph::EdgeIt e(G); e != lemon::INVALID; ++e) {
        if (map[e]) {
            cvt.insert(e);
        }
    }

    return cvt;
}

TJoinShortestPath::NodeSet TJoinShortestPath::nodeMapToNodeSet(
        Graph::NodeMap<bool>& map) {

    NodeSet cvt;

    for (Graph::NodeIt n(G); n != lemon::INVALID; ++n) {
        if (map[n]) {
            cvt.insert(n);
        }
    }

    return cvt;
}

long long TJoinShortestPath::calculateJoinWeight(
        TJoinShortestPath::EdgeSet join, 
        Graph::EdgeMap<long long>& w) {

    long long weight = 0;

    for (auto e : join) {
        weight += w[e]; 
    }

    return weight;
}

TJoinShortestPath::EdgeSet TJoinShortestPath::calculateMinimumJoin(
        TJoinShortestPath::NodeSet nodes) {

    // Create the complete Graph and necessary structures
    FullGraph clt(nodes.size());
    FullGraph::EdgeMap<long long> w(clt);
    FullGraph::EdgeMap<Dijkstra::Path> p(clt);

    Dijkstra dij;
    std::vector<Graph::Node> map(nodes.begin(), nodes.end());

    // Build weight map for the complete graph
    // Save the path used
    for (FullGraph::NodeIt n(clt); n != lemon::INVALID; ++n) {
        dij.run(G, *W, map[clt.id(n)]);

        for (FullGraph::IncEdgeIt e(clt, n); e != lemon::INVALID; ++e) {
            auto opp = clt.oppositeNode(n, e);
            p[e] =  dij.getPath(G, map[clt.id(opp)]);
            w[e] = -dij.distance(G, map[clt.id(opp)]);
        }
    }

    // Find the optimal perfect matching
    // Note that the edges of the graph are inversed
    MWPM match(clt, w);
    match.run();

    // Get all edges in the matching and
    // Count their number in the paths
    Graph::EdgeMap<int> count(G, 0);
    for (FullGraph::EdgeIt m(clt); m != lemon::INVALID; ++m) {
        if (match.matching(m)) {
            for (auto e : p[m]) {
                count[e]++;
            }
        } 
    }

    // Build the output set of edges
    // Only edges that appear in an odd number of paths
    // can be in the final join
    EdgeSet res;
    for (Graph::EdgeIt e(G); e != lemon::INVALID; ++e) {
        if (1 == count[e] % 2) {
            res.insert(e);
        } 
    }

    return res;
}

bool TJoinShortestPath::checkNegativeWeightCycle() {

    auto nodes = this->nodeMapToNodeSet(*_T);
    auto edges = this->edgeMapToEdgeSet(* N);

    if (edges.empty())
        return false;

    if (nodes.empty()) 
        return true;

    auto tjoin = this->symDiff(
            this->calculateMinimumJoin(nodes), 
            this->edgeMapToEdgeSet(*N));

    if (this->calculateJoinWeight(tjoin, *WO) < 0)
        return true;
    return false;
}
