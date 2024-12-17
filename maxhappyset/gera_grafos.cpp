#include "gera_grafos.hpp"
#include <random>
// Alias para lista de adjacência
using namespace std;
// função
typedef vector<vector<int>> grafo;

pair<grafo,vector<int>> criador_grafos(int nvert,int nedges){
    mt19937 gen(random_device{}());
    uniform_int_distribution<> dist(0, nvert-1);
    grafo g(nvert,vector<int> (nvert,0));
    vector<int> graus (nvert,0);
    int u;
    int v;
    for (int i = 0; i < nedges; i++) {
        // Randomly pick two distinct vertices
        u = dist(gen);
        v = dist(gen);

        if ((u != v) and (g[u][v] != 1)) {
            // Aresta entre u e v
            graus[u]++;
            graus[v]++;
            g[u][v] = 1;
            g[v][u] = 1;
        } else {
            // Repetir se for o mesmo vértice
            --i;
        }
    }

    return {g,graus};
}



