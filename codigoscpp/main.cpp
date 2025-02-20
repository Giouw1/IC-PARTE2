#include <bits/stdc++.h> // Todas as bibliotecas padrão de c++
#include <iostream>
#include <filesystem>
#include <chrono>
using namespace std;
#include "maxsetguloso.hpp"
#include "solucao.hpp"
#include "gera_grafos.hpp"

int main(int argc, char* argv[]) {
    if (argc < 5) {  // Expecting at least 4 arguments (program name + 3 inputs)
        cerr << "Está faltando parâmetros: É um path, um inteiro(limite de tempo), e outro inteiro (seed)"<< endl;
        return 1;
    }
    string path = argv[1];     // First argument: file path
    int k = stoi(argv[2]);
    int limit = stoi(argv[3]); // Second argument: integer (converted from string)
    int seed = stoi(argv[4]);
    ofstream arquivoresult("resultcpp.txt");
    grafo g;
    vector<int> graus;
    int nodes;
    int solutionback;
    unordered_set<int> happyback;
    unordered_set<int> coloredback;
    tie(g,graus) = leitor_grafos(path);
    //lendo o grafo
    clock_t clock_start;
    clock_t clock_end;
    double tempo;
    pair<int,unordered_set<int>> getter;
    //executando backtracking
    //printa o tempo atual
    time_t timestamp = time(&timestamp);
    struct tm datetime = *localtime(&timestamp);
    cout << datetime.tm_min<< " "<< datetime.tm_sec<<endl;

    clock_start = clock();
    tie(nodes,getter) = felicidade_maxima(g,graus,k,clock_start,limit,seed);
    clock_end = clock();

    //printa o tempo final: termos de comparação -- é válido o tempo de execução da tabela
    time_t timestampa = time(&timestampa);
    struct tm datetimea = *localtime(&timestampa);
    cout << datetimea.tm_min<< " "<< datetimea.tm_sec<<endl;

    tempo = double((clock_end-clock_start)/CLOCKS_PER_SEC);

    solutionback = getter.first;
    happyback = getter.second;
    arquivoresult<< nodes << " "<< tempo << " "<< solutionback<<endl;
    
    for (int element: happyback){
        for (int viz : g[element]){
            coloredback.insert(viz);
        }
    }
    for (int element:happyback){
        arquivoresult<< "h"<< " "<< element <<endl;
    }    
    for (int element:coloredback){
        arquivoresult<< "c"<< " "<< element <<endl;
    }
    int solutionheur = 0;
    int ajuda = 0;
    int maior_tempo=0;
    int menor_tempo=-1;
    unordered_set<int> selected_heur;
    unordered_set<int> happyheur;
    pair<unordered_set<int>,unordered_set<int>> getterheur;
    unordered_set<int> respostas;
    clock_t clock_starte;
    clock_t clock_ende;
    //chama a heurística
    for (int i = 0;i<25;i++){//marcar todos os resultados e ver os tempos--total e individual
        ajuda = solutionheur;
        clock_starte = clock();
        tie(solutionheur,getterheur) = felicidade_maxima(g,graus,k,seed+i);
        clock_ende = clock();
        if((menor_tempo == -1)or menor_tempo>double((clock_end-clock_start)/CLOCKS_PER_SEC)){
            menor_tempo = double((clock_ende-clock_starte)/CLOCKS_PER_SEC);
        }
        if (double((clock_ende-clock_starte)/CLOCKS_PER_SEC)>maior_tempo){
            maior_tempo = double((clock_ende-clock_starte)/CLOCKS_PER_SEC);
        }

        tempo += double((clock_ende-clock_starte)/CLOCKS_PER_SEC);
        respostas.insert(solutionheur);
        if (ajuda>solutionheur){
            solutionheur = ajuda; //colocar no arquivo intermediário os dados de tempo e das respostas no geral
            happyheur = getterheur.first;
            selected_heur = getterheur.second;
        }
    }
    arquivoresult<<solutionheur<< endl;
    arquivoresult<<tempo<<endl;
    for (int element: happyheur){
        arquivoresult<< "h"<< " "<< element <<endl;
    }
    for(int element: selected_heur){
        arquivoresult<< "c"<< " "<< element <<endl;
    }
    for (int element: respostas){
        arquivoresult<<"r"<<" "<<element<<endl;
    }
    arquivoresult.close();
}
