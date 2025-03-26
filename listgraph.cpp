#include "listgraph.hpp"



        

bool ListGraph::pond(void) {
    return this->pond;
}

bool ListGraph::dir(void) {
    return this->dir;
}


bool ListGraph::inserirVertice(std::string label) {
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

bool ListGraph::removerVertice(std::string label) {
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

bool ListGraph::removerVertice(id_t indice) {
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

void ListGraph::imprimeGrafo() {

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

bool ListGraph::inserirAresta(id_t origem, id_t destino, int weight = 1) {
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

bool ListGraph::removerAresta(id_t origem, id_t destino) {
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

std::string ListGraph::labelVertice(id_t index) {
    try {
        return this->vertices.at(index).label;
    } catch(...) {
        return "Index Inválido";
    }
}

bool ListGraph::existeAresta(id_t origem, id_t destino) {
    try {
        return this->vertices.at(origem).existeAresta(&this->vertices.at(destino));
    } catch(...) {
        std::cout << "Solicitação inválida";
        return false;
    }
}

int ListGraph::pesoAresta(id_t origem, id_t destino) {

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

std::vector<id_t> ListGraph::retornarVizinhos(id_t vertice) {
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