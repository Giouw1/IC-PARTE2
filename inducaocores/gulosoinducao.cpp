//Lógica é escolher um vértice colorido, tentar expandir ele ao máximo: até um vizinho do não colorido ser colorido,e passar para outro: enquanto houver vértice colorido expandível: fazer uma análise inicial dos expandíveis
#include <bits/stdc++.h> // Todas as bibliotecas padrão de c++
#include "gera_grafos.hpp"
using namespace std;
typedef vector<vector<int>> grafo;
//testar isso nas instancias dadas: ler os artigos, fazer o leitor de instâncias, testar esse código e o outro.
vector<int> inducaoheuristica(grafo g, vector<int> cor, unordered_set<int> expandivel){
    int expandir;
    bool pode_expandir;
    while (expandivel.size() != 0){
        expandir = *expandivel.begin(); //pega um elemento qualquer do conjunto "expandível"
        if (cor[expandir] == 0){ //se o elemento for de cor zero, tira ele dali
            expandivel.erase(expandir);
            continue;
        }
        for (int i = 0; i<g[expandir].size();i++){// vê a possível expansão para cada vizinho do candidato
            if (g[expandir][i] == 0){
                continue;
            }
            pode_expandir = 1;
            if (cor[i] == 0){// só vê a possível expansão para vizinhos sem cor
                for (int j = 0; j<g[i].size();j++){//para cada vizinho do vértice vizinho do candidato
                    if (g[i][j] == 1){
                        if ((cor[j] != 0) and (cor[j] != cor[expandir])){
                            pode_expandir = 0;  
                            break;
                            }
                        }
                    }
                    if (pode_expandir == 1){
                        expandivel.insert(i);
                        cor[i] = cor[expandir];
                    }
                }
            }
            expandivel.erase(expandir);
        }
        return cor;
    }
vector<int> coloracaomaxima(grafo g, vector<int> cor){
    // a princípio, todo vértice é expandível (não é verdade, preciso refazer essa análise, mas para um teste inicial já funciona)
    //para termos de teste, funciona falar que todo vértice é expandível e tratar isso depois.
    unordered_set<int> expandivel;
    for (int i = 0; i<g.size();i++){
        expandivel.insert(i);
    }
    return inducaoheuristica(g,cor,expandivel);

}
int main(){
    grafo g;
    vector<int> cores;
    tie (g, cores) = gera_grafo(7, 3, 2);
    cores = coloracaomaxima(g,cores);
    return 0;
}