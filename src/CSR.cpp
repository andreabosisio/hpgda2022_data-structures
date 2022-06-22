#include "../include/CSR.h"
#include <numeric>
#include <algorithm>

void CSR::add_edges(int from, std::vector<uint64_t> &to, std::vector<double> &w) {}

void CSR::add_edge(int from, uint64_t to, double weight) {}

void CSR::populate(std::tuple<uint64_t, uint64_t, double> *e_list)
{
    // REARRANGED COUNTING SORT

    // counting neighbors for each vertex
    uint64_t *count = new uint64_t[num_vertices];

    for (uint64_t n = 0; n < num_edges; ++n)
        row_ptr[std::get<0>(e_list[n]) + 1]++;

    // cumulative sum
    // counting sort: sorting col_idx and weights
    // copying row_ptr into count which will be used for the counting sort

    for (uint64_t i = 1; i <= num_vertices; i++)
    {
        row_ptr[i] += row_ptr[i - 1];
        count[i - 1] = row_ptr[i];
    }
    row_ptr[num_vertices + 1] += row_ptr[num_vertices];

    std::tuple<uint64_t, uint64_t, double> curr_edge;
    uint64_t curr_vertex;
    uint64_t new_pos;

    for (uint64_t i = 0; i < num_edges; i++)
    {

        curr_edge = e_list[i];
        curr_vertex = std::get<0>(curr_edge);

        new_pos = count[curr_vertex] - 1;
        count[curr_vertex]--;

        col_idx_weight[new_pos] = std::make_pair(std::get<1>(curr_edge), std::get<2>(curr_edge));
    }

    // sorting neighbors (sorting col_idx_weight by its first element)
    for (uint64_t i = 0; i < num_vertices; i++)
        std::sort(col_idx_weight + row_ptr[i], col_idx_weight + row_ptr[i + 1]);

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
        std::cout << col_idx_weight[i].first << " ";
    std::cout << "}" << std::endl;

    std::cout << "weights: { ";
    for (uint64_t i = 0; i < num_edges; i++)
        std::cout << col_idx_weight[i].second << " ";
    std::cout << "}" << std::endl;
}