#include <bits/stdc++.h> // Todas as bibliotecas padrão de c++
#include <iostream>
#include <filesystem>
#include <chrono>
#include "solucao.hpp"
#include "gera_grafos.hpp"
using namespace std;
typedef vector<vector<int>> grafo;

pair<int,pair<int,pair<unordered_set<int>,vector<bool>>>> BT (grafo g, vector<int>custo/*Custo de cada vértice*/, int k, 
    unordered_set<int> h/*Felizes*/, unordered_set<int> hB/*Incoloríveis*/, 
    unordered_set<int> f/*Coloríveis*/,
    vector<bool> colorido/*coloridos*/,
    clock_t clock_start,int limit,/*Tempo inicial e limite*/
    int seed,
    int bestsol = 0/*Melhor solução*/, int nodes=0/*Nós explorados*/)
    {
    clock_t clock_now = clock();
    
    if (double((clock_now-clock_start)/CLOCKS_PER_SEC)>=limit){ //Verifica se já se passou o tempo necessário, (sempre excede um pouco, pelo retorno)
        return {nodes,{bestsol,{h,colorido}}};
    }

    int v = -1; //candidato à fazer feliz
    int add; // var auxiliar
    int graumin = g.size()+1; // grau mínimo do candidato à fazer feliz: só para ter parâmetro inicial de comparação (todo custo tem que ser menor que graumin)
    vector<int> candidatos(0);//Vetor de candidatos, escolho eles aleatóriamente depois 
    unordered_set<int> hl= h; //Felizes caso v seja feliz
    vector<int> custol = custo; //custo caso v seja feliz
    vector<bool> coloridol = colorido; //Coloridos caso v seja feliz
    //unordered_set<int> fl = f;  //Coloríveis caso v seja feliz
    unordered_set<int> hBl = hB;//Incoloríveis caso v seja feliz
    vector<int> deletar_f;
    
    mt19937 gen(seed); //Seed não definida

    //Obtendo os candidatos à v: os de menor custo/grau
    for (auto it = f.begin(); it != f.end(); ) {
   

        if (colorido[*it]){
            if (custo[*it]<graumin){ // se custo == 0 ja colocar em hl e tirar de f, nao vai para candidatos
                if (custo[*it] == 0){
                	hl.insert(*it);
                	h.insert(*it);
                	deletar_f.push_back(*it);
                	}
                else{
                	graumin = custo[*it];
                	candidatos = {*it};
                	}
                }
            else{
            	if (custo[*it] > k){
                	hBl.insert(*it);
                	hB.insert(*it);
                	deletar_f.push_back(*it);            		
            		}
            	else{
		        if (custo[*it]==graumin){
		        candidatos.push_back(*it);
		        }// add else p/ ver se custo > k e tirar de f e talvel (verificar) colocar em hB
		     }
            }
        }else{
            if (custo[*it]+1<graumin){
                    graumin = custo[*it]+1;
                    candidatos = {*it};
            }else{
            	if (custo[*it]+1 > k){
                	hBl.insert(*it);
                	hB.insert(*it);
                	deletar_f.push_back(*it);            		
            		}
            	else{
                	if (custo[*it]+1==graumin){
                    		candidatos.push_back(*it);
                    	}// add else p/ ver se custo > k e tirar de f e talvel (verificar) colocar em hB
                    }
            }
        }
        ++it;
        }

    if(deletar_f.size() > 0){
    	if (h.size() > bestsol){
    		bestsol = h.size();
    		}
	for (int fv : deletar_f){
	 f.erase(fv);
	 //fl.erase(fv);
	}
     //eliminando os vértices que n
	}
    //Randomização da escolha do candidato para V
    uniform_int_distribution<> dist(0, candidatos.size()-1);
    if(graumin>k){ 
        return {nodes,{bestsol,{h,colorido}}};
        //Trata o caso de não ter candidatos.
    }
    v = candidatos[dist(gen)];
    //fl.erase(v); //v é colorido para esse conjunto
    hl.insert(v); // Conjunto que assume que v é feliz

    bool marcador = 0; //Esse marcador faz parte da verificação das perguntas abaixo, puxar a branch com v feliz ou n?
    //Já vai ter a branch em algum canto com o vértice que está em hB feliz, sem alterar nada: Portanto, vai haver uma solução espelhada E pior.

    //Tornando V feliz verificando a vizinhança e ajustando os seus "custos"    
    for (int viz : g[v]){  //para cada vizinho de v e para cada vizinho dos vizinhos de v, reduzir o grau do vizinho em 1, caso nao tenham sido coloridos
        //Se v já tinha sido colorido na etapa anterior, não precisa reavaliar vizinhos de v
        if (colorido[v] == 0){ // Luidi: pode fazer esse teste uma unica vez antes 
            custol[viz]--; //tira 1 grau pois o vértice v ficou feliz, logo, colorido
        }
        if (colorido[viz] == 0){
            coloridol[viz] = 1; //colore ele, e reavalia o grau/custo dos vizinhos, para o mundo de v feliz
            if (hB.count(viz) == 1){
                if (custol[viz] == 0){
                    marcador = 1;
                }
            }
            //aqui, verificar se o vizinho pertence a hB e depois se tem custo 0
            for (int vizviz : g[viz]){
                custol[vizviz]--;
                if (hB.count(vizviz) == 1){
                    if (custol[vizviz] == 0){
                        if (colorido[vizviz] == 1){
                            marcador = 1;
                        }
                    }
                }
                //perguntar se o vizinho do vizinho pertence a hB e depois se tem custo 0 E é colorido
            }
                //se uma dessas perguntas é positiva, já matar a branch com o vértice v feliz (certo? acho que sim, é o único caso que vai ter uma solução igual no outro lado)
        }
    }
    coloridol[v] = 1;

    custol[v] = 0;
    f.erase(v);//Seja para o caso de que v é feliz ou não é feliz, v não será colorível: a cada chamada, se acaba com um v
    hB.insert(v);
    //Reavaliando os coloríveis e não coloríveis para o caso de que v foi feito feliz : não preciso, a próxima iteração já faz isso: vou trocar essa iteração por 1 nó a mais.
    
    if (colorido[v] == false)//Considera se v foi colorido nessa etapa ou em uma anterior
    	add = 1;
    else
    	add = 0;
    //se algum vizinho de v foi colorido antes, o custo[v] já é ajustado
    /*for (auto it = f.begin(); it != f.end(); ) {
        if (custol[*it]>(k-custo[v]+add)){ 
            fl.erase(*it);
            hBl.insert(*it);
            }
        ++it;
    }*/
    
    bestsol = max(bestsol,static_cast<int>(hl.size()));//a bestsol é o melhor valor obtido até então
    int melhor;
    int nodes_bra= 0;//nodes explorados no branch a
    int nodes_brb=0;//nodes explorados no branch b
    pair<int,pair<unordered_set<int>,vector<bool>>> getter; //serve para pegar o retorno da função, somente.
    unordered_set<int> solucao1 = hl; // para poder retornar o conjunto-solução
    vector<bool>colorido1 = colorido1;
    unordered_set<int> solucao2 = hl;
    vector<bool>colorido2 = coloridol;
    if ((hl.size() + f/*l*/.size()) > (bestsol)){ //Considero v feliz 
            if (marcador == 0){
               tie(nodes_bra,getter) = BT(g,custol,k-(custo[v]+add),hl,hBl,f/*l*/,coloridol,clock_start,limit,bestsol,nodes);
                melhor = getter.first;
                solucao1 = getter.second.first;
                colorido1 = getter.second.second;
                bestsol = max(bestsol,melhor);//a bestsol é o melhor valor obtido até então, é sempre o "melhor" isso é apenas pra redefinir o bestsol
            }
    }
    //Considerando v não feliz    
    if ((h.size()+f.size())>(bestsol)){
	    tie(nodes_brb,getter) = (BT(g,custo,k,h,hB,f,colorido,clock_start,limit,bestsol,nodes)); 
	    melhor = getter.first;
	    solucao2 = getter.second.first;
        colorido2 = getter.second.second;
	}
        //não tornei ninguém feliz para essa etapa: considera os mesmos conjuntos, retirando v de f e introduzindo em hB
        //hB não é necessário no geral

    if (static_cast<int>(solucao1.size())>static_cast<int>(solucao2.size())){

        return {nodes_bra+nodes_brb+1, {bestsol,{solucao1,colorido1}}};    
    }

    return {nodes_bra+nodes_brb+1, {bestsol,{solucao2,colorido2}}};
}

//A primeira avaliação do hB e do h faço aqui.
pair<int,pair<int,pair<unordered_set<int>,vector<bool>>>> felicidade_maxima_backtrack(grafo g,vector<int> custo, int k, clock_t clock_start,int limit, int seed){
    unordered_set<int> h;
    unordered_set<int> hB;
    unordered_set<int> coloriveis;
    vector<bool> coloridos(g.size(),0);
    //Etapa para a primeira avaliação se o vértice pode ser feliz ou não
    for (int i = 0;i<g.size();i++){
        if (custo[i]+1 > k){
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


