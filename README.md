# [HPGDA2022] Compressed Sparse Row: an efficient representation for sparse graphs
In the Data Structure track of the HPGDA contest there is the chance to play with different sparse
graph representations and explore their runtime performance to improve. In particular, the challenge
requires the creation of an appropriate data structure for the implementation of a graph, a population
algorithm and a neighbor search algorithm for each node. The data structure interfaces with real data from
the LDBC Graphanalytics Benchmark suite dataset. The evaluation is based on: graph population time,
graph size in memory and execution time of the BFS and DFS algorithms.
### Evaluation Graphs
The graphs employed for the evaluation are available in the ``` eval_graphs.tar.gz``` archive on [Google Drive](https://drive.google.com/file/d/15vjYvcNAt7FODQqu4kma3X8jXnxTC6J1/view?usp=sharing).
You can download and extract the archive by running (in this folder):
```
wget -O eval_graphs.tar.gz "https://drive.google.com/uc?export=download&id=15vjYvcNAt7FODQqu4kma3X8jXnxTC6J1&confirm=yes" && tar -xzvf eval_graphs.tar.gz
```
For each graph, there is a folder containing 3 files:
- *graph_name*`.v`: containing the set of nodes;
- *graph_name*`.e`: containing the set of edges;
- *graph_name*`.properties`: summarizing the properties of the graph. It also provides the source vertex chosen by LDBC to ease the reproducibility and comparability of different implementations. In your experiments, you can use other source vertices, but we recommend including in your sets of tested vertices the ones reported in the `.properties` file.   


## Methods
To store the graph we used a data structure based on Compressed Sparse Row (CSR) format. We implemented the following methods:
* ```populate```, that, given an edge list, populate the CSR data structure 
* ```get_neighbors```, implemented with an iteretor that, given a vertex, returns all the neigbors (with the corresponding weight) of that vertex

## Compile and run
### Compile
To compile the code just run ```make``` in the root folder.

### Run
```src/main.cpp``` takes five positional arguments:
1. *path to graph* (required);
1. *source vertex* for BFS and DFS (required);
1. *number of iterations* of graph population, BFS and DFS (required);
1. `-U` if the graph is undirected (default is directed);
1. `-d` for debugging mode (default is no debugging)
1. `-s` to run the serial version of the populate (default is parallel) 

To run the example (3 iterations) on the ```example_directed``` graph, with source vertex 2:
``` 
bin/exe data/example_directed 2 3 -d
```
To run the example (3 iterations) on the ```example_undirected``` graph, with source vertex 2:
``` 
bin/exe data/example_undirected 2 3 -U -d
```
To run the example (1 iteration) on the ```wiki-Talk``` graph (from the Evaluation Graphs suite), with source vertex 2:
``` 
bin/exe eval_graphs/wiki-Talk/wiki-Talk 2 1 -d
```
To run an evaluation-like script:
``` 
bash run.sh
```
``` 
bash runWiki.sh
```
``` 
bash runDota.sh
```
``` 
bash runCit.sh
```
### Parallelization - Segmentation Fault Problem
The  ```populate``` method runs in parallel: you may get a stack overflow due to the duplication of data in the stack of each thread. To avoid this problem you should increase the stack limit by running the following before executing the code (only for Unix systems):
``` 
ulimit -s unlimited
```
If it doesn't work try with 
``` 
ulimit -s 90000
```


## Results
This results are releated to system: Intel Core i7-8550U with 1.8GHz clock speed, 4 cores. The machine has 8GB of RAM,
256KB of L1 cache, 1M of L2 cache, and 8MB
of L3 cache. Programs were written in c++ and
compiled with GCC 9.4.0. and are compered with the basic implementation proposed in the course that uses an Adjacency List to store the graph.
| Data structure | Graph | Memory Usage (MB) | Populate Time (ms) | BFS Time (ms) | DFS Time (ms) | 
|---|---|---|---|---|---|
| CSR | wiki-Talks | 1MB | 135ms | 142ms | 219ms |
| CSR | cit-Patents | 28MB | 1789ms | 40ms | 35ms |
| CSR | dota-league | 1023MB | 1867ms | 1ms | 1ms |
| AL | wiki-Talks | 455MB | 695ms | 767ms | 696ms |
| AL | cit-Patents | 1182MB | 5443ms | 186ms | 188ms |
| AL | dota-league | 3640MB | 8967ms | 197ms | 217ms |
## Libraries used
- OpenMP 
- Boost 
