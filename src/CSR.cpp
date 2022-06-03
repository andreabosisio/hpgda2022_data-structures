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
    // rearranged counting sort 
    uint64_t* count = new uint64_t[num_vertices](); // initializes all values to 0
    for (uint64_t i = 0; i < num_edges; i++)
        row_ptr[std::get<0>(e_list[i]) + 1]++;

    for (uint64_t i = 1; i <= num_vertices; i++){  
        row_ptr[i] += row_ptr[i - 1];
        count[i - 1] = row_ptr[i];
    }  

    std::tuple<uint64_t, uint64_t, double> curr_edge;
    uint64_t curr_vertex;
    uint64_t new_pos;
    for (uint64_t i = num_edges - 1; i < UINT64_MAX; i--) { // at the end i = UINT64_MAX
        curr_edge = e_list[i];
        curr_vertex = std::get<0>(curr_edge);
        new_pos = count[curr_vertex] - 1;

        col_idx[new_pos] = std::get<1>(curr_edge);
        values[new_pos] = std::get<2>(curr_edge);
        count[curr_vertex]--;
    }

    delete[] count;
    finished();
}

void CSR::sortEdgesByNodeId() {
    for(uint64_t i = 0; i <= num_vertices; ++i)
        edges[i].sort();
}

void CSR::finished() {}

void CSR::print() {
    std::cout << "row_ptr: { ";
    for (uint64_t i = 0; i <= num_vertices; i++)
        std::cout << row_ptr[i] << " ";
    std::cout << "}" << std::endl;

    std::cout << "col_idx: { ";
    for (uint64_t i = 0; i < num_edges; i++)
        std::cout << col_idx[i] << " ";
    std::cout << "}" << std::endl;

    std::cout << "values: { ";
    for (uint64_t i = 0; i < num_edges; i++)
        std::cout << values[i] << " ";
    std::cout << "}" << std::endl;
    
}