#include <bits/stdc++.h> // Todas as bibliotecas padrão de c++
#include <iostream>
#include <filesystem>
#include <chrono>
#include "solucao.hpp"
#include "gera_grafos.hpp"
using namespace std;
typedef vector<vector<int>> grafo;

pair<int,pair<int,unordered_set<int>>> BT (grafo g, vector<int>custo/*Custo de cada vértice*/, int k, 
    unordered_set<int> h/*Felizes*/, unordered_set<int> hB/*Incoloríveis*/, 
    unordered_set<int> f/*Coloríveis*/,
    vector<bool> colorido/*coloridos*/,
    clock_t clock_start,int limit,/*Tempo inicial e limite*/
    int seed,
    int bestsol = 0/*Melhor solução*/, int nodes=0/*Nós explorados*/)
    {
    clock_t clock_now = clock();
    
    if (double((clock_now-clock_start)/CLOCKS_PER_SEC)>=limit){ //Verifica se já se passou o tempo necessário, (sempre excede um pouco, pelo retorno)
        return {nodes,{bestsol,h}};
    }

    int v = -1; //candidato à fazer feliz
    int graumin = g.size()+1; // grau mínimo do candidato à fazer feliz: só para ter parâmetro inicial de comparação
    vector<int> candidatos(0);//Vetor de candidatos, escolho eles aleatóriamente depois (semente fixa, talvez tenha que mudar)

    unordered_set<int> hl= h; //Felizes caso v seja feliz
    vector<int> custol = custo; //custo caso v seja feliz
    vector<bool> coloridol = colorido; //Coloridos caso v seja feliz
    unordered_set<int> fl = f;  //Coloríveis caso v seja feliz
    unordered_set<int> hBl = hB;//Incoloríveis caso v seja feliz
    mt19937 gen(seed); //Seed não definida
    //Obtendo os candidatos à v: os de menor custo/grau
    for (auto it = f.begin(); it != f.end(); ) {
        /*Por que não tirar os que não podem ser felizes nessa etapa? Questão de estrutura:
        unordered_set não pode sofrer variação no número e quantidade de elementos: os ponteiros podem se 
        perder*/
        if (colorido[*it]){
            if (custo[*it]<graumin){
                graumin = custo[*it];
                candidatos = {*it};
                }
            else{
                if (custo[*it]==graumin){
                candidatos.push_back(*it);
                }
            }
        }else{
            if (custo[*it]+1<graumin){
                    graumin = custo[*it]+1;
                    candidatos = {*it};
            }else{
                if (custo[*it]+1==graumin){
                    candidatos.push_back(*it);
                    }
            }
        }
        ++it;
        }



    //Randomização da escolha do candidato para V
    uniform_int_distribution<> dist(0, candidatos.size()-1);
    if(graumin>k){
        return {nodes,{bestsol,h}};
        //Trata o caso de não ter candidatos.
    }
    v = candidatos[dist(gen)];

    fl.erase(v); //v é colorido para esse conjunto
    hl.insert(v); // Conjunto que assume que v é feliz


    //Tornando V feliz verificando a vizinhança e ajustando os seus "custo"    
    for (int viz : g[v]){  //para cada vizinho de v e para cada vizinho dos vizinhos de v, reduzir o grau do vizinho em 1, caso nao tenham sido coloridos
        //Se v já tinha sido colorido na etapa anterior, não precisa reavaliar vizinhos de v
        if (colorido[v] == 0){
            custol[viz]--; //tira 1 grau pois o vértice v ficou feliz, logo, colorido

        }
        if (colorido[viz] == 0){
            coloridol[viz] = 1; //colore ele, e reavalia o grau/custo dos vizinhos, para o mundo de v feliz
            for (int vizviz : g[viz]){
                custol[vizviz]--;
            }
        }
    }
    
    coloridol[v] = 1;
    custol[v] = 0;
    f.erase(v);//Seja para o caso de que v é feliz ou não é feliz, v não será colorível: a cada chamada, se acaba com um v
    hB.insert(v);
    //Reavaliando os coloríveis e não coloríveis para o caso de que v foi feito feliz

    if (colorido[v] ==1){//Considera se v foi colorido nessa etapa ou em uma anterior
    //se algum vizinho de v foi colorido antes, o custo[v] já é ajustado
        for (auto it = f.begin(); it != f.end(); ) {
            if (custol[*it]>(k-custo[v])){ 
                fl.erase(*it);
                hBl.insert(*it);
                }
            ++it;
            }
    }
    else{
        for (auto it = f.begin(); it != f.end(); ) {
            if (custol[*it]>(k-(custo[v]+1))){
                fl.erase(*it);
                hBl.insert(*it);
                }
            ++it;
            }
    }
    bestsol = max(bestsol,static_cast<int>(hl.size()));//a bestsol é o melhor valor obtido até então
    int melhor;
    int nodes_bra;//nodes explorados no branch a
    int nodes_brb;//nodes explorados no branch b
    pair<int,unordered_set<int>> getter; //serve para pegar o retorno da função, somente.
    unordered_set<int> solucao1 = hl; // para poder retornar o conjunto-solução
    unordered_set<int> solucao2 = hl;
    if ((hl.size() + fl.size()) > (bestsol)){ //Considero v feliz 
        if (colorido[v] == 1){ //Se essa etapa não coloriu v, o número de vértices coloridos não é k-custo[v]+1 (+1 é o próprio vértice, que já estava colorido)
            
            tie(nodes_bra,getter) = BT(g,custol,k-(custo[v]),hl,hBl,fl,coloridol,clock_start,limit,bestsol,nodes);
            melhor = getter.first;
            solucao1 = getter.second;
            if (melhor< bestsol){
                solucao1 = hl;
            } 
            else{
                bestsol = melhor;
            }   
        }
        else{
            tie(nodes_bra,getter) = (BT(g,custol,k-(custo[v]+1),hl,hBl,fl,coloridol,clock_start,limit,bestsol,nodes));
            melhor = getter.first;
            solucao1 = getter.second;
            if (melhor< bestsol){
                solucao1 = hl;
            } 
            else{
                bestsol = melhor;
            }
            }
        }//O caso acima considera v feliz, de duas formas: v já estava colorido por uma etapa anterior ou não
        
        if ((h.size()+f.size())>(bestsol)){
            tie(nodes_brb,getter) = (BT(g,custo,k,h,hB,f,colorido,clock_start,limit,bestsol,nodes)); 
            melhor = getter.first;
            solucao2 = getter.second;
            if (melhor< bestsol){
                solucao2 = hl;
            } 
            else{
                bestsol = melhor;
            }
        }
        //não tornei ninguém feliz para essa etapa: considera os mesmos conjuntos, retirando v de f e introduzindo em hB
        //hB não é necessário no geral
    if (solucao1.size()>solucao2.size()){

        return {nodes_bra+nodes_brb+1, {bestsol,solucao1}};    
    }

    return {nodes_bra+nodes_brb+1, {bestsol,solucao2}};
}

pair<int,pair<int,unordered_set<int>>> felicidade_maxima(grafo g,vector<int> custo, int k, clock_t clock_start,int limit, int seed){
    unordered_set<int> h;
    unordered_set<int> hB;
    unordered_set<int> coloriveis;
    vector<bool> coloridos(g.size(),0);
    //Etapa para a primeira avaliação se o vértice pode ser feliz ou não
    for (int i = 0;i<custo.size();i++){
        if (custo[i]-1 > k){
            hB.insert(i) ;
        }else{
            coloriveis.insert(i);
        }
    }
    return BT(g,custo,k,h,hB,coloriveis,coloridos,clock_start,limit,seed);
}
/*int main(){
    int nodes;
    int resposta;
    unordered_set<int> solu;
    pair<int,unordered_set<int>> get;
    grafo g;
    vector<int> graus;
    int k;
    pair<grafo,vector<int>> getter;
    tie(k,getter) = leitor_grafos("pasta//graph_d_regular_k2_p0.1_v50_20241127_082538.txt");
    g = getter.first;
    graus = getter.second;
    clock_t clock_start = clock();
    tie(nodes,get) = felicidade_maxima(g,graus,k,clock_start, 1);
    resposta = get.first;
    solu = get.second;

}*/


