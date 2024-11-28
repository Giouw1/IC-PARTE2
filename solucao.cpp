#include <bits/stdc++.h> // Todas as bibliotecas padrão de c++
#include "gera_grafos.hpp"
using namespace std;


int BT (grafo g, vector<int>graus, int k, unordered_set<int> h, unordered_set<int> hB, unordered_set<int> f, vector<bool> colorido = {},int bestsol = 0){
    int v = -1;
    int graumin = g.size()+1;
    vector<int> auxiliar;
    vector<int> candidatos(0);
    //Todas as estruturas a seguir são implementadas para armazenar os conjuntos reavaliados após colorir o candidato
    unordered_set<int> hl= h;
    vector<int> grausl = graus;
    vector<bool> coloridol = colorido;
    unordered_set<int> fl = f;
    unordered_set<int> hBl = hB;
    mt19937 gen(random_device{}());
    //Obtendo os candidatos à v: os de menor custo/grau
    for (auto it = f.begin(); it != f.end(); ) {
        if (graus[*it]<graumin){
            graumin = graus[*it];
            candidatos = {*it};
            }
        else{
            if (graus[*it]==graumin){
            candidatos.push_back(*it);
            }
        }
        ++it;
        }



    //Randomização da escolha do candidato para V (evita ficar preso em regiões)
    uniform_int_distribution<> dist(0, candidatos.size()-1);
    v = candidatos[dist(gen)];
    if (colorido[v] ==1){//Considera se v foi colorido nessa etapa ou em uma anterior
        if(k-graus[v] < 0){
                return bestsol;
            }
    }else{
        if ((k-(graus[v]+1)<0)){
            return bestsol;
            }
        }
    fl.erase(v); //v é colorido para esse conjunto
    hl.insert(v); // Conjunto que assume que v é feliz
    hBl.insert(v);


    //Tornando V feliz verificando os ao redor e ajustando os seus graus: essencial, pois grau é o custo de deixar o vértice feliz dado K
    
    for (int i = 0; i< g[v].size();i++){  //para cada vizinho de v e para cada vizinho dos vizinhos de v, reduzir o grau do vizinho em 1, caso nao tenham sido coloridos
        if (g[v][i] == 1){ //Se v já tinha sido colorido na etapa anterior, não precisa reavaliar vizinhos de v
            if (colorido[v] != 1){
                grausl[i]--; //tira 1 grau pois o vértice v ficou feliz, logo, colorido
            }
            if (colorido[i] != 1){ //se o vértice i já não tiver sido colorido por uma outra etapa
                coloridol[i] = 1; //colore ele, e colore os vizinhos se eles ja nao forem coloridos
                for (int j = 0; j<g[i].size();j++){ // A avaliação dos vizinhos i dos vértices vizinhos ao feliz da vez
                    if (g[i][j] == 1){
                        grausl[j]--;//tira 1 grau para cada vizinho dos coloridol que deixam v feliz.
                    }
                }
            }
        }
    }
    coloridol[v] = 1;
    grausl[v] = 0;
    f.erase(v);//Seja para o caso de que v é feliz ou não é feliz, v não será colorível: a cada chamada, se acaba com um v
    hB.insert(v);
    //Redefinind os f e hB para o caso de que v foi feito feliz
    if (colorido[v] ==1){//Considera se v foi colorido nessa etapa ou em uma anterior
        for (auto it = f.begin(); it != f.end(); ) {
            if (grausl[*it]>(k-graus[v])){
                fl.erase(*it);
                hBl.insert(*it);
                }
            ++it;
            }
    }
    else{
        for (auto it = f.begin(); it != f.end(); ) {
            if (grausl[*it]>(k-(graus[v]+1))){
                fl.erase(*it);
                hBl.insert(*it);
                }
            ++it;
            }
    }

    bestsol = max(bestsol,static_cast<int>(hl.size()));
    if ((hl.size() + fl.size()) > (bestsol)){ //Considero v feliz, k-graus[v]+1 pois colori graus[v] +1 vértices pela questão da não redefinição de f, uma chamada a mais, a não ser que v fosse o último
        if (colorido[v] == 1){ //Se essa etapa não coloriu v, o número de vértices coloridol não é k-graus[v]+1 (+1 é o próprio vértice, que já estava colorido)
            bestsol = max(bestsol, (BT(g,grausl,k-(graus[v]),hl,hBl,fl,coloridol,bestsol)));
            }
        else{
            bestsol = max(bestsol,(BT(g,grausl,k-(graus[v]+1),hl,hBl,fl,coloridol,bestsol)));
            }
        }//O caso acima considera v feliz, de duas formas: v já estava colorido por uma etapa anterior ou não
        
        if ((h.size()+f.size())>(bestsol)){
            bestsol = max(bestsol,(BT(g,graus,k,h,hB,f,colorido,bestsol))); 
        }//não tornei ninguém feliz para essa etapa: considera os mesmos conjuntos, retirando v de f e introduzindo em hB
        //hB não é necessário no geral

    return bestsol;
}

int felicidade_maxima(grafo g,vector<int> graus, int k){
    unordered_set<int> h;
    unordered_set<int> hB;
    unordered_set<int> coloriveis;
    vector<bool> coloridos(g.size(),0);
    for (int i = 0;i<graus.size();i++){
        if (graus[i] > k){
            hB.insert(i) ;
        }else{
            coloriveis.insert(i);
        }
    }
    return BT(g,graus,k,h,hB,coloriveis,coloridos,0);
}


int main(){ //Isso aqui é o problema da mochila, com os custos variando: tentar fazer um bottom up pra melhorar
    grafo g;
    vector<int> graus; 
    tie(g,graus)= criador_grafos(6,9);
    int b = felicidade_maxima(g,graus,5);
    cout << b<< endl;
    return 0;
}

//rever e passar para o git de manhazinha: testar outros casos.
