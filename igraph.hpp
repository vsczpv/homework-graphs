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

};

#endif // IGRAPH_HPP_
