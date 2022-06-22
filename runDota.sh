# Run 20 iterations on dota-league, with src vertex 55712
# Save results in dota-league.csv 
echo "Benchmarking dota-league graph (20 iterations)"
echo "Src_Vertex,PopulateTime(ms),MemUsage(MB),BFSTime(ms),BFSSum,DFSTime(ms),DFSSum" > dota-league.csv 
bin/exe eval_graphs/dota-league/dota-league 55712 20 -U >> dota-league.csv