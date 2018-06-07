#include "../include/TJoinShortestPath.hpp"
#include "../include/Dijkstra.hpp"

#include <stdexcept>
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <string>
#include <chrono>

using namespace lemon;
using namespace std::chrono;

std::vector<ListGraph::Node> readGraph(ListGraph& G, 
        ListGraph::EdgeMap<long long>& wmap, 
        std::string input_file) {

    std::ifstream infile(input_file);
    std::string line;

    std::vector<ListGraph::Node>   nodes;
    std::vector<std::vector<bool>> edges;

    if (infile.fail())
        throw std::invalid_argument("Something wrong with the file?");

    while (std::getline(infile, line)) {
	std::istringstream iss(line);

        std::string a, b;
        long long u, v, w;

	if (!(iss >> a)) { break; }

        if (a == "c") continue;

        if (a == "p") {
            if (!(iss >> b >> u >> v)) { break; }
            
            G.reserveNode(u);
            G.reserveEdge(v);

            edges.resize(u, std::vector<bool>(u, false));

            for(int i=0; i<u; i++) 
                nodes.push_back(G.addNode());
        }

        if (a == "a") {
            if (!(iss >> u >> v >> w)) { break; }
            if (!edges[u][v]) {
                auto e = G.addEdge(nodes[u], nodes[v]);
                wmap[e] = w;

                edges[u][v] = true;
                edges[v][u] = true;
            }
        }
    }

    std::cout << "Number of Nodes: " << countNodes(G) << std::endl;
    std::cout << "Number of Edges: " << countEdges(G) << std::endl;

    return nodes;
}

void readTests(std::vector<std::pair<int, int>>& tests, 
        std::string tests_file) {

    std::ifstream infile(tests_file);
    std::string line;

    if (infile.fail())
        throw std::invalid_argument("Something wrong with the file?");

    while (std::getline(infile, line)) {
	std::istringstream iss(line);

        long long u, v;

	if (!(iss >> u >> v)) { break; }

        tests.push_back(std::make_pair(u, v));
    }
}

void parseArguments(int& argc, char **argv, 
        std::string& input, std::string& tests) {
    int c;

    opterr = 0;

    while ((c = getopt (argc, argv, "i:t:")) != -1)
        switch (c) {
            case 'i':
                input = optarg;
                break;
            case 't':
                tests = optarg;
                break;
            case '?':
                if (optopt == 'i' || optopt == 't')
                    throw std::invalid_argument(
                            std::string("Argument Missing: ") + (char)optopt);

                throw std::invalid_argument(
                        std::string("Unknown option: ") + (char)optopt);
        }

    if (input.empty())
        throw std::invalid_argument("The input file is necessary");

    if (tests.empty())
        throw std::invalid_argument("The tests file is necessary");
}

int main(int argc, char **argv) {
    ListGraph G;
    ListGraph::EdgeMap<long long> weights(G);

    std::string input_file;
    std::string tests_file;

    std::vector<ListGraph::Node>     nodes;
    std::vector<std::pair<int, int>> tests;

    try {
        parseArguments(argc, argv, input_file, tests_file);
        nodes = readGraph(G, weights, input_file);
        readTests(tests, tests_file);
    } catch(const std::invalid_argument& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    high_resolution_clock::time_point tb;
    high_resolution_clock::time_point tf;

    std::vector<unsigned long long> times;

    try {
        tb = high_resolution_clock::now();
        TJoinShortestPath sp(G, weights);
        tf = high_resolution_clock::now();

        times.push_back(duration_cast<microseconds>(tf-tb).count());
        
        for (auto t : tests) {
            tb = high_resolution_clock::now();
            sp.run(nodes[t.first], nodes[t.second]);
            tf = high_resolution_clock::now();

            times.push_back(duration_cast<microseconds>(tf-tb).count());
        }
    } catch(const std::logic_error& e) {
        std::cerr << "GRAPH ERROR: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch(const std::exception& e) {
        std::cerr << "UNKNOWN ERROR: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << std::endl;
    std::cout << "Time for construction: " << times[0] << "us" << std::endl; 
    std::cout << "Time for each test: " << std::endl;

    for (int i=1; i<times.size(); i++) {
        std::cout << "     Test " << i << ": " << times[i] << "us" << std::endl;
    }

    return 0;
}
