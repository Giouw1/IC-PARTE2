#include <bits/stdc++.h> // Todas as bibliotecas padrão de c++
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

typedef vector<vector<int>> grafo;

//Preciso que a heurística guarde as respostas, vai ser bom para o algo gen: receber os grafos ordenadas tem valor
grafo algogen(grafo g,vector<unordered_set<int>> h, vector<int> custos, int k, int seed){
     vector<pair<int,vector<int>>> populacao (h.size(), pair<int,vector<int>>(g[0].size(), 0)); //Indivíduo é um vetor de inteiros, com 0 se o vértice é feliz e 1 se o vértice é infeliz
     //Instanciar a população inicial: cada indivíduo é formado de acordo com cada coloração estabelecida em h (gerada pela heurística anterior)
     srand(seed); //Inicializa os randoms de acordo com a seed, se eu for usar mais para frente
     for (int i =0;i<h.size();i++){
          populacao[i].first = h[i].size();
          for (auto it = h[i].begin(); it != h[i].end(); ) {
               populacao[i].second[*it] = 1;
          }
     }
     //Manter o vetor populacao sempre ordenado de acordo
     //Começo das iterações do algoritmo genético:
     int iter = 0;
     int tamanho_pop = populacao.size();
     while(iter< 100){ //Depois verificação se ele não muda depois de x iterações
          iter++;
          //Seleção do pai
          //Por enquanto, vou dar valores quase arbitrários para os pesos dos indivíduos na seleção:
          //Usar o valor da solução parece ser ruim, pq os valores são próximos: relacionar variação com impuslionar os pesos? - variação + impulsiona 
          //Colocar os valores tamanho_pop, tamanho_pop-1, para avançar nisso logo e ser mais prático
          vector<int> selecionados(12); //6 é um número arbitrário, a fim de testes
          for (int j =0;j<12;j++){
          int numeroAleatorio = rand() % static_cast<int>(ceil(((tamanho_pop)*(tamanho_pop-1))/2));
          for (int i =0;i<populacao.size();i++){
               numeroAleatorio -= tamanho_pop - i;
               if (numeroAleatorio <0){
                    selecionados.push_back(i);
                    }
               }
          }
          //Selecionei os indivíduos: agora é recombinar e mutar e avaliar
          //Como recombinar
          //Posso fazer recombinação bit a bit, de forma dinâmica, no sentido de que a probabilidade da primeira recombinação é maior do que a segunda
          //Não quero dar tanta liberdade de escolha para não ser custoso demais
          //Começar com a crossover rate
          float crossover_rate = 0.8;
          for (int i = 0; i<selecionados.size(); i+=2){
               if (rand() / float(RAND_MAX)> crossover_rate){//executar o crossover

               }
               else{
                    continue;
               }
          }
     }
     //Com a população inicial instanciada, a lógica, agora, é permitir processos seleção, crossover,mutação e reposição.
     //A lógica da seleção é pegar n melhores soluções e realizar crossovers entre elas (e possivelmente mutações): gerando n/2 novos elementos, e após isso repor
     //Parte da solução antiga. Como escolher quem repor? ordenado somente? 
     //Ir pegando o livro-referência para tirar essas dúvidas.
}
int evaluate_algogen(vector<int> individuo, grafo g, int k){// Acho que está correto 
     unordered_set<int> fazem_parte;
     int valor = 0;
     for (int i = 0; i< individuo.size();i++){
          if (individuo[i] == 1){
               //Se ele for feliz
               valor += 1;
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
          return -1; //Se k<0, solução não viável
     }
     else{
          return valor;
     }
}
