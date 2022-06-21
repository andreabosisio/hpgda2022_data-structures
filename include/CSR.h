#ifndef ORACLE_CONTEST_CSR_H
#define ORACLE_CONTEST_CSR_H

#include <iostream>
#include <list>
#include <vector>
#include <functional>

#include <iterator>
#include <array>

// Compressed Sparse Row (CSR) implementation of Graph
class CSR
{
    class EdgeIter
    {

        class iterator
        {
        public:
            iterator(uint64_t *ptr, double *begin_w_ptr) : ptr(ptr), begin_w_ptr(begin_w_ptr) {}

            iterator operator++()
            {
                ++ptr;
                ++begin_w_ptr;
                return *this;
            }

            bool operator!=(const iterator &other) { return ptr != other.ptr; }

            const std::pair<uint64_t, double> &operator*()
            {
                current.first = *ptr;
                current.second = *begin_w_ptr;
                return current;
            };

        private:
            uint64_t *ptr;
            double *begin_w_ptr;
            std::pair<uint64_t, double> current;
        };

    private:
        uint64_t *begin_ptr;
        uint64_t *end_ptr;
        double *begin_w_ptr;

    public:
        EdgeIter(uint64_t *begin_ptr, uint64_t *end_ptr, double *begin_w_ptr) : begin_ptr(begin_ptr), end_ptr(end_ptr), begin_w_ptr(begin_w_ptr) {}

        iterator begin() const { return iterator(begin_ptr, begin_w_ptr); }

        iterator end() const { return iterator(end_ptr, begin_w_ptr); }
    };
    /*
    std::list<uint64_t> *edges;
    std::list<double> *weights;
    */
    uint64_t num_vertices, num_edges;
    uint64_t *col_idx;
    uint64_t *row_ptr;
    double *weights;

public:
    EdgeIter get_neighbors(uint64_t idx)
    {   
        uint64_t pos = row_ptr[idx];
        return EdgeIter(&col_idx[pos], &col_idx[row_ptr[idx + 1]], &weights[pos]);
    }
    /*
        std::vector<std::tuple<uint64_t, double>> get_neighbors(uint64_t idx)
        {
            uint64_t num_neighbors = row_ptr[idx + 1] - row_ptr[idx];
            std::vector<std::tuple<uint64_t, double>> neighbornsForIdx(num_neighbors);

            uint64_t init_pos = row_ptr[idx];
            for (int i = 0; i < num_neighbors; i++)
            {
                neighbornsForIdx[i] = std::make_tuple(col_idx[init_pos + i], weights[init_pos + i]);
            }
            return neighbornsForIdx;
        }
        */

    CSR(uint64_t num_vertices, uint64_t num_edges) : num_vertices(num_vertices), num_edges(num_edges)
    {
        col_idx = new uint64_t[num_edges + 2];
        weights = new double[num_edges + 2];
        row_ptr = new uint64_t[num_vertices + 2](); // initializes all pointers to 0
    }

    ~CSR()
    {
        delete[] col_idx;
        delete[] weights;
        delete[] row_ptr;

        // std::cout<<"AdjacencyList delete"<<std::endl;
    }

    void add_edges(int from, std::vector<uint64_t> &to, std::vector<double> &w);

    void add_edge(int from, uint64_t to, double weight = 0);

    void sortEdgesByNodeId();

    void finished();

    void populate(std::tuple<uint64_t, uint64_t, double> *e_list);

    void print();
};

#endif // ORACLE_CONTEST_CSR_H
