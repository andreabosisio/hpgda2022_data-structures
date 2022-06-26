#ifndef ORACLE_CONTEST_UTILS_H
#define ORACLE_CONTEST_UTILS_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream> 
#include <vector>
#include <boost/bimap.hpp>
#include <set>
#include <chrono>
#include <algorithm>
#include <functional>
#include <unistd.h>

// count non empty lines in a file
uint64_t count_lines(std::string filename){
    std::ifstream inFile(filename);
    uint64_t numLines = 0;
    std::string line;
    while (std::getline(inFile, line)) {
        if (!line.empty())
            numLines++;
    }
    return numLines;
}

// split a string in a vector of strings, given a separator char
std::vector<std::string> split(const std::string &s, char delim){
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        result.push_back(item);
    }
    return result;
}

// load the graph
boost::bimap<uint64_t, uint64_t> load_graph(std::string filename, bool undirected, std::tuple<uint64_t, uint64_t, double>* edges, uint64_t num_edges){
    std::ifstream eFile(filename+".e");
    std::ifstream vFile(filename+".v");
    std::string line;    
    std::vector<std::string> tmp;

    while (std::getline(eFile, line)){
        if (!line.empty()){
            tmp = split(line, ' ');
            break;
        }
    }
    eFile.seekg(0);

    
    if(tmp.size() < 2) 
    {
        std::cout << "Incorrect format file / non-existing file" << std::endl;
        exit(EXIT_FAILURE);
    }


    bool weighted = false;
    if (tmp.size() == 3)
        weighted = true;

    // TODO make it faster
    std::vector<uint64_t> nodes;
    boost::bimap<uint64_t, uint64_t> nodes_bimap;

    while (std::getline(vFile, line)) {
        if (!line.empty()) {
            nodes.push_back(std::stoul(line));
        } 
    }
    
    std::sort(nodes.begin(), nodes.end());

    uint64_t new_idx = 0;
    for (uint64_t node : nodes) {
        nodes_bimap.insert({node, new_idx});
        new_idx++;
    }

    uint64_t tmp_from, tmp_to;
    double tmp_weight;
    uint64_t i = 0;
    while (std::getline(eFile, line)) {
        if (!line.empty()) {
            tmp = split(line, ' ');

            tmp_from = std::stoul(tmp[0]);
            tmp_to = std::stoul(tmp[1]);
            tmp_weight = (weighted)?std::stof(tmp[2]):1; 
            
            edges[i] = std::make_tuple(nodes_bimap.left.at(tmp_from), nodes_bimap.left.at(tmp_to), tmp_weight);
             

            if (undirected) 
            {
                edges[num_edges/2+i] = std::make_tuple(nodes_bimap.left.at(tmp_to), nodes_bimap.left.at(tmp_from), tmp_weight);
            }
            i++;
        }
    }
    
    return nodes_bimap; 
}

void print_graph_info(uint64_t num_vertices, uint64_t num_edges, bool undirected){
    std::string prop = (undirected) ? "Undirected" : "Directed";
    std::cout << prop << " graph" << std::endl;
    std::cout << "num nodes: " << num_vertices << std::endl;
    std::cout << "num directed edges: " << num_edges << std::endl << std::endl;
}

void print_edge_list(std::tuple<uint64_t, uint64_t, double>* edges, uint64_t num_edges){
    for(uint64_t j = 0; j < num_edges; j++)
        std::cout << std::get<0>(edges[j]) << " " << std::get<1>(edges[j]) << " " << std::get<2>(edges[j]) << std::endl;
    std::cout << std::endl;
}

void process_mem_usage(double& vm_usage, double& resident_set, bool diff)
{
    std::ifstream stat_stream("/proc/self/stat",std::ios_base::in);

    std::string pid, comm, state, ppid, pgrp, session, tty_nr;
    std::string tpgid, flags, minflt, cminflt, majflt, cmajflt;
    std::string utime, stime, cutime, cstime, priority, nice;
    std::string O, itrealvalue, starttime;

    unsigned long vsize;
    long rss;

    stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
                >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
                >> utime >> stime >> cutime >> cstime >> priority >> nice
                >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

    stat_stream.close();

    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
    
    if(!diff){
        vm_usage = vsize / 1024.0;
        resident_set = rss * page_size_kb;
    } else {
        vm_usage = (vsize / 1024.0) - vm_usage;
        resident_set = (rss * page_size_kb) - resident_set;
    }
}

#endif //ORACLE_CONTEST_UTILS_H
