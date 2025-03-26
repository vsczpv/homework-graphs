#include "listgraph.hpp"



        

bool ListGraph::pond(void) {
    return m_pond;
}

bool ListGraph::dir(void) {
    return m_dir;
}


bool ListGraph::inserirVertice(std::string label) {
    try{
        Vertex v;
        v.label = label;
        this->vertices.push_back(v);
        verticesCount++;
        return true;
    } catch (...) {
        return false;
    }
}


bool ListGraph::removerVertice(id_t indice) {
    try {
        if (indice >= this->verticesCount)
            return false;

        for(id_t j = 0; j < this->verticesCount; j++) { // remove arestas que apontam para ele
            this->removerAresta(j, indice);
        }
        this->vertices.erase(this->vertices.cbegin() + indice); // remove a vertice
        this->verticesCount--;
        return true;

    } catch(...) {
        return false;
    }
}

void ListGraph::imprimeGrafo() {

    try{ // Lista
        std::cout << "\nLista\n";
        for(id_t  i = 0; i < this->verticesCount; i++) {
            std::cout << i << "\t" << this->vertices.at(i).label << " ->  |";
            for(id_t  j = 0; j < this->vertices.at(i).arestasCount; j++) {
                std::cout << " " << this->vertices.at(i).arestas.at(j).destino->label;
                if(this->m_pond) { std::cout << ": " << this->vertices.at(i).arestas.at(j).weight << " |"; } else { std::cout << " |"; }
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
        if(!this->m_dir) {
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
        if(!this->m_dir) {
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
    std::vector<Vertex*> ptrVizinhos = this->vertices.at(vertice).retornarVizinhos();
    for(Vertex* vertice: ptrVizinhos) {
        for(id_t i = 0; i < this->verticesCount; i++) {
            if(&this->vertices.at(i) == vertice) {
                vizinhos.push_back(i);
            }
        }
    }
    return vizinhos;
}