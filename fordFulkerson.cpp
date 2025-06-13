
#include "igraph.hpp"
#include "igraphloader.hpp"
#include "matrixgraph.hpp"
#include "listgraph.hpp"
#include <memory>

#include <unistd.h>

FordFulkerson::FordFulkerson(IGraph& graph, id_t source, id_t sink)
        : m_graph(graph)
        , m_residual(*std::make_unique<ListGraph>(true, true)) // Inicializa o grafo residual
        , m_source(source)
        , m_sink(sink)
    {}



std::optional<FordFulkerson> FordFulkerson::find_max_flow() noexcept {
    
    /* verificar se há caminho entre origem e sorvedor */
    if(m_source == m_sink || !m_graph.retornarVizinhos(m_source).has_value() || !m_graph.retornarVizinhos(m_sink).has_value()) return std::nullopt;

    generate_residual_graph();

    while(exists_augmenting_path()) {
        float minimum_flow = get_minimum_flow();
        auto iter = m_residual.dfs(m_source);

        for (auto v : iter) {
            // se tiver capacidade positiva, diminui do fluxo e adiciona ao fluxo inverso
            if(v.m_current_node == m_sink) {
                break;
            } else {
                if(m_residual.pesoAresta())
                increment_capacity(minimum_flow);
            }
        }
        Solution += minimum_flow; // Adiciona o fluxo mínimo ao fluxo máximo
    }
}

int FordFulkerson::get_minimum_flow() noexcept {
    //encontra a aresta de menor capacidade no grafo residual
    float min_flow = INT_MAX;
    for (const auto& node : m_residual.getVertices()) {
        for (const auto& aresta : m_residual.retornarVizinhos(node).value_or(std::vector<id_t>())) {
            float peso = m_residual.pesoAresta(node, aresta).value_or(INT_MAX);
            if (peso < min_flow) {
                min_flow = peso;
            }
        }
    }
}


void FordFulkerson::generate_residual_graph() noexcept {/* CRIE RESIDUAL GRAPH COM UM BFS, PARA QUE ELE INICIE EM DIREÇÃO SOURCE -> SINK (CRÍTICO)*/
    std::vector<id_t> vertices = m_graph.getVertices();
    for(auto vertex_id : vertices) {
        m_residual.inserirVertice(std::to_string(vertex_id)); // insere o vértice no grafo residual
        std::optional<std::vector<id_t>> arestas = m_graph.retornarVizinhos(vertex_id);
        for(auto aresta : *arestas) {
            std::optional<weight_t> peso = m_graph.pesoAresta(vertex_id, aresta);

            if (peso) {
                if(!m_residual.existeVertice(aresta)) { // se o destino existe, é porque as arestas dele também existem. 
                    m_residual.inserirAresta(vertex_id, aresta, 0); // Insere como aresta inversa
                } else m_residual.inserirAresta(vertex_id, aresta, *peso);
            }
        }
    }
}

std::optional<std::vector<id_t>> FordFulkerson::get_augment_path() noexcept {
    // Encontra caminho entre origem e sorvedor
    auto caminhos = m_graph.dijkstra_caminhos(m_source);
    auto augment_path = caminhos.find(m_sink);
    if(augment_path == caminhos.end()) {
        return std::nullopt; // Não é possível encontrar fluxo se origem e destino não se encontram
    }
    return augment_path->second; // Retorna o caminho encontrado
}

bool FordFulkerson::exists_augmenting_path() noexcept {
    // Verifica se existe caminho entre origem e sorvedor
    auto iter = m_residual.dfs(m_source);
    for (auto v : iter) {
        if (v == m_sink) {
            return true;
        }
    }
    return false;
}

void FordFulkerson::diminish_capacity(int flow) noexcept {

    if(m_residual.existeVertice(m_source) && m_residual.existeAresta(m_source, m_sink)) {
        weight_t peso_final = m_residual.pesoAresta(m_source, m_sink).value_or(0) - flow;
        if (peso_final < 0) peso_final = 0; // Evita capacidade negativa
        // remove aresta e insere outra no lugar, pois não temos função de mudar peso
        m_residual.removerAresta(m_source, m_sink);
        m_residual.inserirAresta(m_source, m_sink, peso_final);
    } else return;
}


void FordFulkerson::increment_capacity(int flow) noexcept {
    
    if(m_residual.existeVertice(m_source) && m_residual.existeAresta(m_source, m_sink)) {
        weight_t peso_final = m_residual.pesoAresta(m_source, m_sink).value_or(0) + flow;
        // remove aresta e insere outra no lugar, pois não temos função de mudar peso
        m_residual.removerAresta(m_source, m_sink);
        m_residual.inserirAresta(m_source, m_sink, peso_final);
    } else return;

}