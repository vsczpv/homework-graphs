#include "listgraphwrapper.hpp"

#ifndef GRAPH_HPP_
#define GRAPH_HPP_

#include <iostream>
#include <vector>

#include "igraph.hpp"



class Graph{

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
        id_t verticesCont;
        bool m_dir, m_pond;

public:

        Graph(bool pond, bool dir)
            : vertices  (),
            verticesCont(0),
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




    bool pond(void) {
        return this->pond;
    }

    bool dir(void) {
        return this->dir;
    }

    
    bool inserirVertice(std::string label) {
        try{
            Vertice v;
            v.label = label;
            this->vertices.push_back(v);
            verticesCont++;
            return true;
        } catch (...) {
            return false;
        }
    }

    bool removerVertice(std::string label) {
        try{
            for(id_t i = 0; i < this->verticesCont; i++) {
                if(this->vertices.at(i).label == label) { // encontra o vertice
                    for(id_t j = 0; j < this->verticesCont; j++) { // remove arestas que apontam para ele
                        this->removerAresta(j, i);
                    }


                    this->vertices.erase(this->vertices.begin() + i);
                    this->verticesCont--;
                    return true;
                }
                return false;
            }
        } catch (const std::out_of_range& e){
            std::cout << "ERROR: OUT OF BOUND\n";
            return false;
        }
        return false;
    }

    bool removerVertice(id_t indice) {
        try {
			if (indice >= this->verticesCont)
				return false;

            for(id_t j = 0; j < this->verticesCont; j++) { // remove arestas que apontam para ele
                this->removerAresta(j, indice);
            }
            this->vertices.erase(this->vertices.cbegin() + indice); // remove a vertice
            this->verticesCont--;
            return true;

        } catch(...) {
            return false;
        }
    }

    void imprimeGrafo() {

        try{ // Lista
            std::cout << "\nLista\n";
            for(id_t  i = 0; i < this->verticesCont; i++) {
                std::cout << i << "\t" << this->vertices.at(i).label << " ->  |";
                for(id_t  j = 0; j < this->vertices.at(i).arestasCont; j++) {
                    std::cout << " " << this->vertices.at(i).arestas.at(j).destino->label;
                    if(this->pond) { std::cout << ": " << this->vertices.at(i).arestas.at(j).weight << " |"; } else { std::cout << " |"; }
                }
                std::cout << "\n";
            }
        } catch (...) {
            std::cout << "ERROR: erro na geração da matriz da lista\n";
        }
    }

    bool inserirAresta(id_t origem, id_t destino, int weight = 1) {
        try {

            this->vertices.at(origem).inserirAresta(&this->vertices.at(destino), weight);
            if(!this->dir) {
                this->vertices.at(destino).inserirAresta(&this->vertices.at(origem), weight);
            }
            return true;
        } catch(...) {
            std::cout << "ERRO: Vertices inválidos\n";
            return false;
        }
    }

    bool removerAresta(id_t origem, id_t destino) {
        try {

            this->vertices.at(origem).removerAresta(&this->vertices.at(destino));
            if(!this->dir) {
                this->vertices.at(destino).removerAresta(&this->vertices.at(origem));
            }
            return true;
        } catch(...) {
            std::cout << "ERRO: Vertices inválidos\n";
            return false;
        }
    }

    std::string labelVertice(id_t index) {
        try {
            return this->vertices.at(index).label;
        } catch(...) {
            return "Index Inválido";
        }
    }

    bool existeAresta(id_t origem, id_t destino) {
        try {
            return this->vertices.at(origem).existeAresta(&this->vertices.at(destino));
        } catch(...) {
            std::cout << "Solicitação inválida";
            return false;
        }
    }

    int pesoAresta(id_t origem, id_t destino) {

        try {
            if(this->existeAresta(origem, destino)){
                return this->vertices.at(origem).pesoAresta(&this->vertices.at(destino));
            } else {
                return -1;
            }
        } catch(...) {
            std::cout << "Solicitação inválida";
            return -1;
        }
    }

    std::vector<id_t> retornarVizinhos(id_t vertice) {
        std::vector<id_t> vizinhos;
        std::vector<Vertice*> ptrVizinhos = this->vertices.at(vertice).retornarVizinhos();
        for(Vertice* vertice: ptrVizinhos) {
            for(id_t i = 0; i < this->verticesCont; i++) {
                if(&this->vertices.at(i) == vertice) {
                    vizinhos.push_back(i);
                }
            }
        }
        return vizinhos;
    }

};

#endif // GRAPH_HPP_
