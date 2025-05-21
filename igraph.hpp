#ifndef IGRAPH_HPP_
#define IGRAPH_HPP_

#include <string>
#include <cassert>
#include <vector>
#include <limits>
#include <algorithm>      // para usar std::reverse
#include <unordered_set>
#include <optional>
#include <cstdint>
#include <cstring>
#include <queue>
#include <stack>
#include <map>

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

/* Dijkstra */

class DijkstraTable {
private:
	std::map<id_t, weight_t>            m_dists  = {};
	std::map<id_t, std::optional<id_t>> m_prevs  = {};
	std::map<id_t, bool>                m_closed = {};
	IGraph& m_graph;
public:
	DijkstraTable(IGraph& graph);

	void mark_distance(id_t a, id_t b);

	bool is_closed(id_t v) noexcept;
	void set_closed(id_t v) noexcept;
	void reset_dist(id_t v) noexcept;

	size_t nodecount(void) noexcept;
	std::vector <id_t> nodes(void) noexcept;
	std::optional<id_t> parent(id_t v);

	std::optional<id_t> lowest_open(void) noexcept;

	bool all_closed(void) noexcept;

	void dbgprint(void);
};


/* Coloring */

struct Color {
	int r;
	int g;
	int b;

	Color(int r, int g, int b)
		: r(r)
		, g(g)
		, b(b)
	{}
	Color()
		: r(0)
		, g(0)
		, b(0)
	{}

	int get_color() {
		return (r << 16) | (g << 8) | b;
	}
};


/* Bruit Force */


class BrutForce {
private:
	IGraph& m_graph;
	double  m_burst_time = 0; 
	unsigned int	    m_colors_number  = 1;
	
	std::vector<std::pair<id_t, Color>> m_colors      = {}; // vector<id_cor, cor>
	std::vector<std::pair<id_t, id_t>>  m_output_list = {}; // vector<id_vertice, id_cor>

public:

	BrutForce(IGraph& p_graph);

	virtual BrutForce&                             color_graph()       noexcept; // Colorir o Grafo
	virtual BrutForce&                             next_possibility()  noexcept; // Muda para a próxima possibilidade de cores
	virtual bool                                    is_valid()          noexcept; // Verifica se tem vizinhos de mesma cor
	virtual void                                    create_colors()     noexcept; // Preenche a tabela de cores
	virtual void		                            print_output_list() noexcept; // Imprime a lista de saída

	virtual double                                  get_burst_time()    noexcept; // Retorna o tempo de execução
	virtual int		                                get_colors_number() noexcept; // Retorna o número de cores
	virtual std::vector<std::pair<id_t, Color>>     get_colors()        noexcept; // Retorna a tabela de cores
	virtual std::vector<std::pair<id_t, id_t>>      get_output_list()   noexcept; // Retorna a lista de saída

};

/* IGraph */

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

	virtual std::optional<id_t> labelidx(std::string label) noexcept = 0;

	virtual ~IGraph() = default;

	IGraphBFSIterGen bfs(id_t root) noexcept {
		return IGraphBFSIterGen(*this, root);
	}

	IGraphDFSIterGen dfs(id_t root) noexcept {
		return IGraphDFSIterGen(*this, root);
	}

	DijkstraTable dijkstra(id_t origin);
	std::map<id_t, std::vector<id_t>> dijkstra_caminhos(id_t origin);

	BrutForce bruit_force() noexcept {
		return BrutForce(*this);
	}

	WelshPowell welsh_powell() noexcept {
		return WelshPowell(*this);
	}

};

#endif // IGRAPH_HPP_
