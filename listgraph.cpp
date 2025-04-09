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
        if(freeSlots.empty()) { // Se não tem espaços vazios no grafo
            v.position = verticesCount;
            vertices.push_back(v);
            verticesCount++;
        }
        else {
            vertices.at(freeSlots.back()) = v;  // Se tem espaços vazios no grafo
            v.position = freeSlots.back();
            freeSlots.pop_back(); // Remove o espaço preenchido da lista de espaços vazios
        }
        return false;
    } catch (...) {
        return true;
    }
}

bool ListGraph::removerVertice(id_t indice) noexcept {
    try {
        if (indice >= verticesCount) return false;
        if (!vertices.at(indice).exists) return false;

        // remove arestas que apontam para ele
        std::vector<id_t> vizinhos = vertices.at(indice).retornarVizinhosToIt();
        for(id_t j = 0; j < vizinhos.size(); j++) {
            vertices.at(vizinhos.at(j)).removerAresta(&vertices.at(indice));
        }

        // remove registros de arestas para as quais ele aponta
        vizinhos = vertices.at(indice).retornarVizinhos();
        for(id_t j = 0; j < vizinhos.size(); j++) {
            vertices.at(vizinhos.at(j)).removerArestaToIt(&vertices.at(indice));
        }


        
        // remove o vertice
        vertices.at(indice).exists = 0; // Seta ele como vazio ao invés de excluir
        freeSlots.push_back(int(indice));

        while(!vertices.back().exists){ // Exclui os vazios no final do vetor

            for(int i = freeSlots.size()-1; i >= 0; i--) {
                if(freeSlots.at(i) == vertices.back().position) {
                    freeSlots.erase(freeSlots.cbegin() + i);
                }
            }

            vertices.pop_back();
            verticesCount--;

        }

        return false;

    } catch(...) {
        std::cout << "\nERRO\nERRO\nERRO\nERRO";
        return true;
    }
}

void ListGraph::imprimeGrafo() noexcept {

    try{ // Lista
        std::cout << "\nLista\n";
        for(id_t  i = 0; i < verticesCount; i++) {
            if(vertices.at(i).exists){

                std::cout << i << "\t" << vertices.at(i).label << " ->  |"; // Vértice

                for(id_t  j = 0; j < vertices.at(i).edgesFromItCount; j++) {
                    std::cout << " " << vertices.at(vertices.at(i).edgesFromIt.at(j).vertex).label; // Aresta
                    if(m_pond) { std::cout << ": " << vertices.at(i).edgesFromIt.at(j).weight << " |"; } else { std::cout << " |"; } // Peso
                }
                std::cout << "\n";

            }


        }
    } catch (...) {
        std::cout << "ERROR: erro na geração da matriz da lista\n";
    }
}

bool ListGraph::inserirAresta(id_t A, id_t B, weight_t peso = 1) noexcept {
    try {
        
        if (!vertices.at(A).exists) return false;
        if (!vertices.at(B).exists) return false;

        vertices.at(A).inserirAresta(&vertices.at(B), peso);
        if(!m_dir) {
            vertices.at(B).inserirAresta(&vertices.at(A), peso);// Para grafos não direcionados, adiciona a aresta no destino
        } else {
            vertices.at(B).inserirArestaToIt(&vertices.at(A), peso);// Adiciona registro de aresta no vertice destino
        }
        return false;
    } catch(...) {
        return true;
    }
}

bool ListGraph::removerAresta(id_t A, id_t B) noexcept {
    try {

        if (!vertices.at(A).exists) return false;
        if (!vertices.at(B).exists) return false;


        vertices.at(A).removerAresta(&vertices.at(B));
        if(!m_dir) {
            vertices.at(B).removerAresta(&vertices.at(A));
        } else {
            vertices.at(B).removerArestaToIt(&vertices.at(A));
        }
        return false;
    } catch(...) {
        return true;
    }
}

std::optional<std::string> ListGraph::labelVertice(id_t index) noexcept {
    try {

        if(!vertices.at(index).exists) return std::nullopt;

        return vertices.at(index).label;
    } catch(...) {
        return std::nullopt;
    }
}

bool ListGraph::existeAresta(id_t A, id_t B) noexcept {
    try {

        if (!vertices.at(A).exists) return false;
        if (!vertices.at(B).exists) return false;

        return vertices.at(A).existeAresta(&vertices.at(B));
    } catch(...) {
        return true;
    }
}

std::optional<weight_t> ListGraph::pesoAresta(id_t A, id_t B) noexcept {

    try {

        if (!vertices.at(A).exists) return false;
        if (!vertices.at(B).exists) return false;

        if(existeAresta(A, B)){
            return vertices.at(A).pesoAresta(&vertices.at(B));
        } else {
            return std::nullopt;
        }
    } catch(...) {
        return std::nullopt;
    }
}

std::optional<std::vector<id_t>> ListGraph::retornarVizinhos(id_t idx) noexcept {
    

    if (!vertices.at(idx).exists) return std::nullopt;
    
    if(idx >= verticesCount) return std::nullopt;

    return vertices.at(idx).retornarVizinhos();
}
