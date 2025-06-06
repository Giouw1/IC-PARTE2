#include <bits/stdc++.h> // Todas as bibliotecas padrão de c++
#include <iostream>
#include <filesystem>
#include <chrono>
using namespace std;
#include "maxsetgulosonovaabordagem.hpp"
#include "solucao.hpp"
#include "gera_grafos.hpp"

int main(int argc, char* argv[]) { //Colocar os parâmetros N interações heurísticas, M elites e Gamma flexibilidade
    if (argc < 7) {  // Expecting at least 4 arguments (program name + 3 inputs)
        cerr << "Está faltando parâmetros: É um path, um inteiro(limite de tempo), outro inteiro (seed), inteiro N e inteiro Gamma"<< endl;
        return 1;
    }
    string path = argv[1]; 
    int k = stoi(argv[2]);
    int limit = stoi(argv[3]); // Second argument: integer (converted from string)
    int seed = stoi(argv[4]);
    int n = stoi(argv[5]);
    float gamma = stof(argv[6]);


    ofstream arquivoresult("resultcpp.txt");
    grafo g;
    vector<int> graus;
    int nodes;
    int solutionback;
    unordered_set<int> happyback;
    vector<bool> coloredback;
    tie(g,graus) = leitor_grafos(path);
    
    //lendo o grafo
    
    clock_t clock_start;
    clock_t clock_end;
    double tempo;
    pair<int,pair<unordered_set<int>,vector<bool>>> getter; 
    
    //executando backtracking

    /*printa o tempo atual
    time_t timestamp = time(&timestamp);
    struct tm datetime = *localtime(&timestamp);
    cout << datetime.tm_min<< " "<< datetime.tm_sec<<endl;
    Para comparar com o tempo
    */

    clock_start = clock();
    tie(nodes,getter) = felicidade_maxima_backtrack(g,graus,k,clock_start,limit,seed);
    clock_end = clock();

    /*printa o tempo final: termos de comparação -- é válido o tempo de execução da tabela
    time_t timestampa = time(&timestampa);
    struct tm datetimea = *localtime(&timestampa);
    cout << datetimea.tm_min<< " "<< datetimea.tm_sec<<endl;
    */
    
    tempo = static_cast<double>((clock_end-clock_start))/CLOCKS_PER_SEC;

    solutionback = getter.first;
    happyback = getter.second.first;
    coloredback = getter.second.second;
    arquivoresult<< nodes << " "<< tempo << " "<< solutionback<<endl;
    
   
    
    for (int element:happyback){
        arquivoresult<< "h"<< " "<< element <<endl;
    }    
    int contador = 0;//para verificar os erros possíveis
    for (int i = 0; i<coloredback.size();i++){
        if(coloredback[i] == 1){
            contador++;
            arquivoresult<< "c"<< " "<< i <<endl;
        }
    }
    //verificador de erro do backtracking
    int error = 0;
    //Todo feliz tem os vértices adjacentes coloridos?
    if (happyback.size()>0){
        for (int element: happyback){
            for (int viz : g[element]){
                if (coloredback[viz] == 0){
                    error=1;
                }
            }
    
        }
    }
    //K é extrapolado?
    if (contador > k){
        error = 1;
    }
 
    if (error == 1){
        arquivoresult<<"ERROB"<<endl;
    }
    else{
        arquivoresult<<"OKB"<<endl;
    }

    //Escrever os resultados da heurística
    int solutionheur = 0;
    int ajuda = 0;
    int maior_tempo=0;
    int menor_tempo=-1;
    unordered_set<int> selected_heur;
    unordered_set<int> happyheur;
    pair<unordered_set<int>,unordered_set<int>> getterheur;
    unordered_set<int> respostas;
    
    double tempo_total = 0;
    clock_t clock_starte;
    clock_t clock_ende;
    //validaback ta errado, ver
    //chama a heurística
    //Ver esse I: tem que ser uma parâmetro.
    for (int i = 0;i<n;i++){//marcar todos os resultados e ver os tempos total e ?individual?
        //A lógica de "ajuda" é para sempre armazenar a solução de maior valor encontrada
        ajuda = solutionheur;
        clock_starte = clock();
        tie(solutionheur,getterheur) = felicidade_maxima_guloso(g,graus,k,seed+i,gamma);
        clock_ende = clock();
        /*if((menor_tempo == -1)or menor_tempo>double((clock_end-clock_start)/CLOCKS_PER_SEC)){
            menor_tempo = double((clock_ende-clock_starte)/CLOCKS_PER_SEC);
        }
        if (double((clock_ende-clock_starte)/CLOCKS_PER_SEC)>maior_tempo){
            maior_tempo = double((clock_ende-clock_starte)/CLOCKS_PER_SEC);
        }
        Não sei se é necessário
        */ 

        tempo_total += static_cast<double>((clock_ende-clock_starte))/CLOCKS_PER_SEC;
        respostas.insert(solutionheur);
        if (ajuda>solutionheur){
            solutionheur = ajuda; //colocar no arquivo intermediário os dados de tempo e das respostas no geral
        }else{    
            happyheur = getterheur.first;
            selected_heur = getterheur.second;
        }
    }
    arquivoresult<<solutionheur<< endl;
    arquivoresult<<tempo_total<<endl; 
    
    int errorr = 0;
    //Todo feliz tem os vértices adjacentes coloridos?
    if (happyheur.size()>0){
        for (int element: happyheur){
            arquivoresult<< "h"<< " "<< element <<endl;
            for (int viz : g[element]){
                if (selected_heur.find(viz) == selected_heur.end()){
                    error=1;
                }
            }
    
        }
    }
    //K é extrapolado?
    if (selected_heur.size() > k){
        errorr = 1;
    }
    //A verificação desses dois erros garante tudo:
    //Todos os vértices são justamente felizes, (restrição da felicidade)
    //E essa felicidade atende à restrição de K
    for(int element: selected_heur){
        arquivoresult<< "c"<< " "<< element <<endl;
    }
    for (int element: respostas){
        arquivoresult<<"r"<<" "<<element<<endl;
    }
    if (errorr == 1){
        arquivoresult<<"ERROH"<<endl;
    }
    else{
        arquivoresult<<"OKH"<<endl;
    }
    arquivoresult.close();
}