#ifndef GRAPH_HPP_
#define GRAPH_HPP_

#include <iostream>
#include <vector>

#include "igraph.hpp"

struct Vertice {

    std::string label;

    struct Aresta {
        Vertice *origem;
        Vertice *destino;
        int peso;
    };

    std::vector<Aresta> arestas;
    size_t arestasCont=0;

    bool inserirAresta(Vertice *destino, int peso = 1) {
        Aresta a;
        a.origem = this;
        a.destino = destino;
        a.peso = peso;
        this->arestas.push_back(a);
        this->arestasCont++;
        return true;
    }
    bool removerAresta(Vertice *destino) {
        for(size_t i = 0; i < this->arestasCont; i++) {
            if(this->arestas.at(i).destino == destino) {
//              this->arestas.erase(arestas.begin() + i);
				this->arestas.erase(arestas.cbegin() + static_cast<signed>(i));//(signed) i);
                arestasCont--;
                return true;
            }
        }
        return false;
    }

    bool existeAresta(Vertice *destino) {
        for(size_t i = 0; i < this->arestasCont; i++) {
            if(this->arestas.at(i).destino == destino) {
                return true;
            }
        }
        return false;
    }


    int pesoAresta(Vertice *destino) {
        for(size_t i = 0; i < this->arestasCont; i++) {
            if(this->arestas.at(i).destino == destino) {
                return this->arestas.at(i).peso;
            }
        }
        return -1;
    }

    std::vector<Vertice*> retornarVizinhos() {
        try {
            std::vector<Vertice*> vizinhos;
            for(size_t i = 0; i < this->arestasCont; i++) { // Coleta o label dos vizinhos
                vizinhos.push_back(this->arestas.at(i).destino);
            }
            return vizinhos;

        } catch(...) {
            std::cout << "ERRO: no retornarVizinhos\n";
            std::vector<Vertice*> vizinhos;
            return vizinhos;
        }
    }


};

struct Grafo{
    std::vector<Vertice> vertices;
    size_t verticesCont = 0;
    bool direcionado;
    bool ponderado;

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
            for(size_t i = 0; i < this->verticesCont; i++) {
                if(this->vertices.at(i).label == label) { // encontra o vertice
                    for(size_t j = 0; j < this->verticesCont; j++) { // remove arestas que apontam para ele
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

    bool removerVertice(size_t indice) {
        try {
			if (indice >= this->verticesCont)
				return false;

            for(size_t j = 0; j < this->verticesCont; j++) { // remove arestas que apontam para ele
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
            for(size_t  i = 0; i < this->verticesCont; i++) {
                std::cout << i << "\t" << this->vertices.at(i).label << " ->  |";
                for(size_t  j = 0; j < this->vertices.at(i).arestasCont; j++) {
                    std::cout << " " << this->vertices.at(i).arestas.at(j).destino->label;
                    if(this->ponderado) { std::cout << ": " << this->vertices.at(i).arestas.at(j).peso << " |"; } else { std::cout << " |"; }
                }
                std::cout << "\n";
            }
        } catch (...) {
            std::cout << "ERROR: erro na geração da matriz da lista\n";
        }
    }

    bool inserirAresta(size_t origem, size_t destino, int peso = 1) {
        try {

            this->vertices.at(origem).inserirAresta(&this->vertices.at(destino), peso);
            if(!this->direcionado) {
                this->vertices.at(destino).inserirAresta(&this->vertices.at(origem), peso);
            }
            return true;
        } catch(...) {
            std::cout << "ERRO: Vertices inválidos\n";
            return false;
        }
    }

    bool removerAresta(size_t origem, size_t destino) {
        try {

            this->vertices.at(origem).removerAresta(&this->vertices.at(destino));
            if(!this->direcionado) {
                this->vertices.at(destino).removerAresta(&this->vertices.at(origem));
            }
            return true;
        } catch(...) {
            std::cout << "ERRO: Vertices inválidos\n";
            return false;
        }
    }

    std::string labelVertice(size_t index) {
        try {
            return this->vertices.at(index).label;
        } catch(...) {
            return "Index Inválido";
        }
    }

    bool existeAresta(size_t origem, size_t destino) {
        try {
            return this->vertices.at(origem).existeAresta(&this->vertices.at(destino));
        } catch(...) {
            std::cout << "Solicitação inválida";
            return false;
        }
    }

    int pesoAresta(size_t origem, size_t destino) {

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

    std::vector<id_t> retornarVizinhos(size_t vertice) {
        std::vector<id_t> vizinhos;
        std::vector<Vertice*> ptrVizinhos = this->vertices.at(vertice).retornarVizinhos();
        for(Vertice* vertice: ptrVizinhos) {
            for(size_t i = 0; i < this->verticesCont; i++) {
                if(&this->vertices.at(i) == vertice) {
                    vizinhos.push_back(i);
                }
            }
        }
        return vizinhos;
    }

};

#endif // GRAPH_HPP_
