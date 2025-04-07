#include <bits/stdc++.h> // Todas as bibliotecas padrão de c++
#include <iostream>
#include <fstream>
using namespace std;

typedef vector<vector<int>> grafo;

//Preciso que a heurística guarde as respostas, vai ser bom para o algo gen: receber os grafos ordenadas tem valor
grafo algogen(vector<grafo> g,vector<unordered_set<int>> h, vector<int> custos, int k, int seed){ //dar continuidade depois
     //Vou, a princípio, fazer com que cada indivíduo seja o estado dos vértices, felizes ou não.
     vector<vector<bool>> populacao (g.size(), vector<bool>(g[0].size(), 0));
     //Instanciar a população inicial
     for (int i =0;i<g.size();i++){
          for (auto it = h[i].begin(); it != h[i].end(); ) {
               populacao[i][*it] = 1;
          }
     }
     //Com a população inicial instanciada, a lógica, agora, é permitir processos seleção, crossover,mutação e reposição.
     //A lógica da seleção é pegar n melhores soluções e realizar crossovers entre elas (e possivelmente mutações): gerando n/2 novos elementos, e após isso repor
     //Parte da solução antiga. Como escolher quem repor? ordenado somente? 
     //Ir pegando o livro-referência para tirar essas dúvidas.
}
bool evaluate_algogen(vector<bool> individuo, grafo g, int k){// Acho que está correto 
     unordered_set<int> fazem_parte;
     for (int i = 0; i< individuo.size();i++){
          if (individuo[i] == 1){
               //Se ele for feliz
               //Fazer k-- para cada vizinho a nao ser ja tenha tirado
               for (int j = 0; j<g[i].size(); j++){//
               int marker = 0;
               //Se o elemento não for vizinho de algum fazem_parte, k--
                    for(auto it = fazem_parte.begin(); it != fazem_parte.end();){
                         for(int k = 0; k<g[*it].size();k++){
                              if (g[*it][k] == g[i][j]){
                                   marker = 1;
                              }
                         }
                         //se g[i] for vizinho de it
                    }
                    if (marker == 0){
                         k--;
                    }
               }
          }
     }
     if (k < 0){
          return 0; //Se k<0, solução não viável
     }
}