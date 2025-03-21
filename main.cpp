
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <fstream>

#include "vin.hpp"

#include "igraph.hpp"
#include "matrixgraph.hpp"
#include "listgraphwrapper.hpp"

using namespace std;

/* recebe referencia para o grafo */
void menu(IGraph& grafo) {
    while(true) {

		cout << "O que desejas?\n\n"
		        "   1. Inserir Vértice\n"
		        "   2. Remover Vértice\n"
		        "   3. Inserir Aresta\n"
		        "   4. Remover Aresta\n"
		        "   5. Ver Label do Vértice\n"
		        "   6. Ver se Existe Aresta\n"
		        "   7. Ver Peso da Aresta\n"
		        "   8. Retornar Vizinhos do Vértice\n"
		        "   9. Imprimir Grafo\n"
		        "  10. Limpar Tela\n\n: "
		        "   0. Sair\n";

		int opcao = vin::ask<int>("=> ");

        /* executa a opção escolhida */
        switch(opcao){
            case 1: {
                cout << "Digite um label para seu novo vértice\n";
                string label;
                cin >> label;
                if(grafo.inserirVertice(label)) { cout << "Algo deu errado, tente novamente mais tarde\n"; }
                break;
            }

            case 2: {
                cout << "Digite o indice do vértice que deseja remover\n";

                int indice = vin::ask<int>("=> ");

                if(grafo.removerVertice(indice)) { cout << "Vertice não encontrado\n"; }
                break;
            }

            case 3: {
                int peso = 1;
                int origem  = vin::ask<int>("Index do vértice origem: ");
                int destino = vin::ask<int>("Index do vértice destino: ");

                if(grafo.pond())
					peso = vin::ask<int>("Peso da Aresta: ");

                if(grafo.inserirAresta(origem, destino, peso)) { cout << "Algo deu errado, tente novamente\n"; }
                break;
            }

            case 4: {
				int origem  = vin::ask<int>("Index do vértice origem: ");
				int destino = vin::ask<int>("Index do vértice destino: ");

                if(grafo.removerAresta(origem, destino)) { cout << "Algo deu errado, tente novamente\n"; }
                break;
            }

            case 5: {
                int index = vin::ask<int>("Index do vértice procurando: ");

				auto label = grafo.labelVertice(index);

				if (label)
					cout << *label << endl;
				else
					cerr << "Erro ao ler label" << endl;

                break;
            }

            case 6: {
				int origem  = vin::ask<int>("Index do vértice origem: ");
                int destino = vin::ask<int>("Index do vértice destino: ");

                cout << grafo.existeAresta(origem, destino) << "\n";
				break;
            }

            case 7: {
				int origem  = vin::ask<int>("Index do vértice origem: ");
				int destino = vin::ask<int>("Index do vértice destino: ");

                auto peso = grafo.pesoAresta(origem, destino);

				if (peso)
					cout << *peso << "\n";
				else
					cerr << "Erro ao ler peso" << endl;

				break;
            }

            case 8: {
                int vertice = vin::ask<int>("Index do vértice origem: ");

                auto vizinhos = grafo.retornarVizinhos(vertice);

				if (vizinhos) for (auto vizinho: *vizinhos) {
                    cout << vizinho << "\n";
                } else {
					cerr << "Erro ao ler vizinhos" << endl;
				}

				break;
            }

            case 9: {
                grafo.imprimeGrafo();
                break;
            }

            case 0: {
                return;
            }

            case 10: {
				std::cout << VT_CLEAR;
                break;
            }
        }

    }
}

void dot(IGraph& graph) {
	std::stringstream graphtext;

	graphtext << "digraph out {";

	graphtext << "a -> b\n"
	          << "b -> c\n"
	          << "c -> a\n"
			  << "b -> d\n"
			  << "b -> c\n";

	graphtext << "}";

	std::fstream out;
	out.open("out.txt", std::fstream::out | std::fstream::trunc);

	out << graphtext.str();

	out.flush();
	out.close();

	// TODO: Make this not suck
	system("dot -Tpng out.txt > out.png");
}

int main() {

	vin::compat_prologue();

	/* Coleta especificações do Grafo */

	std::cout << "Bem vindo ao Criador de Grafos\n"
	          << "Escolha o tipo de grafo que desejas criar:\n\n"
	          << "  0. Lista de Referência\n"
	          << "  1. Matriz de Adjacência\n" << std::endl;

	auto matrix = vin::ask<bool>("=> ");


    std::cout << "\n\nEscolha o tipo de grafo que deseja criar:\n\n"
              << "  0. Grafo Não Direcionado\n"
              << "  1. Grafo Direcionado\n" << std::endl;

	auto direcionado = vin::ask<bool>("=> ");

	std::cout << "\n\nEscolha o tipo de grafo que deseja criar:\n\n"
	          << "  0. Grafo Não Ponderado\n"
	          << "  1. Grafo Ponderado\n" << std::endl;

	auto ponderado = vin::ask<bool>("=> ");

	/* Cria o Grafo */

	std::unique_ptr <IGraph> grafo;

	if (matrix)
		grafo = std::make_unique<MatrixGraph>(ponderado, direcionado);
	else
		grafo = std::make_unique<ListGraphWrapper>(ponderado, direcionado);

	std::cout << VT_CLEAR << "Grafo Criado\n";

	menu(*grafo);

	vin::compat_epilogue();

	return EXIT_SUCCESS;
}
