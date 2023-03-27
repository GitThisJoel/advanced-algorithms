# Lab 2 - Independent Set

from collections import defaultdict
from copy import copy


def read_file(fp):
    with open(fp, "r") as f:
        n = int(f.readline())
        data = f.read().strip("\n").split("\n")
        adj_matrix = ["".join(d.split()) for d in data]
    return n, adj_matrix


def make_graph(adj_matrix):
    G = defaultdict(set)
    for i in range(len(adj_matrix)):
        # remember that the matrix is symetric
        # if (i,j) ∈ E then (j,i) ∈ E
        for j in range(i, len(adj_matrix[i])):
            if adj_matrix[i][j] == "1":
                G[i].add(j)
                G[j].add(i)
    return dict(G)


def R0(G, nodes):
    if len(nodes) == 0:
        return 0

    max_neigh = -1
    for k in nodes:
        v = G[k]
        if len(v) == 1:
            nodes.remove(k)
            return 1 + R0(G, nodes)
        if len(v) > max_neigh:
            u = k
            max_neigh = len(v)

    n1 = copy(nodes)
    n1.remove(u)
    for n in G[u]:
        if n in n1:
            n1.remove(n)

    n2 = copy(nodes)
    n2.remove(u)
    return max(1 + R0(G, n1), R0(G, n2))


if __name__ == "__main__":
    n, am = read_file("../data/g30.in")
    G = make_graph(am)
    assert len(G) == n, "wrong number of vertecies"

    v = R0(G, set(G.keys()))
    print(v)
