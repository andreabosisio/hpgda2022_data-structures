#ifndef ORACLE_CONTEST_CSR_H
#define ORACLE_CONTEST_CSR_H

#include <iostream>
#include <list>
#include <vector>
#include <functional>

// Compressed Sparse Row (CSR) implementation of Graph
class CSR{

    class EdgeIter {
        
        class iterator {
        public:
            iterator(std::list<uint64_t>::iterator ptr, std::list<double>::iterator begin_w_ptr) : ptr(ptr), begin_w_ptr(begin_w_ptr) {}

            iterator operator++() {
                ++ptr;
                ++begin_w_ptr;
                return *this;
            }

            bool operator!=(const iterator &other) { return ptr != other.ptr; }

            const std::pair<uint64_t, double> &operator*() {
                current.first = *ptr;
                current.second = *begin_w_ptr;
                return current;
            };

        private:
            std::list<uint64_t>::iterator ptr;
            std::list<double>::iterator begin_w_ptr;
            std::pair<uint64_t, double> current;
        };

    private:
        std::list<uint64_t>::iterator begin_ptr, end_ptr;
        std::list<double>::iterator begin_w_ptr;
    public:
        EdgeIter(std::list<uint64_t>::iterator begin_ptr, std::list<uint64_t>::iterator end_ptr, std::list<double>::iterator begin_w_ptr) : begin_ptr(begin_ptr), end_ptr(end_ptr), begin_w_ptr(begin_w_ptr) {}

        iterator begin() const { return iterator(begin_ptr, begin_w_ptr); }

        iterator end() const { return iterator(end_ptr, begin_w_ptr); }
    };

    uint64_t num_vertices, num_edges;
    std::list<uint64_t>* edges;
    std::list<double>* weights;

    uint64_t* col_idx;
    uint64_t* row_ptr;
    double* values;

public:

    EdgeIter get_neighbors(int idx){
        return EdgeIter(begin(idx), end(idx), begin_weights(idx));
    }

    CSR(uint64_t num_vertices, uint64_t num_edges) : num_vertices(num_vertices), num_edges(num_edges){
        col_idx = new uint64_t[num_edges + 2];
        values = new double[num_edges + 2];
        row_ptr = new uint64_t[num_vertices + 2](); // initializes all pointers to 0
    }

    ~CSR(){
        delete[] col_idx;
        delete[] values;
        delete[] row_ptr;    

        //std::cout<<"AdjacencyList delete"<<std::endl;
    }

    void add_edges(int from, std::vector<uint64_t>& to, std::vector<double>& w);

    void add_edge(int from, uint64_t to, double weight = 0);

    void sortEdgesByNodeId();

    void finished();

    void populate(std::tuple<uint64_t, uint64_t, double>* e_list);

    void print();

    inline std::list<uint64_t>::iterator begin(int cur_vertex) {
        return edges[cur_vertex].begin();
    }

    inline std::list<uint64_t>::iterator end(int cur_vertex){
        return edges[cur_vertex].end();
    }

    inline std::list<double>::iterator begin_weights(int cur_vertex) {
        return weights[cur_vertex].begin();
    }
    
};


#endif //ORACLE_CONTEST_CSR_H
