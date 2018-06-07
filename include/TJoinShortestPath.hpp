#ifndef _T_JOIN_SHORTEST_PATH_HPP
#define _T_JOIN_SHORTEST_PATH_HPP

#include <set>
#include <lemon/adaptors.h>
#include <lemon/matching.h> 
#include <lemon/list_graph.h>
#include <lemon/full_graph.h>

class TJoinShortestPath {
private:
    typedef lemon::ListGraph Graph;
    typedef lemon::FilterEdges<const Graph> SubGraph;
    typedef lemon::FullGraph FullGraph;
    typedef long long Value;
    typedef lemon::MaxWeightedPerfectMatching<FullGraph, 
            FullGraph::EdgeMap<Value>> MWPM;

public:
    typedef std::set<Graph::Edge> EdgeSet;
    typedef std::set<Graph::Node> NodeSet;

private:
    const Graph& G;
    const Graph::EdgeMap<Value>& WO;

    Graph::EdgeMap<Value> *W;
    Graph::EdgeMap<bool>  * N;
    Graph::NodeMap<bool>  *_T;

    void init();

    EdgeSet edgeMapToEdgeSet(Graph::EdgeMap<bool>&);
    NodeSet nodeMapToNodeSet(Graph::NodeMap<bool>&);

    EdgeSet symDiff(EdgeSet, EdgeSet);
    NodeSet symDiff(NodeSet, NodeSet);

    EdgeSet calculateMinimumJoin(NodeSet);
    bool checkNegativeWeightCycle();

public:    
    TJoinShortestPath(const Graph&, const Graph::EdgeMap<Value>&);
    Value calculateJoinWeight(EdgeSet);
    EdgeSet run(Graph::Node, Graph::Node);
};

#endif
