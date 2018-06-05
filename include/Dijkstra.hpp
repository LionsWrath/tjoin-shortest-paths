#ifndef _DIJKSTRA_HPP
#define _DIJKSTRA_HPP

#include <lemon/list_graph.h>
#include <unordered_map>
#include <deque>

#include "BinaryHeap.hpp"

class Dijkstra {
    typedef std::pair<int, long long> NodeW;

    class compare {
    public:
        bool operator()(const NodeW& lhs, 
                const NodeW& rhs) {
            return lhs.second > rhs.second;
        }
    };

    std::unordered_map<int, lemon::ListGraph::Edge> parents;
    std::unordered_map<int, long long> distances;
    BinaryHeap<long long, int, compare> heap;

    void init(lemon::ListGraph&, lemon::ListGraph::Node);

public:
    typedef std::deque<lemon::ListGraph::Edge> Path;

    Dijkstra();

    void reset();

    void run(lemon::ListGraph&, 
            lemon::ListGraph::EdgeMap<long long>&, 
            lemon::ListGraph::Node);

    long long distance(lemon::ListGraph&, lemon::ListGraph::Node);

    Path getPath(lemon::ListGraph&, lemon::ListGraph::Node);
};

#endif
