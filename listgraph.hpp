#include "listgraphwrapper.hpp"

#ifndef GRAPH_HPP_
#define GRAPH_HPP_

#include <iostream>
#include <vector>

#include "igraph.hpp"



class ListGraph{

private:
    struct Vertex {

        std::string label;

        struct Edge {
            Vertex *origin;
            Vertex *destination;
            int weight;
        };

        std::vector<Edge> edges;
        id_t edgesCount=0;

        bool inserirAresta(Vertex *destination, int weight = 1) {
            Edge a;
            a.origin = this;
            a.destination = destination;
            a.weight = weight;
            this->edges.push_back(a);
            this->edgesCount++;
            return true;
        }
        bool removerAresta(Vertex *destination) {
            for(id_t i = 0; i < this->edgesCount; i++) {
                if(this->edges.at(i).destination == destination) {
    //              this->arestas.erase(arestas.begin() + i);
                    this->edges.erase(edges.cbegin() + static_cast<signed>(i));//(signed) i);
                    edgesCount--;
                    return true;
                }
            }
            return false;
        }

        bool existeAresta(Vertex *destination) {
            for(id_t i = 0; i < this->edgesCount; i++) {
                if(this->edges.at(i).destination == destination) {
                    return true;
                }
            }
            return false;
        }


        int pesoAresta(Vertex *destination) {
            for(id_t i = 0; i < this->edgesCount; i++) {
                if(this->edges.at(i).destination == destination) {
                    return this->edges.at(i).weight;
                }
            }
            return -1;
        }

        std::vector<Vertex*> retornarVizinhos() {
            try {
                std::vector<Vertex*> vizinhos;
                for(id_t i = 0; i < this->edgesCount; i++) { // Coleta o label dos vizinhos
                    vizinhos.push_back(this->edges.at(i).destination);
                }
                return vizinhos;

            } catch(...) {
                std::cout << "ERRO: no retornarVizinhos\n";
                std::vector<Vertex*> vizinhos;
                return vizinhos;
            }
        }


    };

        std::vector<Vertex> vertices;
        id_t verticesCount;
        bool m_dir, m_pond;

public:

        ListGraph(bool pond, bool dir)
            : vertices  (),
            verticesCount(0),
            m_dir       (dir),
            m_pond      (pond)
        {}

        
        bool inserirVertice(std::string label)                 noexcept override;
        bool removerVertice(id_t idx)                          noexcept override;
        bool inserirAresta (id_t A, id_t B, weight_t peso = 1) noexcept override;
        bool removerAresta (id_t A, id_t B)                    noexcept override;
    
        std::optional<std::string> labelVertice(id_t idx) noexcept override;
    
        bool existeAresta(id_t A, id_t B) noexcept override;
    
        std::optional<weight_t> pesoAresta(id_t A, id_t B) noexcept override;
    
        std::optional<std::vector<id_t>> retornarVizinhos(id_t idx) noexcept override;
    
        void imprimeGrafo(void) noexcept override;
    
        bool pond(void) noexcept override;
        bool dir (void) noexcept override;

};

#endif // GRAPH_HPP_
