#include "../include/CSR.h"
#include <omp.h>


void CSR::add_edges(int from, std::vector<uint64_t> &to, std::vector<double> &w) {}

void CSR::add_edge(int from, uint64_t to, double weight) {}

void CSR::populate(std::tuple<uint64_t, uint64_t, double> *e_list)
{
    // rearranged counting sort

    // building row_ptr vector
    // count number of neighbors for each vertex
    uint64_t *count = new uint64_t[num_vertices](); // initializes all values to 0
    for (uint64_t i = 0; i < num_edges; i++)
        row_ptr[std::get<0>(e_list[i]) + 1]++;

    // row_ptr[0] = 0
    // cumulative sum

    for (uint64_t i = 1; i <= num_vertices; i++)
    {
        row_ptr[i] += row_ptr[i - 1];
        count[i - 1] = row_ptr[i];
    }
    row_ptr[num_vertices + 1] += row_ptr[num_vertices];

    // sorting col_idx and weights
    std::tuple<uint64_t, uint64_t, double> curr_edge;
    uint64_t curr_vertex;
    uint64_t new_pos;

    for (uint64_t i = num_edges - 1; i < UINT64_MAX; i--) // at the end i = UINT64_MAX
    { 
        curr_edge = e_list[i];
        curr_vertex = std::get<0>(curr_edge);
        new_pos = count[curr_vertex] - 1;

        col_idx[new_pos] = std::get<1>(curr_edge);
        weights[new_pos] = std::get<2>(curr_edge);
        count[curr_vertex]--;
    }

    delete[] count;
    finished();
}

void CSR::sortEdgesByNodeId() {}

void CSR::finished() {}

void CSR::print()
{
    std::cout << "row_ptr: { ";
    for (uint64_t i = 0; i <= num_vertices; i++)
        std::cout << row_ptr[i] << " ";
    std::cout << "}" << std::endl;

    std::cout << "col_idx: { ";
    for (uint64_t i = 0; i < num_edges; i++)
        std::cout << col_idx[i] << " ";
    std::cout << "}" << std::endl;

    std::cout << "weights: { ";
    for (uint64_t i = 0; i < num_edges; i++)
        std::cout << weights[i] << " ";
    std::cout << "}" << std::endl;
}
