#include <bits/stdc++.h> // Todas as bibliotecas padrão de c++
#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
using namespace std;
//Objetivo agora: melhorar a seleção parental
//Uma opção válida é o GA simples: troca todo mundo à cada iteração
//Outra opção 
//Depois, fazer o crossover e a mutação
//Depois, fazer o replacement
//Preciso pensar na estratégia ótima dado o problema
typedef vector<vector<int>> grafo;
vector<int> selection_algogen(vector<pair<int,vector<int>>> populacao,int numero_selec,int tamanho_pop,int seed= time(0)); //Declaração das funções
vector<vector<int>> crossover_algogen(vector<pair<int,vector<int>>>populacao, vector<int>selecionados);

//Preciso que a heurística guarde as respostas, vai ser bom para o algo gen: receber os grafos ordenadas tem valor
//usar operações de gene em gene na recombinação e mutação: se fosse pegando uma fatia, teria problemas de localidade
vector<int> algogen(grafo g,vector<unordered_set<int>> h, vector<int> custos, int k, int seed){
     vector<pair<int,vector<int>>> populacao (h.size(), pair<int,vector<int>>(g[0].size(), 0)); //Indivíduo é um vetor de inteiros, com 0 se o vértice é feliz e 1 se o vértice é infeliz
     //Instanciar a população inicial: cada indivíduo é formado de acordo com cada coloração estabelecida em h (gerada pela heurística anterior)
     mt19937 gen(seed);
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
     int selec_por_iter = 12; //Número de indivíduos que serão selecionados para a próxima geração
     while(iter< 100){ //Depois verificação se ele não muda depois de x iterações
          iter++;
          vector<int> selecionados(selec_por_iter); //posição no vetor população dos selecionados
          vector<vector<int>> offspring(selec_por_iter); //as soluções criadas
          selecionados = selection_algogen(populacao, selec_por_iter ,tamanho_pop,seed); //Selecionar os indivíduos
          offspring = crossover_algogen(populacao, selecionados);
     }//realizar o crossover bit a bit
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
vector<int> selection_algogen(vector<pair<int,vector<int>>> populacao,int numero_selec,int tamanho_pop,mt19937 gen){
     //As soluções são muito próximas, ranking linear parece interessante:dá as probabilidades sem grandes saltos
     //Logo, primeira opção: ranking linear
     vector<int> selecionados;
     vector<double> p_rank_linear(tamanho_pop);

     selecionados.reserve(numero_selec); //

     vector<double> pointers;//ponteiros para o SUS
     pointers.reserve(numero_selec);
     //Vou usar a fórmula que PRank = 2-s/u + 2i(s-1)/u(u-1), onde S vou considerar 2: peso para "aumentar" a pressão de seleção
     //Ranks maiores perto do 0: ordenado do maior para o menor fitness
     p_rank_linear[0] = 1;
     for (int i=1; i< tamanho_pop;i++){
          //CDF decrescente, começa com 1 e tira a probabilidade de i, que é N-i-1/a soma de todas as probabilidades.
          p_rank_linear[i] = p_rank_linear[i-1] - 2*(tamanho_pop-i-1)/(tamanho_pop*(tamanho_pop-1));
     }
     //Agora, samplear os dados usando a SUS e colocar elitismo também.
     
     uniform_real_distribution<double> dist(0.0, 1.0 / numero_selec);
     double start = dist(gen);
     for (int i =0; i<numero_selec;i++){
          pointers.push_back(start + i* (1.0/numero_selec));
     }
     for (double pointer : pointers){
          for (int i = p_rank_linear.size()-1; i>=0;i--){
               if (pointer>p_rank_linear[i]){
                    continue;
               }
               else{
                    selecionados.push_back(i);
                    break;
               }
          }
     }
     return selecionados;
}

vector<vector<int>> crossover_algogen(vector<pair<int,vector<int>>>populacao, vector<int>selecionados){ 

}
