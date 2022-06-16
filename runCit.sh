# Run 10 iterations on cit-Patents, with src vertex 3774754
# Save results in cit-Patents.csv 
echo "Benchmarking dota-league graph (10 iterations)"
echo "Src_Vertex,PopulateTime(ms),MemUsage(MB),BFSTime(ms),BFSSum,DFSTime(ms),DFSSum" > cit-Patents.csv 
bin/exe eval_graphs/cit-Patents/cit-Patents 3774754 10 >> cit-Patents.csv