#include "gera_grafos.hpp"
#include <random>
#include <cstdlib>  // For system()

using namespace std;
typedef vector<vector<int>> grafo;

void gera_txt(int nvert, float preco, float numco){
    float p = (preco/nvert);
    cout << "Qual é o tipo do grafo: R, D (R é random, D é d-regular)"<< endl; //deixei o outro tipo de lado, não entendi bem a lógica
    string tipo;
    cin >> tipo;
    string parametro4;

    string comeco = "gerador\\gen.exe";
    string parametro1 = " -n " + to_string(nvert); //parâmetro que dá a quantidade de arestas
    string parametro2 = " -p " + to_string(p);//parâmetro de porcentagem de cores iniciais precoloridas
    string parametro3 = " -k " + to_string(numco); //parâmetro que da o número de cores
    if (tipo.c_str() ==  "D"){
        int d_reg;
        cout << "Qual o grau de cada vértice" << endl;
        cin >> d_reg;
        parametro4 = " -" + tipo + " " + to_string(d_reg);
    }
    else{
        double r_reg;
        cout << "Qual a probabilidade de gerar uma aresta (de 0 até 1)" << endl;
        cin >> r_reg;
        parametro4 = " -" + tipo + " " +to_string(r_reg);
    }
    string fullcommand = comeco + parametro1 + parametro2 + parametro3 + parametro4;//comando para gerar o grafo
    cout<< fullcommand<< endl;
    system(fullcommand.c_str());
}
// --------------------------------------------------------------------------------------------------------------------------------
pair<grafo,vector<int>> txtparagrafo(int nvert){
    grafo g(nvert, vector<int>(nvert,0));
    vector<int> cores(nvert);
    ifstream file("graph.txt");
    string line;
    while (getline(file,line)){
        if (line[0] == 'e'){ //representa a aresta no txt
            for (int i = 0; i<line.size(); i++){
                if (line[i] == ' '){
                    for (int j = i+1;j<line.size();j++){
                        if (line[j]== ' '){
                            g[stoi(line.substr(i,j))-1][stoi(line.substr(j))-1] = 1;
                            g[stoi(line.substr(j))-1][stoi(line.substr(i,j))-1] = 1;
                            break;
                        }
                    }
                    break;
                }
            }
        }
        if (line[0] == 'n'){ //representa a cor no txt
            for (int i = 0; i<line.size(); i++){
                if (line[i] == ' '){
                    for (int j = i+1;j<line.size();j++){
                        if (line[j]== ' '){
                            cores[stoi(line.substr(i,j))-1] = stoi(line.substr(j));
                            break;
                        }
                    }
                    break;
                }
            }
        }
        else{//as outras informações não importam, a princípio
            continue;
        }
    }
    return {g,cores};
}    
// --------------------------------------------------------------------------------------------------------------------------------
pair<grafo,vector<int>> gera_grafo(int nvert, float preco, float numco){
    grafo g;
    vector<int> cores;
    gera_txt(nvert,preco,numco); //gera o grafo
    tie(g,cores) = txtparagrafo(nvert);//le o txt gerado pelo grafo

    return {g,cores}; //retorna o grafo e as cores
}



