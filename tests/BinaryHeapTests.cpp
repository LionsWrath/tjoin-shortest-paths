#include "gtest/gtest.h"
#include "../include/BinaryHeap.hpp"
#include "lemon/list_graph.h"

typedef std::pair<std::string, int> Node;
typedef std::pair<int, long long> NodeG;

class compare {
public:
    bool operator()(const std::pair<std::string, int>& lhs,
            const std::pair<std::string, int>& rhs) {
        return lhs.second > rhs.second;
    }        
};
 
class compare_graph {
public:
    bool operator()(const std::pair<int, long long>& lhs,
            const std::pair<int, long long>& rhs) {
        return lhs.second > rhs.second;
    }        
};

TEST(BinaryHeapTest, CorrectMin) { 
    BinaryHeap<int, std::string, compare> heap;
    heap.insert_key("1", 10);
    heap.insert_key("2", 5);
    heap.insert_key("3", 45);
    heap.insert_key("4", 15);
    heap.insert_key("5", 100);

    EXPECT_EQ(Node("2",  5), heap.extract_min());
    EXPECT_EQ(Node("1", 10), heap.extract_min());
    EXPECT_EQ(Node("4", 15), heap.extract_min());
    EXPECT_EQ(Node("3", 45), heap.extract_min());
    EXPECT_EQ(Node("5", 100), heap.extract_min());

}

TEST(BinaryHeapTest, CorrectDecrease) { 
    BinaryHeap<int, std::string, compare> heap;
    heap.insert_key("1", 10);
    heap.insert_key("2", 5);
    heap.insert_key("3", 45);
    heap.insert_key("4", 15);
    heap.insert_key("5", 100);

    heap.decrease_key("5", 2);
    heap.decrease_key("3", 10);
    heap.decrease_key("2", 1);

    EXPECT_EQ(Node("2",  1), heap.extract_min());
    EXPECT_EQ(Node("5",  2), heap.extract_min());
    EXPECT_EQ(Node("1", 10), heap.extract_min());
    EXPECT_EQ(Node("3", 10), heap.extract_min());
    EXPECT_EQ(Node("4", 15), heap.extract_min());

}

TEST(BinaryHeapTest, CorrectGraphMin) {
    lemon::ListGraph G;

    lemon::ListGraph::Node a = G.addNode();
    lemon::ListGraph::Node b = G.addNode();
    lemon::ListGraph::Node c = G.addNode();

    BinaryHeap<long long, int, compare_graph> heap;
    heap.insert_key(G.id(a), 15);
    heap.insert_key(G.id(b), 40);
    heap.insert_key(G.id(c), 5);

    EXPECT_EQ(NodeG(G.id(c),  5), heap.extract_min());
    EXPECT_EQ(NodeG(G.id(a), 15), heap.extract_min());
    EXPECT_EQ(NodeG(G.id(b), 40), heap.extract_min());
}
