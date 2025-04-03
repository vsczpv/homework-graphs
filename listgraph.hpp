
#ifndef LIST_GRAPH_HPP_
#define LIST_GRAPH_HPP_

#include <iostream>
#include <vector>

#include "igraph.hpp"



class ListGraph : public IGraph {

private:
    struct Vertex {

        std::string label;

        struct Edge {
            Vertex *vertex;
            weight_t weight;
        };

        std::vector<Edge> edgesFromIt;
        id_t edgesFromItCount=0;
        std::vector<Edge> edgesToIt;
        id_t edgesToItCount=0;

        bool inserirAresta(Vertex *destination, weight_t weight = 1) {
            Edge a;
            a.vertex = destination;
            a.weight = weight;
            this->edges.push_back(a);
            this->edgesCount++;
            return false;
        }
        bool removerAresta(Vertex *destination) {
            for(id_t i = 0; i < this->edgesCount; i++) {
                if(this->edges.at(i).vertex == destination) {
    //              this->arestas.erase(arestas.begin() + i);
                    this->edges.erase(edges.cbegin() + static_cast<signed>(i));//(signed) i);
                    edgesCount--;
                    return false;
                }
            }
            return true;
        }

        bool existeAresta(Vertex *destination) {
            for(id_t i = 0; i < this->edgesCount; i++) {
                if(this->edges.at(i).vertex == destination) {
                    return true;
                }
            }
            return false;
        }


        std::optional<weight_t> pesoAresta(Vertex *destination) {
            for(id_t i = 0; i < this->edgesCount; i++) {
                if(this->edges.at(i).vertex == destination) {
                    return this->edges.at(i).weight;
                }
            }
            return std::nullopt;
        }

        std::vector<Vertex*> retornarVizinhos() {
            try {
                std::vector<Vertex*> vizinhos;
                for(id_t i = 0; i < this->edgesCount; i++) { // Coleta o label dos vizinhos
                    vizinhos.push_back(this->edges.at(i).vertex);
                }
                return vizinhos;

            } catch(...) {
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
        bool inserirAresta (id_t A, id_t B, weight_t peso) noexcept override;
        bool removerAresta (id_t A, id_t B)                    noexcept override;
    
        std::optional<std::string> labelVertice(id_t idx) noexcept override;
    
        bool existeAresta(id_t A, id_t B) noexcept override;
    
        std::optional<weight_t> pesoAresta(id_t A, id_t B) noexcept override;
    
        std::optional<std::vector<id_t>> retornarVizinhos(id_t idx) noexcept override;
    
        void imprimeGrafo(void) noexcept override;
    
        bool pond(void) noexcept override;
        bool dir (void) noexcept override;

};

#endif // LIST_GRAPH_HPP_
