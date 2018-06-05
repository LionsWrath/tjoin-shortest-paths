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
    typedef lemon::FilterEdges<Graph> SubGraph;
    typedef lemon::FullGraph FullGraph;
    typedef lemon::MaxWeightedPerfectMatching<
        FullGraph, 
        FullGraph::EdgeMap<long long>> MWPM;

public:
    typedef std::set<Graph::Edge> EdgeSet;
    typedef std::set<Graph::Node> NodeSet;

private:
    Graph G;

    Graph::NodeMap<Graph::Node> *NR;
    Graph::EdgeMap<Graph::Edge> *ER;

    Graph::EdgeMap<long long> *W;
    Graph::EdgeMap<long long> *WO;
    Graph::EdgeMap<bool> * N;
    Graph::NodeMap<bool> *_T;

    EdgeSet edgeMapToEdgeSet(Graph::EdgeMap<bool>&);
    NodeSet nodeMapToNodeSet(Graph::NodeMap<bool>&);

    EdgeSet symDiff(EdgeSet, EdgeSet);
    NodeSet symDiff(NodeSet, NodeSet);

    EdgeSet calculateMinimumJoin(NodeSet);

    bool checkNegativeWeightCycle();

public:    
    TJoinShortestPath(Graph&, Graph::EdgeMap<long long>&);

    long long calculateJoinWeight(EdgeSet, Graph::EdgeMap<long long>&);

    EdgeSet run(Graph::Node, Graph::Node);
};

#endif
