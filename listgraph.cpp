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

bool ListGraph::inserirAresta(id_t A, id_t B, weight_t peso = 1) {
    try {

        this->vertices.at(A).inserirAresta(&this->vertices.at(B), peso);
        if(!this->m_dir) {
            this->vertices.at(B).inserirAresta(&this->vertices.at(A), peso);
        }
        return true;
    } catch(...) {
        std::cout << "ERRO: Vertices inválidos\n";
        return false;
    }
}

bool ListGraph::removerAresta(id_t A, id_t B) {
    try {

        this->vertices.at(A).removerAresta(&this->vertices.at(B));
        if(!this->m_dir) {
            this->vertices.at(B).removerAresta(&this->vertices.at(A));
        }
        return true;
    } catch(...) {
        std::cout << "ERRO: Vertices inválidos\n";
        return false;
    }
}

std::optional<std::string> ListGraph::labelVertice(id_t index) {
    try {
        return this->vertices.at(index).label;
    } catch(...) {
        return "Index Inválido";
    }
}

bool ListGraph::existeAresta(id_t A, id_t B) {
    try {
        return this->vertices.at(A).existeAresta(&this->vertices.at(B));
    } catch(...) {
        std::cout << "Solicitação inválida";
        return false;
    }
}

std::optional<weight_t> ListGraph::pesoAresta(id_t A, id_t B) {

    try {
        if(this->existeAresta(A, B)){
            return this->vertices.at(A).pesoAresta(&this->vertices.at(B));
        } else {
            return std::nullopt;
        }
    } catch(...) {
        std::cout << "Solicitação inválida";
        return std::nullopt;
    }
}

std::optional<std::vector<id_t>> ListGraph::retornarVizinhos(id_t idx) {
    std::vector<id_t> vizinhos;
    std::vector<Vertex*> ptrVizinhos = this->vertices.at(idx).retornarVizinhos();
    for(Vertex* vertice: ptrVizinhos) {
        for(id_t i = 0; i < this->verticesCount; i++) {
            if(&this->vertices.at(i) == vertice) {
                vizinhos.push_back(i);
            }
        }
    }
    return vizinhos;
}