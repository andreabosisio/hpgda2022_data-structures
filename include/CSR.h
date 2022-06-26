#ifndef ORACLE_CONTEST_CSR_H
#define ORACLE_CONTEST_CSR_H

#include <iostream>
#include <vector>
#include <functional>
#include <iterator>
#include <array>

// Compressed Sparse Row (CSR) implementation 
class CSR
{
    class CSRIter
    {
        class iterator
        {
        public:
            iterator(std::pair<uint64_t, double> *ptr) : ptr(ptr) {}

            iterator operator++()
            {
                ++ptr;
                return *this;
            }

            bool operator!=(const iterator &other) { return ptr != other.ptr; }

            const std::pair<uint64_t, double> &operator*()
            {
                return *ptr;
            };

        private:
            std::pair<uint64_t, double> *ptr;
        };

    private:
        std::pair<uint64_t, double> *begin_ptr;
        std::pair<uint64_t, double> *end_ptr;

    public:
        CSRIter(std::pair<uint64_t, double> *begin_ptr, std::pair<uint64_t, double> *end_ptr) : begin_ptr(begin_ptr), end_ptr(end_ptr) {}

        iterator begin() const { return iterator(begin_ptr); }

        iterator end() const { return iterator(end_ptr); }
    };

    uint64_t num_vertices, num_edges;
    uint64_t *row_ptr;
    std::pair<uint64_t, double> *col_idx_weight;
/*
private:
    void sortNeighborsOf(uint64_t vertex_idx);   
*/    

public:

    CSRIter get_neighbors(uint64_t vertex_idx)
    {   
        return CSRIter(col_idx_weight + row_ptr[vertex_idx], col_idx_weight + row_ptr[vertex_idx + 1]); 
    }

    CSR(uint64_t num_vertices, uint64_t num_edges) : num_vertices(num_vertices), num_edges(num_edges)
    {   
        col_idx_weight = new std::pair<uint64_t, double>[num_edges + 2];
        row_ptr = new uint64_t[num_vertices + 2](); 
    }

    ~CSR()
    {
        delete[] col_idx_weight;
        delete[] row_ptr;
    }

    void add_edges(int from, std::vector<uint64_t> &to, std::vector<double> &w);

    void add_edge(int from, uint64_t to, double weight = 0);

    void sortEdgesByNodeId();

    void finished();

    void populate(std::tuple<uint64_t, uint64_t, double> *e_list);
    void populate_serial_execution(std::tuple<uint64_t, uint64_t, double> *e_list);

    void print();
};

#endif // ORACLE_CONTEST_CSR_H
