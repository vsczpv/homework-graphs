#include "igraph.hpp"


AGMPrim::AGMPrim(IGraph& graph, bool choosed, id_t choose)
  : m_graph(graph)
  , m_solution            (std::vector<AGMEdge>{}) // Inicia um inicia um conjunto $S$ vazio de arestas
  , m_control             (graph.getVertices())    // Cria conjunto de ($Q$) com todos ss vértices do grafo
  , minimum_spanning_tree (AGMNode(4294967295)) // cria árvore, já com um nó raiz que aponta para todos os de primeira camada
  , m_choosed             (choosed)
  , m_choose              (choose)
{

  std::sort(m_control.begin(), m_control.end(),  [this](id_t a, id_t  b) {
    return this->m_graph.retornarVizinhos(a).value_or(std::vector<id_t>{}).size() < this->m_graph.retornarVizinhos(b).value_or(std::vector<id_t>{}).size();
  });

  /*teste Da Ordenação
  for(id_t vertex : m_control) {
    std::cout << "\nneighbourhood of vector " << vertex << ": " << graph.retornarVizinhos(vertex).value_or(std::vector<id_t>{}).size();
  }*/

  if(m_choosed) { 
    /* se a pessoa escolheu um nó, apaga ele do controle, substitui o nó raiz por ele, e adiciona na árvore e na m_solution */

    for(std::vector<id_t>::iterator vert = m_control.begin(); vert != m_control.end(); vert++) {
      if(*vert == m_choose){
        m_control.erase(vert);
        break;
      }
    }

    minimum_spanning_tree.filhos.clear();
    minimum_spanning_tree.filhos.push_back(AGMNode(m_choose));

    m_solution.push_back(AGMEdge(m_choose, m_choose, 0)); // aresta para o nada, só para registrar que já foi consumido esse vértice


    recursiveGenerator(m_choose, minimum_spanning_tree.filhos.back());

  } else {
    /* Se a pessoa não escolheu, utiliza um nó fictício como raiz, inicia pelo vértice com mais vizinhos e encontra todos os vértices do grafo, não apenas uma sub árvore*/
    while(m_control.begin() != m_control.end()) {

      id_t current_node = m_control.back();
      m_control.pop_back();
      m_solution.push_back(AGMEdge(4294967295, current_node, 0)); // se não for conectado com o restante da aresta, conecta no nó raiz, com um peso 0
      minimum_spanning_tree.filhos.push_back(AGMNode(current_node));
      
      recursiveGenerator(current_node, minimum_spanning_tree.filhos.back());
    }
  }
}


/* Funcionamento:   recursiveGenerator -> chooseEdge -> recursiveGenerator -> chooseEdge . . . */
void AGMPrim::recursiveGenerator(id_t vertex, AGMNode& current_node) {

  std::vector<AGMEdge> edges;
  
  for(id_t edge : m_graph.retornarVizinhos(vertex).value_or(std::vector<id_t> {})) {
    edges.push_back(AGMEdge(vertex, edge, m_graph.pesoAresta(vertex, edge).value_or(id_t(4294967295))));
  }
  
  std::sort(edges.begin(), edges.end(),  [this](AGMEdge a, AGMEdge b) {
    return a.weight < b.weight;
  });


  for(AGMEdge edge : edges){
    chooseEdge(edge, current_node);
  }
}


/* Esta funhção verifica se o destino da aresta já foi pego, se já foi adiciona na solução e apaga do controle*/
void AGMPrim::chooseEdge(AGMEdge edge, AGMNode& current_node) {
  if(!isInSolution(edge)) {

    m_solution.push_back(edge);

    current_node.filhos.push_back(AGMNode(edge.destination));

    for(auto vert = m_control.begin(); vert != m_control.end(); vert++) {
      if(*vert == edge.destination){
        m_control.erase(vert);
        break;
      }
    }
    recursiveGenerator(edge.destination, current_node.filhos.back());
    return;
  } else {
    return;
  }

}

bool AGMPrim::isInSolution(AGMEdge edge) {
  if(this->m_control.size() < this->m_solution.size()) {
    for(id_t vector_edge : m_control) {
      if(vector_edge == edge.destination) return false;
    }
    return true;
  } else {
    for(AGMEdge vector_edge : m_solution) {
      if(vector_edge.destination == edge.destination) return true;
    }
    return false;
  }
}


void recursivelyPrintAgm(AGMNode node, int deeph) {
  if(node.vertice_id == 4294967295) {
    std::cout << "{Raiz: -- }\n";

  } else {

    for(int i = 0; i < deeph; i++) {
      std::cout << "\t";
    }
    std::cout << "{node: " << node.vertice_id << " }\n";
  }

  for(AGMNode i_node : node.filhos) {
    recursivelyPrintAgm(i_node, deeph + 1);
  }
}

void AGMPrim::printAGM() {
  weight_t peso_total = 0;
  for(AGMEdge edge : m_solution) {
    peso_total += edge.weight;
  }
  std::cout << "\nPeso total: " << peso_total;

  std::cout << "\nÀrvore: \n";

  recursivelyPrintAgm(minimum_spanning_tree, 0);

}
/*
bool AGMPrim::sortAGMEdgesIterators(std::vector<AGMEdge>::iterator a, std::vector<AGMEdge>::iterator b) {
  return a->weight < b->weight;
}
bool AGMPrim::sortVertexByNumberOfNeighbours(id_t a, id_t b) {
  return this->m_graph.retornarVizinhos(a).value_or(std::vector<id_t>{}).size() > this->m_graph.retornarVizinhos(a).value_or(std::vector<id_t>{}).size();
}*/