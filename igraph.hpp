#ifndef IGRAPH_HPP_
#define IGRAPH_HPP_

#include <string>
#include <vector>
#include <limits>
#include <algorithm>      // para usar std::reverse
#include <unordered_set>
#include <optional>
#include <cstdint>
#include <cstring>
#include <queue>
#include <stack>


#include <iostream>

using weight_t = float;
using id_t     = uint32_t;

class IGraph;

class IGraphBFSIter {
	IGraph&                   m_graph;
	bool                      m_end;
	std::unordered_set<id_t>& m_visits;
	std::queue<id_t>          m_queue = {};
	id_t                      m_current_node;
public:
	IGraphBFSIter(IGraph& parent, bool is_end, std::unordered_set<id_t>& visitstore, id_t root)
		: m_graph       (parent)
		, m_end         (is_end)
		, m_visits      (visitstore)
		, m_current_node(root)
	{}

	IGraphBFSIter(const IGraphBFSIter& rhs)
		: m_graph        (rhs.m_graph)
		, m_end          (rhs.m_end)
		, m_visits       (rhs.m_visits)
		, m_queue        (rhs.m_queue)
		, m_current_node (rhs.m_current_node)
	{}

	~IGraphBFSIter() = default;

	IGraphBFSIter& operator=(const IGraphBFSIter& rhs);

	IGraphBFSIter& operator++(); /* prefix */

	id_t operator*() const;

	friend bool operator==(const IGraphBFSIter& lhs, const IGraphBFSIter& rhs);
	friend bool operator!=(const IGraphBFSIter& lhs, const IGraphBFSIter& rhs);

	friend void swap(IGraphBFSIter& lhs, IGraphBFSIter& rhs);
};

class IGraphDFSIter {
	IGraph&                   m_graph;
	bool                      m_end;
	std::unordered_set<id_t>& m_visits;
	std::stack<id_t>          m_stack;
	id_t                      m_current_node;
public:
	IGraphDFSIter(IGraph& parent, bool is_end, std::unordered_set<id_t>& visitstore, id_t root)
		: m_graph       (parent)
		, m_end         (is_end)
		, m_visits      (visitstore)
		, m_current_node(root)
	{}

	~IGraphDFSIter() = default;

	IGraphDFSIter& operator=(const IGraphDFSIter& rhs);

	IGraphDFSIter& operator++(); /* prefix */

	id_t operator*() const;

	friend bool operator==(const IGraphDFSIter& lhs, const IGraphDFSIter& rhs);
	friend bool operator!=(const IGraphDFSIter& lhs, const IGraphDFSIter& rhs);

	friend void swap(IGraphDFSIter& lhs, IGraphDFSIter& rhs);
};

class IGraphBFSIterGen {
	IGraph&                  m_graph;
	std::unordered_set<id_t> m_visits = {};
	id_t                     m_root;
public:
	IGraphBFSIterGen(IGraph& parent, id_t root)
		: m_graph(parent)
		, m_root (root)
	{}

	IGraphBFSIter begin();
	IGraphBFSIter end();
};

class IGraphDFSIterGen {
	IGraph&                  m_graph;
	std::unordered_set<id_t> m_visits = {};
	id_t                     m_root;
public:
	IGraphDFSIterGen(IGraph& parent, id_t root)
		: m_graph(parent)
		, m_root (root)
	{}

	IGraphDFSIter begin();
	IGraphDFSIter end();
};





















class DijkstraTableElement {
	private:
		id_t              vertexId;
		std::optional<std::vector<id_t>> neighborhood;
		weight_t          originDistance;
		std::optional<id_t>              previousVertex;
		bool              found;

	public:

	id_t getVertexId() {
		return vertexId;
	}

	weight_t getOriginDistance() {
		return originDistance;
	}

	std::optional<id_t> getPreviousVertex() {
		return previousVertex;
	}

	DijkstraTableElement(id_t vertexId, std::optional<std::vector<id_t>> neigborhood, weight_t originDistance, std::optional<id_t> previousVertex, bool found)
	: vertexId(vertexId), neighborhood(neighborhood), originDistance(originDistance), previousVertex(previousVertex), found(found)
	{};

	void update(weight_t originDistance, id_t previousVertex) {
		originDistance = originDistance;
		previousVertex = previousVertex;
		found = true;
	}

	bool isfound() {
		return found;
	}

	void find() {
		found = true;
	}

};


class DijkstraTable {
private:

	std::vector<DijkstraTableElement> table;
	std::vector<id_t> convertionTable;

public:

	DijkstraTable()
	: table(), convertionTable()
	{};

	bool createElement(id_t vertexId, std::optional<std::vector<id_t>> neigborhood, weight_t originDistance, std::optional<id_t> previousVertex, bool found) {
		try {
			table.push_back(DijkstraTableElement(vertexId, neigborhood, originDistance, previousVertex, found));

			return false;
		} catch(...) {
			return true;
		}
	}

	void createTabelaConversaoElement(id_t naDijkstraTable, id_t noGrafo) {
		if(noGrafo > 10000) { // Para não consumir memória de mais, vai só até o vértice 10000
			return;
		}

		// O vetor será um espelho do grafo, o id x do grafo ficará no id x daqui
		if(noGrafo > convertionTable.size()){
			for(id_t i = 0; i < noGrafo; i++) {
				convertionTable.push_back(-1);
			}
		}
		convertionTable.at(noGrafo) = naDijkstraTable;
		return;
	}
	
	DijkstraTableElement at(id_t id) {
		return table.at(id);
	}

	std::optional<id_t> getPreviousVertex(id_t id) {
		return table.at(id).getPreviousVertex();
	}

	size_t size() {
		return table.size();
	}

	void updateElement(id_t id, weight_t originDistance, id_t previousVertex) {
		table.at(id).update(originDistance, previousVertex);
	}
 
	id_t findByVertexId(id_t id) {
		// Com o id do vértice, retorna aonde ele está
		for(size_t i = 0; i < table.size(); i++) {
			if(table.at(i).getVertexId() == id) return i;
		}
	}

};


class DijkstraIGraphBFSIter {
	IGraph&                   m_graph;
	bool                      m_end;
	std::unordered_set<id_t>& m_visits;
	std::queue<id_t>          m_queue = {};
	id_t                      m_current_node;
	id_t                      m_previous_node;
public:
	DijkstraIGraphBFSIter(IGraph& parent, bool is_end, std::unordered_set<id_t>& visitstore, id_t root)
		: m_graph       (parent)
		, m_end         (is_end)
		, m_visits      (visitstore)
		, m_current_node(root)
	{}

	DijkstraIGraphBFSIter(const DijkstraIGraphBFSIter& rhs)
		: m_graph        (rhs.m_graph)
		, m_end          (rhs.m_end)
		, m_visits       (rhs.m_visits)
		, m_queue        (rhs.m_queue)
		, m_current_node (rhs.m_current_node)
	{}

	id_t getCurrentNode() {
		return m_current_node;
	}
	id_t getPreviousNode() {
		return m_previous_node;
	}

	~DijkstraIGraphBFSIter() = default;

	DijkstraIGraphBFSIter& operator=(const DijkstraIGraphBFSIter& rhs);

	DijkstraIGraphBFSIter& operator++(); /* prefix */

	id_t operator*() const;

	friend bool operator==(const DijkstraIGraphBFSIter& lhs, const DijkstraIGraphBFSIter& rhs);
	friend bool operator!=(const DijkstraIGraphBFSIter& lhs, const DijkstraIGraphBFSIter& rhs);

	friend void swap(DijkstraIGraphBFSIter& lhs, DijkstraIGraphBFSIter& rhs);
};

class DijkstraIGraphBFSIterGen {
	IGraph&                  m_graph;
	std::unordered_set<id_t> m_visits = {};
	id_t                     m_root;
public:
	DijkstraIGraphBFSIterGen(IGraph& parent, id_t root)
		: m_graph(parent)
		, m_root (root)
	{}

	DijkstraIGraphBFSIter begin();
	DijkstraIGraphBFSIter end();
};


class IGraph {

public:

	virtual bool               inserirVertice(std::string label)                 noexcept = 0;
	virtual bool               removerVertice(id_t idx)                          noexcept = 0;
	virtual bool               inserirAresta (id_t A, id_t B, weight_t peso = 1) noexcept = 0;
	virtual bool               removerAresta (id_t A, id_t B)                    noexcept = 0;
	virtual std::vector<id_t>  getVertices   ()                                  noexcept = 0;

	virtual std::optional<std::string>
	                    labelVertice  (id_t idx)                    noexcept = 0;

	virtual bool        existeAresta  (id_t A, id_t B)              noexcept = 0;

	virtual std::optional<weight_t>
						pesoAresta    (id_t A, id_t B)              noexcept = 0;

	virtual std::optional<std::vector<id_t>>
	                   retornarVizinhos(id_t idx)                   noexcept = 0;

	virtual void imprimeGrafo(void) noexcept = 0;

	virtual bool pond(void) noexcept = 0;
	virtual bool dir (void) noexcept = 0;

	virtual ~IGraph() = default;

	IGraphBFSIterGen bfs(id_t root) noexcept {
		return IGraphBFSIterGen(*this, root);
	}

	IGraphDFSIterGen dfs(id_t root) noexcept {
		return IGraphDFSIterGen(*this, root);
	}

	DijkstraIGraphBFSIterGen djkbfs(id_t root) noexcept {
		return DijkstraIGraphBFSIterGen(*this, root);
	} // Iterador usado pelo Dijkstra


	std::vector<std::vector<id_t>> dijkstra(id_t origin) {
		//          Definir um vértice de Origem ^
		
		//Criar tabela com todos os vértices e Campos: Distância do Vértice Origem; Vértice Pai; Caminho Encontrado (Y, N)
		DijkstraTable dijkstraTable;
		std::vector<id_t> allVertices = getVertices();
		
		//Cria um id que representa o vazio
		id_t fim = static_cast<id_t>(allVertices.size());

		for(size_t i = 0; i < allVertices.size(); i++) {
			dijkstraTable.createElement(allVertices.at(i), retornarVizinhos(i), std::numeric_limits<weight_t>::max(), std::nullopt, 0);
			std::cout << std::numeric_limits<weight_t>::max();
			if(dijkstraTable.at(i).getVertexId() == origin) dijkstraTable.at(i).update(0, fim);
			//dijkstraTable.createTabelaConversaoElement(i, allVertices.at(i));
		}


		dijkstraTable.createElement(fim, std::nullopt, 0, std::nullopt, 1);
		
	/*	Para cada vizinho ($v$) do vértice atual ($u$):
	1. Calcular a distância ($d$) dele ao início considerando o caminho atual $d(v) = d(u) + w(u, v)$
	2. Se $d(u)$ atual for menor do que a $d(u)$ já salva, substituir todos os atributos pelos novos.
	3. Após visitar um nó, marca-lo como visitado.
	4. LOOP, repetir até que todos os nós tenham sido visitados ou o vértice destino tenha sido encontrado
	*/
	auto bfs = this->djkbfs(origin);

	for (auto it = ++bfs.begin(); it != bfs.end(); ++it) // Viaja pela BFS
	{

		weight_t distanciaAnterior = dijkstraTable.at(it.getCurrentNode()).getOriginDistance();
		weight_t distanciaCaminhoAtual = dijkstraTable.at(it.getPreviousNode()).getOriginDistance();
		weight_t pesoDaAresta = pesoAresta(it.getPreviousNode(), it.getCurrentNode()).value_or(99999.0);

		
		if(it.getCurrentNode() == origin){
			distanciaAnterior = 0;
			distanciaCaminhoAtual = 0;
			pesoDaAresta = 0;
		}

		
		std::cout << "\n\nvértice anterior: " << it.getPreviousNode();
		std::cout << "\ndistanciaAnterior: " << distanciaAnterior << "\ndistanciaCaminhoAtual: " << distanciaCaminhoAtual << "\npesoDaAresta: " << pesoDaAresta;

		if(distanciaAnterior > distanciaCaminhoAtual + pesoDaAresta) { // Se o novo caminho for melhor
			dijkstraTable.at(it.getCurrentNode()).update(distanciaCaminhoAtual + pesoDaAresta, it.getPreviousNode()); // d(v) = w(u, v)
			std::cout << "\nRealizando Update";
		}
		
	}
		// 5. Ao Encontrar o vértice destino, reconstruir ele utilizando os vértices pais e retornar o caminho
		std::vector<std::vector<id_t>> caminhos;
		caminhos.resize(dijkstraTable.size());
		
		for(size_t i = 0; i < dijkstraTable.size(); i++) {
			
			for(id_t j = dijkstraTable.at(i).getVertexId(); j != fim; j = dijkstraTable.getPreviousVertex(i).value_or(fim)) {
				
				caminhos[i].push_back(allVertices.at(j));
			}
			
			std::reverse(caminhos[i].begin(), caminhos[i].end()); // inverte o caminho, já que foi reconstruído de trás para frente
		}
		
	return caminhos;

	}

	/*bool imprimeDijkstra(id_t origin) {
		try {
			std::vector<std::vector<id_t>> caminhos = dijkstra(origin);
			for(int i = 0; i < caminhos.size(); i++){
				std::cout << "\n" << i << "\t" << " -> |";
				for(id_t j = 0; j < caminhos[i].size(); j++) {
					std::cout << caminhos[i].at(j) << " | ";
					
				}
			}
			return false;

		} catch (...) {
			return true;
		}
	}*/



//	IGraphBFSIter(this, false, , origem);
};

#endif // IGRAPH_HPP_
