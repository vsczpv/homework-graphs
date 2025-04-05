
#ifndef LIST_GRAPH_HPP_
#define LIST_GRAPH_HPP_

#include <iostream>
#include <vector>

#include "igraph.hpp"


// Features to Inplement
// 1. Passar a usar ID; Na exclusão de um vértice, apagar ele apenas se for o vértice final, se for o vértice do eio, use esse espaço para colocar futuros vértices; Arestas devem conter os IDs, não poteiros
// 2. edgesToIt should be simple vertices, not a real edge
// 2. Before checking, existeAresta should verify witch is shorter, (1) edgesFromIt from origin or (2) edgesToIt from destination
// 3. Better edges organization, to save time searching it


// Implementar iterador XX (aprender a usar o implementado)
// Implementar busca em largura XX (aprender a usar o implementado)
// Implementar busca em profundidade
// Implementar Dijkstra


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
            this->edgesFromIt.push_back(a);
            this->edgesFromItCount++;

            destination->inserirArestaToIt(this); // Insere registro dessa aresta também no vértice destino
            return false;
        }
        
        bool inserirArestaToIt(Vertex *origin, weight_t weight = 1) { // Registra aresta que aponta para ele
            Edge a;
            a.vertex = origin;
            a.weight = weight;
            this->edgesToIt.push_back(a);
            this->edgesToItCount++;
            return false;
        }

        bool removerAresta(Vertex *destination) {

            
            for(id_t i = 0; i < this->edgesFromItCount; i++) {
                if(this->edgesFromIt.at(i).vertex == destination) {
    //              this->arestas.erase(arestas.begin() + i);
                    
                    this->edgesFromIt.erase(edgesFromIt.cbegin() + static_cast<signed>(i));//(signed) i);
                    edgesFromItCount--;

                    destination->removerArestaToIt(this); // Remove também o registro da aresta no vértice destino
                    return false;
                }
            }
            return true;
        }

        bool removerArestaToIt(Vertex *origin) { // Remove registro de aresta que aponta para ele
            for(id_t i = 0; i < this->edgesToItCount; i++) {
                if(this->edgesToIt.at(i).vertex == origin) {
    //              this->arestas.erase(arestas.begin() + i);
                    
                    this->edgesToIt.erase(edgesToIt.cbegin() + static_cast<signed>(i));//(signed) i);
                    edgesToItCount--;
                    return false;
                }
            }
            return true;
        }

        bool existeAresta(Vertex *destination) {
            for(id_t i = 0; i < this->edgesFromItCount; i++) {
                if(this->edgesFromIt.at(i).vertex == destination) {
                    return true;
                }
            }
            return false;
        }


        std::optional<weight_t> pesoAresta(Vertex *destination) {
            for(id_t i = 0; i < this->edgesFromItCount; i++) {
                if(this->edgesFromIt.at(i).vertex == destination) {
                    return this->edgesFromIt.at(i).weight;
                }
            }
            return std::nullopt;
        }

        std::vector<Vertex*> retornarVizinhos() {
            try {
                std::vector<Vertex*> vizinhos;
                for(id_t i = 0; i < this->edgesFromItCount; i++) { // Coleta o label dos vizinhos
                    vizinhos.push_back(this->edgesFromIt.at(i).vertex);
                }
                return vizinhos;

            } catch(...) {
                std::cout << "\nErro em pegar vizinhos From It";
                std::vector<Vertex*> vizinhos;
                return vizinhos;
            }
        }

        std::vector<Vertex*> retornarVizinhosToIt() {
            try {
                std::vector<Vertex*> vizinhos;
                for(id_t i = 0; i < this->edgesToItCount; i++) { // Coleta o label dos vizinhos
                    vizinhos.push_back(this->edgesToIt.at(i).vertex);
                }
                return vizinhos;

            } catch(...) {
                std::cout << "\nErro em pegar vizinhos To It";
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
