#include "listgraph.hpp"



        

bool ListGraph::pond(void) noexcept {
    return m_pond;
}

bool ListGraph::dir(void) noexcept {
    return m_dir;
}


bool ListGraph::inserirVertice(std::string label) noexcept {
    try{
        Vertex v;
        v.label = label;
        vertices.push_back(v);
        verticesCount++;
        return true;
    } catch (...) {
        return false;
    }
}


bool ListGraph::removerVertice(id_t indice) noexcept {
    try {
        if (indice >= verticesCount)
            return false;

        for(id_t j = 0; j < verticesCount; j++) { // remove arestas que apontam para ele
            removerAresta(j, indice);
        }
        vertices.erase(vertices.cbegin() + indice); // remove a vertice
        verticesCount--;
        return true;

    } catch(...) {
        return false;
    }
}

void ListGraph::imprimeGrafo() noexcept {

    try{ // Lista
        std::cout << "\nLista\n";
        for(id_t  i = 0; i < verticesCount; i++) {
            std::cout << i << "\t" << vertices.at(i).label << " ->  |";
            for(id_t  j = 0; j < vertices.at(i).edgesCount; j++) {
                std::cout << " " << vertices.at(i).edges.at(j).destination->label;
                if(m_pond) { std::cout << ": " << vertices.at(i).edges.at(j).weight << " |"; } else { std::cout << " |"; }
            }
            std::cout << "\n";
        }
    } catch (...) {
        std::cout << "ERROR: erro na geração da matriz da lista\n";
    }
}

bool ListGraph::inserirAresta(id_t A, id_t B, weight_t peso = 1) noexcept {
    try {

        vertices.at(A).inserirAresta(&vertices.at(B), peso);
        if(!m_dir) {
            vertices.at(B).inserirAresta(&vertices.at(A), peso);
        }
        return true;
    } catch(...) {
        std::cout << "ERRO: Vertices inválidos\n";
        return false;
    }
}

bool ListGraph::removerAresta(id_t A, id_t B) noexcept {
    try {

        vertices.at(A).removerAresta(&vertices.at(B));
        if(!m_dir) {
            vertices.at(B).removerAresta(&vertices.at(A));
        }
        return true;
    } catch(...) {
        std::cout << "ERRO: Vertices inválidos\n";
        return false;
    }
}

std::optional<std::string> ListGraph::labelVertice(id_t index) noexcept {
    try {
        return vertices.at(index).label;
    } catch(...) {
        return std::nullopt;
    }
}

bool ListGraph::existeAresta(id_t A, id_t B) noexcept {
    try {
        return vertices.at(A).existeAresta(&vertices.at(B));
    } catch(...) {
        std::cout << "Solicitação inválida";
        return false;
    }
}

std::optional<weight_t> ListGraph::pesoAresta(id_t A, id_t B) noexcept {

    try {
        if(existeAresta(A, B)){
            return vertices.at(A).pesoAresta(&vertices.at(B));
        } else {
            return std::nullopt;
        }
    } catch(...) {
        std::cout << "Solicitação inválida";
        return std::nullopt;
    }
}

std::optional<std::vector<id_t>> ListGraph::retornarVizinhos(id_t idx) noexcept {
    if(idx >= verticesCount) return std::nullopt;
    std::vector<id_t> vizinhos;
    std::vector<Vertex*> ptrVizinhos = vertices.at(idx).retornarVizinhos();
    for(Vertex* vertice: ptrVizinhos) {
        for(id_t i = 0; i < verticesCount; i++) {
            if(&vertices.at(i) == vertice) {
                vizinhos.push_back(i);
            }
        }
    }
    return vizinhos;
}