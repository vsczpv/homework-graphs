#include "igraph.hpp"

Optimizer::Optimizer(IGraph& graph) noexcept
	: m_graph(std::unique_ptr<IGraph>(graph.duplicate()))
	, m_edges(graph.getArestas())
{}

IGraph* Optimizer::optimize(id_t F, id_t S) noexcept
{

	weight_t baseline = FordFn(*m_graph, F, S).max();
//	std::cout << baseline << std::endl;

	std::vector<weight_t> scores;
//	id_t aaa = 0;
	for (auto& m : m_edges) {
		auto [a, b, w] = m;

		auto clone = m_graph->duplicate();

		auto ia = clone->pesoAresta(b, a);
		auto ia_v = ia ? *ia : 0;

		w += ia_v;

		clone->removerAresta(a, b);
		clone->inserirAresta(b, a, w);

		auto score = FordFn(*clone, F, S).max();

		scores.push_back(score);

//		m_graph->removerAresta(a, b);
//		m_graph->inserirAresta(b, a, w);
//		auto score = FordFn(*m_graph, F, S).max();
//
//		std::cout << "X: " << aaa << " " << score << std::endl;
//		scores.push_back(score);
//		m_graph->removerAresta(b, a);
//		m_graph->inserirAresta(a, b, w);
//		aaa++;
	}

	id_t     h_id = -1;
	weight_t highest = -1.0f;

	for (id_t i = 0; i < scores.size(); i++) {
		if (scores[i] > highest) {
			highest = scores[i];
			h_id = i;
		}
	}

//	std::cout << highest << " " << baseline << " " << h_id << std::endl;

	if (highest <= baseline) {
		return m_graph.release();
	}

	auto [a, b, w] = m_edges[h_id];

//	std::cout << "Y: " << FordFn(*m_graph, F, S).max() << std::endl;

	auto ia = m_graph->pesoAresta(b, a);
	auto ia_v = ia ? *ia : 0;

	w += ia_v;

	m_graph->removerAresta(a, b);
	m_graph->inserirAresta(b, a, w);

//	std::cout << "Z: " << FordFn(*m_graph, F, S).max() << std::endl;

	*this = Optimizer(*m_graph);

	return this->optimize(F, S);
}
