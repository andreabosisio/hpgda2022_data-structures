# Run 10 iterations on dota-league, with src vertex 3774754
# Save results in dota-league.csv 
echo "Benchmarking dota-league graph (10 iterations)"
echo "Src_Vertex,PopulateTime(ms),MemUsage(MB),BFSTime(ms),BFSSum,DFSTime(ms),DFSSum" > dota-league.csv 
bin/exe eval_graphs/dota-league/dota-league 55712 10 -U >> dota-league.csv