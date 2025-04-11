
#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>

#include "vin.hpp"

#include "igraph.hpp"
#include "matrixgraph.hpp"
#include "listgraph.hpp"

#include <cassert>
#include <unistd.h>

using namespace std;

void foo(IGraph& graph)
{

	auto bar = graph.bfs(6);

	for (auto no : bar)
	{
		printf("%i\n", no);
	}


}

void printg(IGraph& graph)
{

	auto outerdfsi = graph.dfs(0);

	for (auto u : outerdfsi)
	{
		std::stringstream graphtext;

		const char* edge;

		if (graph.dir())
		{
			graphtext << "digraph out {\n";
			edge = "->";
		}
		else
		{
			graphtext << "graph out {\n";
			edge = "--";
		}

		auto bfsi = graph.bfs(0);

		assert(bfsi.begin() != bfsi.end());

		for (auto v : bfsi)
		{
			auto nei = graph.retornarVizinhos(v);

			auto lbl = [&graph](id_t x) {
				return "\"" + std::to_string(x) + ": " + *graph.labelVertice(x) + "\"";
			};

			const char* attrib;

			if (u == v)
				attrib = " [style=filled color=black fontcolor=white] ";
			else
				attrib = "";

			graphtext << lbl(v) << attrib << "\n";

			if (nei) for (auto n : *nei)
				graphtext << lbl(v) << edge << lbl(n) << "\n";
		}

		graphtext << "}";

		std::fstream out;
		out.open("out.txt", std::fstream::out | std::fstream::trunc);

		out << graphtext.str();

		out.flush();
		out.close();

		// TODO: Make this not suck
		system("dot -Tpng out.txt > out.png");

		sleep(2);
	}

	return;
}

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
				"  11. Imprimir Dijkstra\n\n"
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
                weight_t peso = 1;
                int origem  = vin::ask<int>("Index do vértice origem: ");
                int destino = vin::ask<int>("Index do vértice destino: ");

                if(grafo.pond())
					peso = vin::ask<weight_t>("Peso da Aresta: ");

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
				printg(grafo);
                break;
            }

            case 0: {
                return;
            }

            case 10: {
				std::cout << VT_CLEAR;
                break;
            }

			case 11: {

				int origem  = vin::ask<int>("\nVertice de origem: ");
				
			try {
				std::vector<std::vector<id_t>> caminhos = grafo.dijkstra(origem);
				for(id_t i = 0; i < caminhos.size(); i++){
					std::cout << "\n" << i << "\t" << " -> |";
					for(id_t j = 0; j < caminhos[i].size(); j++) {
						std::cout << caminhos[i].at(j) << " | ";
						
					}
					std::cout << "\n\n";
				}
			} catch (const std::exception& e) {
				std::cout << "\n\nERRO - ERRo no DIJKSTRA - ERRO\n\nERRO - ERRo no DIJKSTRA - ERRO";
				std::cout << "\n Exceção " << e.what();
			}
			
			
			
			}
        }

    }
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
		grafo = std::make_unique<ListGraph>(ponderado, direcionado);

	std::cout << VT_CLEAR << "Grafo Criado\n";

	menu(*grafo);

	vin::compat_epilogue();

	return EXIT_SUCCESS;
}
