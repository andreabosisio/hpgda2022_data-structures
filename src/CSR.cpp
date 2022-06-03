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

    row_ptr[0] = 0;

/*     uint64_t i = 0;
    uint16_t curr_vertex = std::get<0>(e_list[0]), next_vertex = curr_vertex;

    if(curr_vertex == 1)
        row_ptr[1] = 0; 
    
    bool zero_row = false;
    for(; i < num_edges; i++) {
        col_idx[i] = std::get<1>(e_list[i]);
        values[i] = std::get<2>(e_list[i]);   
        next_vertex = std::get<0>(e_list[i+1]);

        if(next_vertex != curr_vertex) {
            if(next_vertex == curr_vertex + 1) {
                row_ptr[next_vertex] = i+1; 
                if(zero_row) {
                    row_ptr[curr_vertex - 1] = row_ptr[curr_vertex];
                    zero_row = false;
                }
                curr_vertex = next_vertex;
            }
            else
                zero_row = true;
                curr_vertex = next_vertex;
        }    
    }
    row_ptr[num_vertices + 1] = i;   */

/*     if(std::get<0>(e_list[0]) == 1)
        row_ptr[1] = 0;
    uint16_t curr_vertex = std::get<0>(e_list[0]), next_vertex = curr_vertex;      
    int neighbors_count;  
    for (uint64_t i = 0; i < num_edges; i++) {
        neighbors_count = 0;
        while(curr_vertex == next_vertex){
            col_idx[i] = std::get<1>(e_list[i]);
            values[i] = std::get<2>(e_list[i]); 
            neighbors_count++;
            i++;
            curr_vertex = next_vertex;
            next_vertex = std::get<0>(e_list[i]);
        }
        if(next_vertex == curr_vertex + 1)
            row_ptr[next_vertex] = row_ptr[curr_vertex] + neighbors_count;
        else
            row_ptr[next_vertex] = row_ptr[curr_vertex];
        curr_vertex = next_vertex;        

    } */

    for (uint64_t i = 0; i <= num_vertices; i++)
        row_ptr[i] = 0;    

    std::tuple<uint64_t, uint64_t, double> curr_edge;
    for (uint64_t i = 0; i < num_edges; i++) {
        curr_edge = e_list[i];
        col_idx[i] = std::get<1>(curr_edge);
        values[i] = std::get<2>(curr_edge);
        row_ptr[std::get<0>(curr_edge) + 1] += 1;
    }

    for (uint64_t i = 1; i <= num_vertices; i++)
        row_ptr[i] = row_ptr[i-1] + row_ptr[i];
    
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