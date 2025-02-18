#include "gera_grafos.hpp"
//media desvio padrao para todas as diferentes seeds, gerar algumas por vez a heurística.
//marcar os nós do normal
//testar para todos, com tempo limitado.
//nós explorados para todos
// Alias para lista de adjacência
using namespace std;
typedef vector<vector<int>> grafo;

pair<int,pair<grafo,vector<int>>> leitor_grafos(string path){
    ifstream file(path);
    string line;
    grafo g;
    int numvert;
    int numarest;
    int k;
    vector<int> graus;
    while (getline(file, line)){
        if (line[0] == 'p'){//a razão do valor 7 se dá pelo formato da instância gerada. Isso facilita a obtenção dos dados
            int primeiro = 0;
            for (int i = 7;i<line.size();i++){ 
                if (line[i] == ' ' and primeiro == 0){
                    primeiro = i;
                    continue;
                }
                if (line[i] == ' '){
                    numvert = stoi(line.substr(7,primeiro-7));
                    numarest = stoi(line.substr(primeiro,i-primeiro));
                    k = stoi(line.substr(i));
                    g.resize(numvert);
                    graus.resize(numvert,0);
                }

            }
        }
        if (line[0] == 'e'){
            for (int i = 2 ; i<line.size();i++){//o valor 2 também é por causa da formatação
                if (line[i] == ' '){
                    int a = stoi(line.substr(2,i-2))-1;
                    int b = (stoi(line.substr(i+1))-1);
                    g[stoi(line.substr(2,i-2))-1].push_back(stoi(line.substr(i+1))-1);
                    g[stoi(line.substr(i+1))-1].push_back(stoi(line.substr(2,i-2))-1);
                    graus[stoi(line.substr(i+1))-1] +=1;
                    graus[stoi(line.substr(2,i-2))-1] +=1;
                }
            }
        }
    }
    return {k,{g,graus}};

}
