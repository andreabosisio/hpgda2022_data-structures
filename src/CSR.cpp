#include "../include/CSR.h"
#include <numeric>
#include <omp.h>
#include <chrono>


void CSR::add_edges(int from, std::vector<uint64_t> &to, std::vector<double> &w) {}

void CSR::add_edge(int from, uint64_t to, double weight) {}

void CSR::populate(std::tuple<uint64_t, uint64_t, double> *e_list)
{
    // rearranged counting sort

    // counting neighbors for each vertex

    // NO REDUCTION
    uint64_t *count = new uint64_t[num_vertices];
    uint64_t *row_ptr_private;
    #pragma omp parallel
    {
        const int nthreads = omp_get_num_threads();
        const int ithread = omp_get_thread_num();

        #pragma omp single 
        {
            row_ptr_private = new uint64_t[(num_vertices+2)*nthreads];
            for(uint64_t i = 0; i < ((num_vertices+2) * nthreads); i++) row_ptr_private[i] = 0;
        }
        #pragma omp for
        for (uint64_t n = 0 ; n < num_edges ; ++n )
        {   
            row_ptr_private[ithread * (num_vertices+2) + (std::get<0>(e_list[n]) + 1)]++;
        }
        #pragma omp for
        for(uint64_t i = 1; i < num_vertices+2; i++) {
            for(int j = 0; j < nthreads; j++) {
                row_ptr[i] += row_ptr_private[(num_vertices+2)*j + i];
            }
        }
    }
    
    delete[] row_ptr_private;
/*
    // REDUCTION  (to check)
    #pragma omp parallel for reduction (+:row_ptr[:(num_vertices+2)])
    for(uint64_t i = 0; i < num_edges; i++)
        row_ptr[std::get<0>(e_list[i]) + 1]++;
*/


     // SERIAL SECOND FOR
/*
    //#pragma omp for // OMP
    for (uint64_t i = 1; i <= num_vertices; i++)
    {
        row_ptr[i] += row_ptr[i - 1];
        count[i - 1] = row_ptr[i];
    }    
    row_ptr[num_vertices + 1] += row_ptr[num_vertices];
*/    

    // PARALLEL SECOND FOR 
   
    // cumulative sum
    // row_ptr[0] = 0
    std::partial_sum(row_ptr, row_ptr + (num_vertices+2), row_ptr);

    // counting sort: sorting col_idx and weights

    // PARALLEL LAST FOR 
    // copying row_ptr into count which will be used for the counting sort and setting the locks for the count vector
    omp_lock_t count_locks[num_vertices];
    // double start_time = omp_get_wtime();
    #pragma omp parallel for 
    for (uint64_t i = 0; i < num_vertices; i++)
    {   
        count[i] = row_ptr[i+1];
        omp_init_lock(&count_locks[i]);
    }

    #pragma omp parallel 
    {   
        std::tuple<uint64_t, uint64_t, double> curr_edge;
        uint64_t curr_vertex;
        uint64_t new_pos;

        #pragma omp for 
        for (uint64_t i = 0; i < num_edges; i++) // at the end i = UINT64_MAX
        {      
            //printf("thread %d is managing index %llu -> curr_vertex %llu \n", omp_get_thread_num(), i, std::get<0>(e_list[i]));
            //#pragma omp ordered
            //{
            curr_edge = e_list[i];
            //}
            curr_vertex = std::get<0>(curr_edge);
            
            // locking curr_vertex
            omp_set_lock(&count_locks[curr_vertex]); 
              
            new_pos = count[curr_vertex] - 1;
            count[curr_vertex]--;
            //printf("thread %d is managing index %llu -> LOCK curr_vertex %llu ::: NEW POS: %llu \n", omp_get_thread_num(), i, std::get<0>(e_list[i]), new_pos);
            
            //printf("thread %d is managing index %llu -> RELEASING LOCK curr_vertex %llu \n", omp_get_thread_num(), i, std::get<0>(e_list[i]));
            // unlocking curr_vertex
            omp_unset_lock(&count_locks[curr_vertex]);

            col_idx_weight[new_pos] = std::make_pair(std::get<1>(curr_edge), std::get<2>(curr_edge));
        }
    }

    // destroying locks
    #pragma omp parallel for 
    for (uint64_t i = 0; i < num_vertices; i++)
    {
        omp_destroy_lock(&count_locks[i]);
    }
    // double time = omp_get_wtime() - start_time;
    //std::cout << "insertion time: " << time << std::endl;

/*
   // SERIAL LAST FOR

    auto begin_insertion = std::chrono::high_resolution_clock::now();
    std::tuple<uint64_t, uint64_t, double> curr_edge;
    uint64_t curr_vertex;
    uint64_t new_pos;
    for(uint64_t i = num_edges - 1; i < UINT64_MAX; i--) // at the end i = UINT64_MAX
    {      
        //printf("thread %d is managing index %llu -> curr_vertex %llu \n", omp_get_thread_num(), i, std::get<0>(e_list[i]));
        curr_edge = e_list[i];
        curr_vertex = std::get<0>(curr_edge);
        
        //omp_set_lock(&count_locks[curr_vertex]); 
        {
            new_pos = count[curr_vertex] - 1;
            count[curr_vertex]--;

            col_idx[new_pos] = std::get<1>(curr_edge);
            weights[new_pos] = std::get<2>(curr_edge);
        }
        //omp_unset_lock(&count_locks[curr_vertex]);

    }
    auto end_insertion = std::chrono::high_resolution_clock::now(); 
    auto elapsed_insertion = std::chrono::duration_cast<std::chrono::milliseconds>(end_insertion - begin_insertion);
    std::cout << "insertion time: " << elapsed_insertion.count() << std::endl; 
*/
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