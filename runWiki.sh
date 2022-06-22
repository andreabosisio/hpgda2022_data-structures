# Run 20 iterations on wiki-Talk, with src vertex 2
# Save results in results_wiki-Talk.csv 
echo "Benchmarking wiki-Talk graph (20 iterations)"
echo "Src_Vertex,PopulateTime(ms),MemUsage(MB),BFSTime(ms),BFSSum,DFSTime(ms),DFSSum" > results_wiki-Talk.csv 
bin/exe eval_graphs/wiki-Talk/wiki-Talk 2 20 >> results_wiki-Talk.csv