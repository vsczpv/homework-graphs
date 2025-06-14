#include "igraph.hpp"

Optimizer::Optimizer(IGraph& graph) noexcept
	: m_graph(std::unique_ptr<IGraph>(graph.duplicate()))
	, m_edges(graph.getArestas())
{}

IGraph* Optimizer::optimize(id_t F, id_t S) noexcept
{

	weight_t baseline = FordFn(*m_graph, F, S).max();

	std::cout << "Tentativa atual FM = " << baseline << std::endl;

	std::vector<weight_t> scores;

	id_t cy = 0;
	for (auto& m : m_edges) {
		std::cout << "Ciclo " << cy++ << std::endl;
		auto [a, b, w] = m;

		auto old_w = w;

		auto ia = m_graph->pesoAresta(b, a);
		auto ia_v = ia ? *ia : 0;

		w += ia_v;

		m_graph->removerAresta(a, b);
		m_graph->inserirAresta(b, a, w);

		auto score = FordFn(*m_graph, F, S).max();

		scores.push_back(score);

		m_graph->removerAresta(b, a);
		m_graph->inserirAresta(a, b, old_w);

		if (ia)
			m_graph->inserirAresta(b, a, *ia);

	}

	id_t     h_id = -1;
	weight_t highest = -1.0f;

	for (id_t i = 0; i < scores.size(); i++) {
		if (scores[i] > highest) {
			highest = scores[i];
			h_id = i;
		}
	}

	if (highest <= baseline) {
		return m_graph.release();
	}

	auto [a, b, w] = m_edges[h_id];

	auto ia = m_graph->pesoAresta(b, a);
	auto ia_v = ia ? *ia : 0;

	w += ia_v;

	m_graph->removerAresta(a, b);
	m_graph->inserirAresta(b, a, w);

	*this = Optimizer(*m_graph);

	return this->optimize(F, S);
}
