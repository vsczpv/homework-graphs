#include "igraph.hpp"

DijkstraTable::DijkstraTable(IGraph& graph)
	: m_graph(graph)
{
	auto verts = graph.getVertices();

	for (auto v : verts)
		m_dists [v] = std::numeric_limits<weight_t>::max(),
		m_prevs [v] = std::nullopt,
		m_closed[v] = false;

}

void DijkstraTable::mark_distance(id_t a, id_t b)
{
	if (m_prevs[a])
	{
		auto curdis = m_dists[b];
		auto newdis = *m_graph.pesoAresta(a, b) + m_dists[a]; //*m_graph.pesoAresta(*m_prevs[a], a);

		if (newdis < curdis)
			m_dists[b] = newdis,
			m_prevs[b] = a;
	}

	else
	{
		m_dists[b] = *m_graph.pesoAresta(a, b);
		m_prevs[b] = a;
	}
};

bool DijkstraTable::is_closed(id_t v) noexcept {
	return m_closed[v];
}

void DijkstraTable::set_closed(id_t v) noexcept {
	m_closed[v] = true;
}

void DijkstraTable::reset_dist(id_t v) noexcept
{
	m_dists[v] = 0;
}

size_t DijkstraTable::nodecount(void) noexcept
{
	return m_dists.size();
}

std::vector <id_t> DijkstraTable::nodes(void) noexcept
{
	std::vector <id_t> res;

	for (auto [k, _] : m_dists)
		res.push_back(k);

	return res;
}

std::optional<id_t> DijkstraTable::parent(id_t v)
{
	return m_prevs[v];
}

void DijkstraTable::dbgprint() {

	for (auto v : m_graph.getVertices())
		std::cout << v << "\t";

	std::cout << std::endl;

	for (auto [_, d] : m_dists)
		std::cout << d << "\t";

	std::cout << std::endl;

	for (auto [_, p] : m_prevs)
		std::cout << (p ? std::to_string(*p) : "-") << "\t";

	std::cout << std::endl;

	for (auto [_, a] : m_closed)
		std::cout << a << "\t";

	std::cout << std::endl;

}

DijkstraTable IGraph::dijkstra(id_t origin) {

	DijkstraTable table(*this);
	table.reset_dist(origin);

	for (auto sv : this->bfs(origin)) {

		auto visit = this->retornarVizinhos(sv);

		if (visit) for (auto vv : *visit)
		{
			if (table.is_closed(vv)) continue;
			table.mark_distance(sv, vv);
		}

		table.set_closed(sv);;
	}

	return table;
}

std::map<id_t, std::vector<id_t>> IGraph::dijkstra_caminhos(id_t origin)  {

	auto table = this->dijkstra(origin);

	std::map<id_t, std::vector<id_t>> caminhos;

	for (auto k : table.nodes())
	{
		if (k == origin) continue;

		std::optional<id_t> trail = k;
		while ((trail = table.parent(*trail)))
			caminhos[k].push_back(*trail);

		std::reverse(caminhos[k].begin(), caminhos[k].end());
	}

	return caminhos;
};
