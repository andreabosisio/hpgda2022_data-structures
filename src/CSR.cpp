#include "../include/CSR.h"

void CSR::add_edges(int from, std::vector<uint64_t>& to, std::vector<double>& w) {
    for(uint64_t i : to)
        edges[from].push_back(i);
    for(double i : w)
        weights[from].push_back(i);
}

void CSR::add_edge(int from, uint64_t to, double weight) {
    edges[from].push_back(to);
    weights[from].push_back(weight);
}

void CSR::populate(std::tuple<uint64_t, uint64_t, double>* e_list){

    std::sort(e_list, e_list + num_edges); // O(num_edges * log(num_edges))

    // treating as there is always a node 0
    int starting_indx = 0;
    if(std::get<0>(e_list[0]) == 1)
        starting_indx = 1;

    uint64_t i = 0;
    for(; i < num_edges; i++) {
        col_idx[i] = std::get<1>(e_list[i]);
        values[i] = std::get<2>(e_list[i]);
        row_ptr[std::get<0>(e_list[i]) + starting_indx] = i + starting_indx;
    }
    row_ptr[num_vertices + 1] = i + starting_indx;  

    //To Delete
    /* for(uint64_t i = 0; i < num_edges; i++)
        add_edge(std::get<0>(e_list[i]), std::get<1>(e_list[i]), std::get<2>(e_list[i])); */

        
    finished();
}

void CSR::sortEdgesByNodeId() {
    for(uint64_t i = 0; i <= num_vertices; ++i)
        edges[i].sort();
}

void CSR::finished() {}

void CSR::print() {
    std::cout << "row_ptr: { ";
    for (u_int64_t i = 0; i <= num_vertices + 1; i++)
        std::cout << row_ptr[i] << " ";
    std::cout << "}" << std::endl;

    std::cout << "col_idx: { ";
    for (u_int64_t i = 0; i < num_edges; i++)
        std::cout << col_idx[i] << " ";
    std::cout << "}" << std::endl;

    std::cout << "values: { ";
    for (u_int64_t i = 0; i < num_edges; i++)
        std::cout << values[i] << " ";
    std::cout << "}" << std::endl;
    
}