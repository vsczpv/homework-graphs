// ninja -C build
// .build/graphs in.txt


#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>

#include "vin.hpp"

#include "igraph.hpp"
#include "igraphloader.hpp"

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

void view_path(IGraph& graph, id_t rootid, std::deque<id_t> path)
{

	for (auto u : path)
	{
		std::stringstream graphtext;

		const char* edge;

		if (graph.dir())
		{
			graphtext << "strict digraph out {\n";
			edge = "->";
		}
		else
		{
			graphtext << "strict graph out {\n";
			edge = "--";
		}

		auto bfsi = graph.bfs(rootid);

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
			{

				std::ostringstream weightos;

				if (graph.pond())
					weightos <<  " [label = " << *graph.pesoAresta(v, n) << "]";

				graphtext << lbl(v) << edge << lbl(n) << std::move(weightos).str() << "\n";
			}
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
		        "  10. Limpar Tela\n"
				"  11. Imprimir Dijkstra\n"
				"  12. Visualizar BFS\n"
				"  13. Visualizar DFS\n"
				"  14. Imprimir Caminhos Dijkstra\n"
				"  15. Visualizar Grafo\n"
				"  16. Visualizar Dijkstra\n"
				"  17. Visualizar Caminhos Dijkstra\n"
				"  18. Visualizar Coloração Força Bruta\n"
				"  19. Visualizar Coloração Welsh Powell\n"
				"  20. Visualizar Coloração DSATUR\n"
				"  21. Visualizar Coloração Sem Ordem\n"
				"  22. Fluxo Máximo (Fork-Fukerson)\n"
				"  23. Imprimir Arestas\n"
				"  24. Otimizar Fluxo Máximo\n"
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

				int origem = vin::ask<int>("\nVertice de origem: ");

				grafo.dijkstra(origem).dbgprint();

				break;
			}

			case 12:
			{

				int origem = vin::ask<int>("\nVertice de origem: ");

				std::deque <id_t> path = {};

				auto bfs = grafo.bfs(origem);

				for (auto v : bfs) path.push_back(v);

				view_path(grafo, origem, path);

				break;
			}

			case 13:
			{

				int origem = vin::ask<int>("\nVertice de origem: ");

				std::deque <id_t> path = {};

				auto dfs = grafo.dfs(origem);

				for (auto v : dfs) path.push_back(v);

				view_path(grafo, origem, path);

				break;
			}

			case 14:
			{

				int origem = vin::ask<int>("\nVertice de origem: ");

				auto cam = grafo.dijkstra_caminhos(origem);

				for (auto [k, v] : cam)
				{
					std::cout << k << ":\t\t";
					for (auto n : v) {
						std::cout << n << " > ";
					}
					std::cout << k << std::endl;
				}

				break;
			}

			case 15:
			{
				id_t origem = vin::ask<int>("\nVertice de origem: ");

				std::deque <id_t> path = { origem };

				view_path(grafo, origem, path);

				break;
			}

			case 17:
			{

				int origem = vin::ask<int>("\nVertice de origem: ");

				auto cam = grafo.dijkstra_caminhos(origem);

				std::cout << "Caminhos encontrados: ";

				for (auto [k, v] : cam)
					std::cout << k << " ";

				int destino = vin::ask<int>("\nCaminho: ");

				std::deque<id_t> select;

				std::move(
					begin(cam[destino]),
					end  (cam[destino]),
					back_inserter(select)
				);

				select.push_back(destino);

				view_path(grafo, origem, select);

				break;
			}

			case 18:
			{
				BrutForce bruit_force = grafo.bruit_force();
				bruit_force.color_graph();
				bruit_force.print_output_list();
				std::cout << "\n------------------------------------------------------------------------------\n";
				std::cout << "Tempo de execução: " << bruit_force.get_burst_time() << "\n" << "Quantidade de cores: " << bruit_force.get_colors_number() << "\n";
				
				break;
			}
			case 19:
			{
				WelshPowell wp = grafo.welsh_powell();
				wp.color_graph();
				wp.print_output_list();
				std::cout << "\n------------------------------------------------------------------------------\n";
				std::cout << "Tempo de execução: " << wp.get_burst_time() << "\n" << "Quantidade de cores: " << wp.get_colors_number() << "\n";
				

				break;
			}

			case 20:
			{
				DSATUR ds = grafo.dsatur();
				ds.color_graph();
				ds.print_output_list();
				std::cout << "\n------------------------------------------------------------------------------\n";
				std::cout << "Tempo de execução: " << ds.get_burst_time() << "\n" << "Quantidade de cores: " << ds.get_colors_number() << "\n";
				break;
			}

			case 21:
			{
				NoOrder no = grafo.noorder();
				no.color_graph();
				no.print_output_list();
				std::cout << "\n------------------------------------------------------------------------------\n";
				std::cout << "Tempo de execução: " << no.get_burst_time() << "\n" << "Quantidade de cores: " << no.get_colors_number() << "\n";
				break;
			}

			case 22:
			{
				if ( (grafo.pond() && grafo.dir()) == false )
				{
					std::cout << "Grafo não é ponderado e direcionado, abortando." << std::endl;
				}

				else
				{
					id_t F = vin::ask<id_t>("\nVertice Fonte: ");
					id_t S = vin::ask<id_t>("\nVertice Sorvedor: ");

					FordFn fn = FordFn(grafo, F, S);

					weight_t max = fn.max();

					std::cout << "O fluxo máximo do grafo é " << max << "." << std::endl;
				}

				break;
			}
			case 23:
			{
				auto ars = grafo.getArestas();

				for (auto a : ars) {
					std::cout << std::get<0>(a) << " -> " << std::get<1>(a) << ": " << std::get<2>(a) << std::endl;
				}

				std::cout << "total: " << ars.size() << std::endl;

				break;
			}
			case 24:
			{

				id_t F = vin::ask<id_t>("\nVertice Fonte: ");
				id_t S = vin::ask<id_t>("\nVertice Sorvedor: ");

				auto bg = Optimizer(grafo).optimize(F, S);

				std::cout << "Melhor Fluxo: " << FordFn(*bg, F, S).max() << std::endl;

//				std::deque <id_t> path = { F };
//
//				view_path(*bg, F, path);

				delete bg;

				break;
			}
        }

    }
}

int main(int argc, char* argv[]) {

	if (argc == 1)
	{
		std::cout << "Por favor forneça um arquivo." << std::endl;
		return EXIT_FAILURE;
	}

	vin::compat_prologue();

	/* Coleta especificações do Grafo */

	std::cout << "Bem vindo ao Criador de Grafos\n"
	          << "Escolha o tipo de grafo que desejas criar:\n\n"
	          << "  0. Lista de Referência\n"
	          << "  1. Matriz de Adjacência\n" << std::endl;

	auto type = igraphtype_from_bool(vin::ask<bool>("=> "));

	auto grafo = std::unique_ptr<IGraph>(IGraphLoader(type).from_file(argv[1]));

	if (grafo.get() == NULL)
	{
		std::cerr << "Erro ao carregar o grafo." << vin::endl;
		return EXIT_FAILURE;
	}

	std::cout << VT_CLEAR << "Grafo Carregado" << std::endl;

	menu(*grafo);

	vin::compat_epilogue();

	return EXIT_SUCCESS;
}
