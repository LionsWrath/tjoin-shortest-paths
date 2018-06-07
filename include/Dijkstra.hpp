#ifndef _DIJKSTRA_HPP
#define _DIJKSTRA_HPP

#include <lemon/list_graph.h>
#include <unordered_map>
#include <deque>

#include "BinaryHeap.hpp"

class Dijkstra {
    typedef lemon::ListGraph Graph;
    typedef long long Value;
    typedef std::pair<int, Value> NodeW;

    class compare {
    public:
        bool operator()(const NodeW& lhs, 
                const NodeW& rhs) {
            return lhs.second > rhs.second;
        }
    };

    std::unordered_map<int, Graph::Edge> parents;
    std::unordered_map<int, Value> distances;
    BinaryHeap<Value, int, compare> heap;

    void init(Graph&, Graph::Node);

public:
    typedef std::deque<Graph::Edge> Path;

    Dijkstra();

    void reset();

    void run(Graph&, Graph::EdgeMap<Value>&, Graph::Node);

    Value distance(Graph&, Graph::Node);

    Path getPath(Graph&, Graph::Node);
};

#endif
