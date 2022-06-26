
#include "../include/CSR.h"
#include <vector>
#include <numeric>
#include <omp.h>
#include <chrono>
#include <algorithm>


void CSR::add_edges(int from, std::vector<uint64_t> &to, std::vector<double> &w) {}

void CSR::add_edge(int from, uint64_t to, double weight) {}

void CSR::populate(std::tuple<uint64_t, uint64_t, double> *e_list)
{
    // rearranged counting sort

    // creating a temp. adjacency list 
    std::vector<std::pair<uint64_t, double>> adj_list[num_vertices];

    omp_lock_t count_locks[num_vertices];

    double init = omp_get_wtime();

    #pragma omp parallel for 
    for (uint64_t i = 0; i < num_vertices; i++)
    {   
        adj_list[i].reserve(num_edges/num_vertices);
        omp_init_lock(&count_locks[i]);
    }

   
    #pragma omp parallel 
    {   
        std::tuple<uint64_t, uint64_t, double> curr_edge;
        uint64_t curr_vertex;
        uint64_t new_pos;

        #pragma omp for 
        for (uint64_t i = 0; i < num_edges; i++) 
        {      
            curr_edge = e_list[i];
            curr_vertex = std::get<0>(curr_edge);
            
            // locking curr_vertex
            omp_set_lock(&count_locks[curr_vertex]); 
              
            adj_list[curr_vertex].push_back(std::make_pair(std::get<1>(curr_edge), std::get<2>(curr_edge)));

            //printf("thread %d is managing index %llu -> LOCK curr_vertex %llu ::: NEW POS: %llu \n", omp_get_thread_num(), i, std::get<0>(e_list[i]), new_pos);
            
            //printf("thread %d is managing index %llu -> RELEASING LOCK curr_vertex %llu \n", omp_get_thread_num(), i, std::get<0>(e_list[i]));
            // unlocking curr_vertex
            omp_unset_lock(&count_locks[curr_vertex]);
        }
    }
    
    std::cout << "First for : " << omp_get_wtime()-init << std::endl;
    
    init = omp_get_wtime();
    #pragma omp parallel for 
    for (uint64_t i = 0; i < num_vertices; i++)
    {   
        row_ptr[i+1] = adj_list[i].size(); // count neighbors
        std::sort(adj_list[i].begin(), adj_list[i].end()); // sorting neighbors of node i to obtain always the same order

        omp_destroy_lock(&count_locks[i]); // destroying locks
    }

    std::cout << "Second for : " << omp_get_wtime()-init << std::endl;
    
/*
    for(int i=0; i<num_vertices; i++)
    {
        std::cout << "neighbors of node " << i << std::endl;
        for(auto& n : adj_list[i])
            std::cout << n.first << " ";
        std::cout << " " << std::endl;    
    }
    std::cout << "finished" << std::endl; 
*/
    auto begin_part_sum= std::chrono::high_resolution_clock::now();
    // building row_ptr with cumulate sum
    row_ptr[0] = 0;
    std::partial_sum(row_ptr, row_ptr + (num_vertices+2), row_ptr);
    auto end_part_sum = std::chrono::high_resolution_clock::now();
    std::cout << "part sum time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end_part_sum - begin_part_sum).count()  << " ms" << std::endl;
    

    init = omp_get_wtime();
    // mapping the adjacency list into the col_idx_weight array (CSR)
    #pragma omp parallel for
    for (uint64_t i = 0; i < num_vertices; i++) 
    {      
        std::copy(adj_list[i].begin(), adj_list[i].end(), col_idx_weight + row_ptr[i]);
        /*
        for (uint64_t j = 0; j < row_ptr[i+1] - row_ptr[i]; j++)  // OPTIMIZE memcpy
        {   
            col_idx_weight[j + row_ptr[i]] = adj_list[i][j];
        }
        */           

    }
    std::cout << "last for : " << omp_get_wtime()-init << std::endl;
    

    finished();
}

void CSR::populate_serial_execution(std::tuple<uint64_t, uint64_t, double> *e_list) {}


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
