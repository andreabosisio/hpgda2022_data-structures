#include "../include/CSR.h"
#include <omp.h>

/*
void CSR::add_edges(int from, std::vector<uint64_t> &to, std::vector<double> &w)
{
    for (uint64_t i : to)
        edges[from].push_back(i);
    for (double i : w)
        weights[from].push_back(i);
}
void CSR::add_edge(int from, uint64_t to, double weight)
{
    edges[from].push_back(to);
    weights[from].push_back(weight);
}
*/
void CSR::populate(std::tuple<uint64_t, uint64_t, double> *e_list)
{
    // rearranged counting sort
    // building row_ptr vector
    // count number of neighbors for each vertex
    uint64_t *count = new uint64_t[num_vertices](); // initializes all values to 0

#pragma omp parallel for reduction(+ \
                                   : row_ptr[:num_vertices + 2])
    for (uint64_t i = 0; i < num_edges; i++)
    {
        row_ptr[std::get<0>(e_list[i]) + 1]++;
        // creare numero di thread row_ptr e sommarli per ogni indice   ->  (suddividere (numero di nodi/ numero thread))
        // row_ptr1[idx]+row_ptr2[idx]+...+row_ptr_trhead[idx]
        // row_ptr[0] = 0
        // cumulative sum
        // printf("thread %d is managing index %llu -> curr_vertex %llu \n", omp_get_thread_num(), i, std::get<0>(e_list[i]));
    }
    for (uint64_t i = 1; i <= num_vertices; i++)
    {
        row_ptr[i] += row_ptr[i - 1];
        count[i - 1] = row_ptr[i];
    }
    row_ptr[num_vertices + 1] += row_ptr[num_vertices];

    // dividere row_ptr in più parti

    // sorting col_idx and weights

#pragma omp parallel for ordered schedule(dynamic) // OMP

    for (uint64_t i = num_edges - 1; i < UINT64_MAX; i--) // at the end i = UINT64_MAX
    // for (uint64_t i = 0; i < num_edges; i++)
    {
        // printf("thread %d is managing index %llu -> curr_vertex %llu", omp_get_thread_num(), i, num_edges);
        std::tuple<uint64_t, uint64_t, double> curr_edge = e_list[i];
        uint64_t curr_vertex = std::get<0>(curr_edge);
        uint64_t new_pos = 0;
#pragma omp critical
        {

            new_pos = count[curr_vertex] - 1;
            count[curr_vertex]--;
            // printf("thread %d is managing index %llu -> curr_vertex: %llu new_pos %llu\n", omp_get_thread_num(), i, curr_vertex, new_pos);
            col_idx[new_pos] = std::get<1>(curr_edge);
            weights[new_pos] = std::get<2>(curr_edge);
        }

        // atomica(semaforo), condividere count con tutti thread
    }
    delete[] count; // needed?
    finished();
}
/*
void CSR::sortEdgesByNodeId()
{
    for (uint64_t i = 0; i <= num_vertices; ++i)
        edges[i].sort();
}
*/
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