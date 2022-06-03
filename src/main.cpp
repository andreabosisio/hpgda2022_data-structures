#include "../include/utils.h"
#include "../include/CSR.h"
#include "../include/GraphAlgorithm.h"
#include <fstream>
#include <ostream>
#include <string>

#define NOOP ((void)0)


int main(int argc, char **argv) {
    // argv[1] -> graph name (required)
    // argv[2] -> source vertex for BFS and DFS (required)
    // argv[3] -> -U (if undirected graph, default: directed graph)
    // argv[4] -> -d (if debugging)
    // arhv[5] -> -ref (if reference implementation (adj. list))

    // variables to measure memory usage
    double vm_usage = 0.0, resident_set_size = 0.0;

    // vertex stored in graphName.v
    // edges stored in graphName.e
    std::string graphName = argv[1];

    // get source vertex from command arguments
    uint64_t src_vertex;
    if (argc <= 2){
        std::cout << "ERROR: missing required arguments!" << std::endl; 
        return 1;
    } else {
        src_vertex = std::stoi(std::string(argv[2]));
    }
    
    // default: directed graph
    // default: debugging inactive
    bool undirected = false;
    bool debug = false;
    // bool ref_implementation = false; 
    if (argc > 3){
        undirected = (std::string(argv[3]) == "-U") ? true : false; 
        debug = (std::string(argv[3]) == "-d") ? true : false; 
        if (argc > 4){
            debug = (std::string(argv[4]) == "-d") ? true : false; 
            // if (argc > 5)
                // ref_implementation = (std::string(argv[5]) == "-ref") ? true : false;
        }
    }

    // get number of edges
    uint64_t num_edges = count_lines(graphName + ".e");
    // no self-loop allowed: each undirected edge = 2 directed edges
    if (undirected) num_edges *= 2;

    // get memory usage before loading the graph
    process_mem_usage(vm_usage, resident_set_size, false);

    // temporary data structs for nodes and edges
    std::tuple<uint64_t, uint64_t, double>* edges = new std::tuple<uint64_t, uint64_t, double>[num_edges];
    std::set<uint64_t> nodes;

    // read nodes and edges
    if(debug) std::cout << "Loading the graph " << graphName << std::endl;
    nodes = load_graph(graphName, undirected, edges, num_edges);
    if(debug) std::cout << "Graph loaded!" << std::endl << std::endl;

    // get increment in memory usage after loading the graph
    process_mem_usage(vm_usage, resident_set_size, true);
    if(debug) std::cout << "Edge list size: " << resident_set_size/1024 << " MB" << std::endl << std::endl;

    // get number of nodes
    uint64_t num_vertices = nodes.size();
    
    // print graph info
    if(debug) print_graph_info(num_vertices, num_edges, undirected);

    // print edges
    //if(debug) print_edge_list(edges, num_edges);

    // get memory usage before instantiating and populating the graph
    process_mem_usage(vm_usage, resident_set_size, false);

    // instantiate the graph
    auto *graph = new GraphAlgorithm<CSR>(num_vertices,num_edges);  

    // populate the graph and measure time
    auto begin_populate = std::chrono::high_resolution_clock::now();
    graph->populate(edges);
    auto end_populate = std::chrono::high_resolution_clock::now();
    auto elapsed_populate = std::chrono::duration_cast<std::chrono::milliseconds>(end_populate - begin_populate);
    if(debug) 
        std::cout << "Graph population time: " << elapsed_populate.count() << " ms" << std::endl << std::endl;
    else 
        std::cout << elapsed_populate.count() << ",";
    
    //  get increment in memory usage after instantiating and populating the graph
    process_mem_usage(vm_usage, resident_set_size, true);
    if(debug)
        std::cout << "Graph size: " << resident_set_size/1024 << " MB" << std::endl << std::endl;
    else
        std::cout << resident_set_size/1024 << ",";

    //if(debug) print_edge_list(edges, num_edges);    

    //if(debug) graph->print();

    
    /* TODO
    
    double result = -1;
    
    // execute bfs and measure time
    auto begin_bfs = std::chrono::high_resolution_clock::now();
    result = graph->bfs(src_vertex);
    auto end_bfs = std::chrono::high_resolution_clock::now();
    auto elapsed_bfs = std::chrono::duration_cast<std::chrono::milliseconds>(end_bfs - begin_bfs);
    if(debug) {
        std::cout << "BFS execution time: " << elapsed_bfs.count() << " ms" << std::endl;
        std::cout << "BFS sum: " << result << std::endl;
        std::cout << "Writing BFS results..." << std::endl;
    } else {
        std::cout << elapsed_bfs.count() << "," << result << ",";
    }
    // write results of the BFS
    graph->write_results(graphName + ".bfs");
    if(debug) std::cout << "BFS results written in " << graphName + ".bfs" << std::endl << std::endl;

    // execute dfs and measure time
    auto begin_dfs = std::chrono::high_resolution_clock::now();
    result = graph->dfs(src_vertex);
    auto end_dfs = std::chrono::high_resolution_clock::now();
    auto elapsed_dfs = std::chrono::duration_cast<std::chrono::milliseconds>(end_dfs - begin_dfs);
    if(debug) {
        std::cout << "DFS execution time: " << elapsed_dfs.count() << " ms" << std::endl;
        std::cout << "DFS sum: " << result << std::endl;
        std::cout << "Writing DFS results..." << std::endl;
    } else {
        std::cout << elapsed_dfs.count() << "," << result << std::endl;
    }
    // write results of the BFS
    graph->write_results(graphName + ".dfs");
    if(debug) std::cout << "DFS results written in " << graphName + ".dfs" << std::endl << std::endl; */

    // free memory
    delete graph;
    delete[] edges;
    return 0;
}

