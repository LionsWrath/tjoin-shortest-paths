#include "../include/TJoinShortestPath.hpp"
#include "../include/Dijkstra.hpp"

#include <cmath>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <unordered_map>

// Estimate on complexity:
//      On construction:        O(|U|^3 log |U|)
//      For each call on run(): O(|J"|^3 log |J"|)

// O(|U|^3 log |U|)
TJoinShortestPath::TJoinShortestPath(
        const Graph& g, 
        const Graph::EdgeMap<Value>& weights) : G(g), WO(weights) {

    // Remover esse g
    this->init();

    // Transform the original weight mapping to the absolute version
    // Construct N
    // O(|E|)
    for(Graph::EdgeIt e(G); e != lemon::INVALID; ++e) {
        if (WO[e] < 0) {
            (*N)[e] = true;
            (*W)[e] = std::llabs(WO[e]);
        } else (*W)[e] = WO[e];
    }

    // Construct the T' using the G[N]
    // O(|E(G[N])|) -> O(|E|)
    SubGraph sg(G, *N);

    for (SubGraph::NodeIt n(sg); n != lemon::INVALID; ++n) {
        int dg = 0;
        for (SubGraph::IncEdgeIt e(sg, n); e != lemon::INVALID; ++e) {
            dg++;
        }

        if (0 == dg) continue;
        if (1 == dg % 2) (*_T)[n] = true; 
    }

    // Construct an {}-join to check negative weight cycle
    // O(|U|^3 log |U|)
    if (this->checkNegativeWeightCycle())
        throw std::logic_error("This Graph contain a negative cycle!");
}

// O(|E|)
void TJoinShortestPath::init() {
    W  = new Graph::EdgeMap<Value>(G);
    N  = new Graph::EdgeMap<bool>(G, false);
    _T = new Graph::NodeMap<bool>(G, false);
}

// O(|E|)
TJoinShortestPath::EdgeSet TJoinShortestPath::symDiff(EdgeSet l, EdgeSet r) {
    std::vector<Graph::Edge> res(l.size() + r.size());

    auto it = std::set_symmetric_difference(
            l.begin(), l.end(), 
            r.begin(), r.end(),
            res.begin());

    res.resize(it - res.begin());

    return EdgeSet(res.begin(), res.end());
}

// O(|V|)
TJoinShortestPath::NodeSet TJoinShortestPath::symDiff(NodeSet l, NodeSet r) {
    std::vector<Graph::Node> res(l.size() + r.size());

    auto it = std::set_symmetric_difference(
            l.begin(), l.end(), 
            r.begin(), r.end(),
            res.begin());

    res.resize(it - res.begin());

    return NodeSet(res.begin(), res.end());
}

// O(|J"|^3 log |J"|)
TJoinShortestPath::EdgeSet TJoinShortestPath::run(
        Graph::Node u, Graph::Node v) {

    NodeSet T;
    T.insert(u); // O(log |T|)
    T.insert(v); // O(log |T|)

    // O(|V| + |J"|^3 log |J"| + |E|)
    NodeSet _J   = this->symDiff(T, this->nodeMapToNodeSet(*_T));
    EdgeSet join = this->calculateMinimumJoin(_J);
    EdgeSet J    = this->symDiff(join, this->edgeMapToEdgeSet(*N));

    return J;
}

// O(|E|)
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

// O(|V|)
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

// O(|E|)
TJoinShortestPath::Value TJoinShortestPath::calculateJoinWeight(
        TJoinShortestPath::EdgeSet join) {

    Value weight = 0;

    for (auto e : join) {
        weight += WO[e]; 
    }

    return weight;
}

// O(|T|^3 log |T|)
TJoinShortestPath::EdgeSet TJoinShortestPath::calculateMinimumJoin(
        TJoinShortestPath::NodeSet nodes) {

    // Create the complete Graph and necessary structures
    // E = O(|T|^2)
    FullGraph clt(nodes.size());
    FullGraph::EdgeMap<Value> w(clt);
    FullGraph::EdgeMap<Dijkstra::Path> p(clt);

    Dijkstra dij;
    std::vector<Graph::Node> map(nodes.begin(), nodes.end());

    // Build weight map for the complete graph
    // Save the path used
    // O(|T|^3 log |T|)
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
    // O(|T|*|T|^2* log |T|) = O(|T|^3 log |T|)
    MWPM match(clt, w);
    match.run();

    // Get all edges in the matching and
    // Count their number in the paths
    // O(|T|)
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
    // O(|E|)
    EdgeSet res;
    for (Graph::EdgeIt e(G); e != lemon::INVALID; ++e) {
        if (1 == count[e] % 2) {
            res.insert(e);
        } 
    }

    return res;
}

// U = T delta N
// O(|U|^3 log |U|)
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

    if (this->calculateJoinWeight(tjoin) < 0)
        return true;
    return false;
}
