edgelist = [7 4 0.83;
3 1 0.53;
5 3 0.69;
1 3 0.5;
6 4 0.39;
8 1 0.39;
3 10 0.52;
1 5 0.3;
2 4 0.1;
5 8 0.1;
6 3 0.23;
2 5 0.3;
3 8 0.21;
2 10 0.12;
9 4 0.69;
3 5 0.62;
5 4 0.53];

edgelist = unique(edgelist, 'rows');
sz = max(max(edgelist(:, 1:2)));
A = sparse(edgelist(:,1), edgelist(:,2), edgelist(:,3), sz, sz);

B = full(A)

%csvwrite('/Users/andreabosisio/Desktop/full-matrix-example_directed.csv', B)