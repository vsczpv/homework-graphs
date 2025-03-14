
#include <iostream>
#include <vector>

#include "graph.hpp"

using namespace std;





//bool inserirVertice(string label);
//bool removerVertice(int indice);
//bool inserirAresta(int origem, int destino, int peso = 1);
//bool removerAresta(int origem, int destino);
//string labelVertice(int indice);
//bool existeAresta(int origem, int destino);
//float pesoAresta(int origem, int destino);
//vector<int> retornarVizinhos(int vertice);
//void imprimeGrafo();

//Remover as arestas as quais tem o vértice removido como destino










void menu(Grafo *grafo) {// recebe ponteiro para o grafo
    while(true) {

        cout << "O que desejas?\n\n";
        cout << "  1. Inserir Vértice\n  2. Remover Vértice\n  3. Inserir Aresta\n  4. Remover Aresta\n  5. Ver Label do Vértice\n  6. Ver se Existe Aresta\n  7. Ver Peso da Aresta\n  8. Retornar Vizinhos do Vértice\n  9. Imprimir Grafo\n  0. Sair\n 10. Limpar Tela\n\n: ";

        int opcao;
        cin >> opcao;
        while(cin.fail() || opcao < 0 || opcao > 10) {
            cout << "Digite uma das opções acima\n";
            cin.clear();
            string c;
            cin >> c;
            cin >> opcao;
        }

        //executa a opção escolhida
        switch(opcao){
            case 1: {
                cout << "Digite um label para seu novo vértice\n";
                string label;
                cin >> label;
                if(!(grafo->inserirVertice(label))) { cout << "Algo deu errado, tente novamente mais tarde\n"; }
                break;
            }

            case 2: {
                cout << "Digite o indice do vértice que deseja remover\n";

                int indice;
                cin >> indice;
                while(cin.fail()) {
                    cout << "Digite uma das opções acima\n";
                    cin.clear();
                    string c;
                    cin >> c;
                    cin >> indice;
                }
                if(!(grafo->removerVertice(indice))) { cout << "Vertice não encontrado\n"; }
                break;
            }

            case 3: {
                int origem, destino, peso=1;
                cout << "Index do vértice origem: ";
                cin >> origem;
                while(cin.fail()) {
                    cout << "Digite um número\n";
                    cin.clear();
                    string c;
                    cin >> c;
                    cin >> origem;
                }
                cout << "\nIndex do vértice destino: ";
                cin >> destino;
                while(cin.fail()) {
                    cout << "Digite um número\n";
                    cin.clear();
                    string c;
                    cin >> c;
                    cin >> destino;
                }
                if(grafo->ponderado) {
                    cout << "\nPeso da Aresta: ";
                    cin >> peso;
                    while(cin.fail()) {
                        cout << "Digite um número\n";
                        cin.clear();
                        string c;
                        cin >> c;
                        cin >> peso;
                    }
                }
                if(!grafo->inserirAresta(origem, destino, peso)) { cout << "Algo deu errado, tente novamente\n"; }
                break;
            }

            case 4: {
                int origem, destino;
                cout << "Index do vértice origem: ";
                cin >> origem;
                while(cin.fail()) {
                    cout << "Digite um número\n";
                    cin.clear();
                    string c;
                    cin >> c;
                    cin >> origem;
                }
                cout << "\nIndex do vértice destino: ";
                cin >> destino;
                while(cin.fail()) {
                    cout << "Digite um número\n";
                    cin.clear();
                    string c;
                    cin >> c;
                    cin >> destino;
                }
                if(!grafo->removerAresta(origem, destino)) { cout << "Algo deu errado, tente novamente\n"; }
                break;
            }

            case 5: {
                int index;
				cout << "Index do vértice procurado: ";
				cin >> index;
				while(cin.fail()) {
					cout << "Digite um número\n";
					cin.clear();
					string c;
					cin >> c;
					cin >> index;
				}
                cout << grafo->labelVertice(index) << endl;
                break;
            }

            case 6: {
                int origem, destino;
                cout << "Index do vértice origem: ";
                cin >> origem;
                while(cin.fail()) {
                    cout << "Digite um número\n";
                    cin.clear();
                    string c;
                    cin >> c;
                    cin >> origem;
                }
                cout << "\nIndex do vértice destino: ";
                cin >> destino;
                while(cin.fail()) {
                    cout << "Digite um número\n";
                    cin.clear();
                    string c;
                    cin >> c;
                    cin >> destino;
                }
                cout << grafo->existeAresta(origem, destino) << "\n";
				break;
            }

            case 7: {
                int origem, destino;
                cout << "Index do vértice origem: ";
                cin >> origem;
                while(cin.fail()) {
                    cout << "Digite um número\n";
                    cin.clear();
                    string c;
                    cin >> c;
                    cin >> origem;
                }
                cout << "\nIndex do vértice destino: ";
                cin >> destino;
                while(cin.fail()) {
                    cout << "Digite um número\n";
                    cin.clear();
                    string c;
                    cin >> c;
                    cin >> destino;
                }
                cout << grafo->pesoAresta(origem, destino) << "\n";
				break;
            }

            case 8: {
                int vertice;
                cout << "Index do vértice origem: ";
                cin >> vertice;
                while(cin.fail()) {
                    cout << "Digite um número\n";
                    cin.clear();
                    string c;
                    cin >> c;
                    cin >> vertice;
                }
                vector<int> vizinhos = grafo->retornarVizinhos(vertice);
                for(int vizinho: vizinhos) {
                    cout << vizinho << "\n";
                }
				break;
            }

            case 9: {
                grafo->imprimeGrafo();
                break;
            }

            case 0: {
                return;
            }

            case 10: {
//              system("cls");
				system("clear");
                break;
            }
        }

    }
}

int main() {

//Coleta especificações do Grafo
    bool direcionado;
    bool ponderado;

    cout << "Bem vindo ao Criador de Grafos\n";
    cout << "\n\nEscolha o tipo de grafo que deseja criar:\n";
    cout << "  0. Grafo Não Direcionado\n";
    cout << "  1. Grafo Direcionado\n";
    cin >> direcionado;
    while(cin.fail()) {
        cout << "Digite apenas 0 ou 1\n";
        cin.clear();
        string c;
        cin >> c;
        cin >> direcionado;
    }


    cout << "\n\nEscolha o tipo de grafo que deseja criar:\n";
    cout << "  0. Grafo Não Ponderado\n";
    cout << "  1. Grafo Ponderado\n";
    cin >> ponderado;
    while(cin.fail()) {
        cout << "Digite apenas 0 ou 1\n";
        cin.clear();
        string c;
        cin >> c;
        cin >> ponderado;
    }

//Cria o Grafo
    Grafo *grafo = new Grafo;
    grafo->direcionado = direcionado;
    grafo->ponderado = ponderado;

    cout << " Grafo Criado\n";

//    system("cls");
    menu(grafo);//passa o endereço do grafo




}
