#include "gtest/gtest.h"
#include "lemon/list_graph.h"
#include "../include/Dijkstra.hpp"

TEST(DijkstraTest, CorrectDistance_1) { 
    lemon::ListGraph G;
    lemon::ListGraph::Node a = G.addNode();
    lemon::ListGraph::Node b = G.addNode();
    lemon::ListGraph::Node c = G.addNode();
    lemon::ListGraph::Node d = G.addNode();

    lemon::ListGraph::Edge arc_1 = G.addEdge(a, b);
    lemon::ListGraph::Edge arc_2 = G.addEdge(a, c);
    lemon::ListGraph::Edge arc_3 = G.addEdge(a, d);
    lemon::ListGraph::Edge arc_4 = G.addEdge(b, d);
    lemon::ListGraph::Edge arc_5 = G.addEdge(c, d);

    lemon::ListGraph::EdgeMap<long long> w(G);
    w[arc_1] = 1;
    w[arc_2] = 1;
    w[arc_3] = 3;
    w[arc_4] = 1;
    w[arc_5] = 1;

    Dijkstra dijkstra;
    dijkstra.run(G, w, a);

    EXPECT_EQ(1, dijkstra.distance(G, b));
    EXPECT_EQ(1, dijkstra.distance(G, c));
    EXPECT_EQ(2, dijkstra.distance(G, d));
}

TEST(DijkstraTest, CorrectDistance_2) { 
    lemon::ListGraph G;
    lemon::ListGraph::Node a = G.addNode();
    lemon::ListGraph::Node b = G.addNode();
    lemon::ListGraph::Node c = G.addNode();
    lemon::ListGraph::Node d = G.addNode();

    lemon::ListGraph::Edge arc_1 = G.addEdge(a, b);
    lemon::ListGraph::Edge arc_2 = G.addEdge(a, c);
    lemon::ListGraph::Edge arc_3 = G.addEdge(a, d);
    lemon::ListGraph::Edge arc_4 = G.addEdge(b, d);
    lemon::ListGraph::Edge arc_5 = G.addEdge(c, d);

    lemon::ListGraph::EdgeMap<long long> w(G);
    w[arc_1] = 100;
    w[arc_2] = 25;
    w[arc_3] = 5;
    w[arc_4] = 25;
    w[arc_5] = 100;

    Dijkstra dijkstra;
    dijkstra.run(G, w, a);

    EXPECT_EQ(30, dijkstra.distance(G, b));
    EXPECT_EQ(25, dijkstra.distance(G, c));
    EXPECT_EQ(5 , dijkstra.distance(G, d));
}

TEST(DijkstraTest, CorrectDistance_3) { 
    lemon::ListGraph G;
    lemon::ListGraph::Node a = G.addNode(); //1
    lemon::ListGraph::Node b = G.addNode(); //2
    lemon::ListGraph::Node c = G.addNode(); //3
    lemon::ListGraph::Node d = G.addNode(); //4
    lemon::ListGraph::Node e = G.addNode(); //5
    lemon::ListGraph::Node f = G.addNode(); //6

    lemon::ListGraph::Edge arc_1 = G.addEdge(a, b);
    lemon::ListGraph::Edge arc_2 = G.addEdge(a, c);
    lemon::ListGraph::Edge arc_3 = G.addEdge(a, f);
    lemon::ListGraph::Edge arc_4 = G.addEdge(b, c);
    lemon::ListGraph::Edge arc_5 = G.addEdge(b, d);
    lemon::ListGraph::Edge arc_6 = G.addEdge(c, d);
    lemon::ListGraph::Edge arc_7 = G.addEdge(c, f);
    lemon::ListGraph::Edge arc_8 = G.addEdge(d, e);
    lemon::ListGraph::Edge arc_9 = G.addEdge(e, f);

    lemon::ListGraph::EdgeMap<long long> w(G);
    w[arc_1] = 7;
    w[arc_2] = 9;
    w[arc_3] = 14;
    w[arc_4] = 10;
    w[arc_5] = 15;
    w[arc_6] = 11;
    w[arc_7] = 2;
    w[arc_8] = 6;
    w[arc_9] = 9;

    Dijkstra dijkstra;
    dijkstra.run(G, w, a);

    EXPECT_EQ( 7, dijkstra.distance(G, b));
    EXPECT_EQ( 9, dijkstra.distance(G, c));
    EXPECT_EQ(20, dijkstra.distance(G, d));
    EXPECT_EQ(20, dijkstra.distance(G, e));
    EXPECT_EQ(11, dijkstra.distance(G, f));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
