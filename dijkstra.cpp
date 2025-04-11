#include <ranges>

#include "igraph.hpp"

MyDijkstraTable::MyDijkstraTable(IGraph& graph)
	: m_graph(graph)
{
	auto verts = graph.getVertices();

	for (auto v : verts)
		m_dists [v] = std::numeric_limits<weight_t>::max(),
		m_prevs [v] = std::nullopt,
		m_closed[v] = false;

}

void MyDijkstraTable::mark_distance(id_t a, id_t b)
{
	if (m_prevs[a])
	{
		auto curdis = m_dists[b];
		auto newdis = *m_graph.pesoAresta(a, b) + *m_graph.pesoAresta(*m_prevs[a], a);

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

bool MyDijkstraTable::is_closed(id_t v) noexcept {
	return m_closed[v];
}

void MyDijkstraTable::set_closed(id_t v) noexcept {
	m_closed[v] = true;
}

void MyDijkstraTable::reset_dist(id_t v) noexcept
{
	m_dists[v] = 0;
}


void MyDijkstraTable::dbgprint() {

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

MyDijkstraTable IGraph::my_dijktra(int origin) {

	MyDijkstraTable table(*this);
	table.reset_dist(origin);

	for (auto sv : this->bfs(origin)) {

		auto visit = this->retornarVizinhos(sv);

		if (visit)
		{
			auto open = *visit | std::views::filter([&table](id_t v){ return !table.is_closed(v); });
			for (auto vv : open)
				table.mark_distance(sv, vv);
		}

		table.set_closed(sv);;
	}

	return table;
}
