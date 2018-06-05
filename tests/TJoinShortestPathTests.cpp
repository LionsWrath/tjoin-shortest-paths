#include "gtest/gtest.h"
#include "lemon/list_graph.h"
#include "../include/TJoinShortestPath.hpp"
#include "../include/Dijkstra.hpp"

TEST(TJoinTest, CorrectDistance_1) { 
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
    w[arc_1] =  2;
    w[arc_2] = -1;
    w[arc_3] =  3;
    w[arc_4] =  1;
    w[arc_5] = -2;

    TJoinShortestPath tjoinsp(G, w);

    EXPECT_EQ(-2, tjoinsp.calculateJoinWeight(tjoinsp.run(c, d), w));
    EXPECT_EQ(-2, tjoinsp.calculateJoinWeight(tjoinsp.run(a, b), w));
    EXPECT_EQ(-3, tjoinsp.calculateJoinWeight(tjoinsp.run(a, d), w));
    EXPECT_EQ(-1, tjoinsp.calculateJoinWeight(tjoinsp.run(d, b), w));
}

TEST(TJoinTest, CorrectDistance_2) { 
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

    TJoinShortestPath tjoinsp(G, w);

    EXPECT_EQ(30, tjoinsp.calculateJoinWeight(tjoinsp.run(a, b), w));
    EXPECT_EQ(25, tjoinsp.calculateJoinWeight(tjoinsp.run(a, c), w));
    EXPECT_EQ( 5, tjoinsp.calculateJoinWeight(tjoinsp.run(a, d), w));
    EXPECT_EQ(25, tjoinsp.calculateJoinWeight(tjoinsp.run(d, b), w));
}

TEST(TJoinTest, CorrectDistance_3) { 
    lemon::ListGraph G;
    lemon::ListGraph::Node a = G.addNode(); //1(0)
    lemon::ListGraph::Node b = G.addNode(); //2(1)
    lemon::ListGraph::Node c = G.addNode(); //3(2)
    lemon::ListGraph::Node d = G.addNode(); //4(3)
    lemon::ListGraph::Node e = G.addNode(); //5(4)
    lemon::ListGraph::Node f = G.addNode(); //6(5)

    lemon::ListGraph::Edge arc_1 = G.addEdge(a, b); //1 - 2
    lemon::ListGraph::Edge arc_2 = G.addEdge(a, c); //1 - 3
    lemon::ListGraph::Edge arc_3 = G.addEdge(a, f); //1 - 6
    lemon::ListGraph::Edge arc_4 = G.addEdge(b, c); //2 - 3
    lemon::ListGraph::Edge arc_5 = G.addEdge(b, d); //2 - 4
    lemon::ListGraph::Edge arc_6 = G.addEdge(c, d); //3 - 4
    lemon::ListGraph::Edge arc_7 = G.addEdge(c, f); //3 - 6
    lemon::ListGraph::Edge arc_8 = G.addEdge(d, e); //4 - 5
    lemon::ListGraph::Edge arc_9 = G.addEdge(e, f); //5 - 6

    lemon::ListGraph::EdgeMap<long long> w(G);
    w[arc_1] =   7;
    w[arc_2] =  -9;
    w[arc_3] =  14;
    w[arc_4] =  10;
    w[arc_5] =  15;
    w[arc_6] = -11;
    w[arc_7] =  -2;
    w[arc_8] =   6;
    w[arc_9] =   9;

    TJoinShortestPath tjoinsp(G, w);

    EXPECT_EQ( -5, tjoinsp.calculateJoinWeight(tjoinsp.run(a, b), w));
    EXPECT_EQ( -9, tjoinsp.calculateJoinWeight(tjoinsp.run(a, c), w));
    EXPECT_EQ(-20, tjoinsp.calculateJoinWeight(tjoinsp.run(a, d), w));
    EXPECT_EQ(-14, tjoinsp.calculateJoinWeight(tjoinsp.run(a, e), w));
    EXPECT_EQ(-11, tjoinsp.calculateJoinWeight(tjoinsp.run(a, f), w));
    EXPECT_EQ( -2, tjoinsp.calculateJoinWeight(tjoinsp.run(b, c), w));
}
TEST(TJoinTest, CorrectDistance_4) { 
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

    TJoinShortestPath tjoinsp(G, w);

    EXPECT_EQ( 7, tjoinsp.calculateJoinWeight(tjoinsp.run(a, b), w));
    EXPECT_EQ( 9, tjoinsp.calculateJoinWeight(tjoinsp.run(a, c), w));
    EXPECT_EQ(20, tjoinsp.calculateJoinWeight(tjoinsp.run(a, d), w));
    EXPECT_EQ(20, tjoinsp.calculateJoinWeight(tjoinsp.run(a, e), w));
    EXPECT_EQ(11, tjoinsp.calculateJoinWeight(tjoinsp.run(a, f), w));
}

TEST(TJoinTest, CheckNegativeCycle_1) {
    lemon::ListGraph G;
    lemon::ListGraph::Node a = G.addNode();
    lemon::ListGraph::Node b = G.addNode();
    lemon::ListGraph::Node c = G.addNode();

    lemon::ListGraph::Edge arc_1 = G.addEdge(a, b);
    lemon::ListGraph::Edge arc_2 = G.addEdge(a, c);
    lemon::ListGraph::Edge arc_3 = G.addEdge(b, c);

    lemon::ListGraph::EdgeMap<long long> w(G);
    w[arc_1] = -5;
    w[arc_2] = -5;
    w[arc_3] = -5;

    try {
        TJoinShortestPath tjoinsp(G, w);
        FAIL();
    } catch (const std::logic_error& e) {
        ASSERT_STREQ("This Graph contain a negative cycle!", e.what());
    }
}

TEST(TJoinTest, CheckNegativeCycle_2) {
    lemon::ListGraph G;
    lemon::ListGraph::Node a = G.addNode();
    lemon::ListGraph::Node b = G.addNode(); 
    lemon::ListGraph::Node c = G.addNode();
    lemon::ListGraph::Node d = G.addNode();
    lemon::ListGraph::Node e = G.addNode();

    lemon::ListGraph::Edge arc_1 = G.addEdge(a, b);
    lemon::ListGraph::Edge arc_2 = G.addEdge(b, e);
    lemon::ListGraph::Edge arc_3 = G.addEdge(b, c);
    lemon::ListGraph::Edge arc_4 = G.addEdge(e, c);
    lemon::ListGraph::Edge arc_5 = G.addEdge(c, e);

    lemon::ListGraph::EdgeMap<long long> w(G);
    w[arc_1] =  2;
    w[arc_2] = -5;
    w[arc_3] =  1;
    w[arc_4] =-10;
    w[arc_5] =  1;

    try {
        TJoinShortestPath tjoinsp(G, w);
        FAIL();
    } catch (const std::logic_error& e) {
        ASSERT_STREQ("This Graph contain a negative cycle!", e.what());
    }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
