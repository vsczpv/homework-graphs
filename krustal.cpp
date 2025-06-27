#include "igraph.hpp"

Krustal::Krustal(IGraph& parent) noexcept
		: m_graph(parent)
{

	auto as = m_graph.getArestas();
	m_Q = std::deque<std::tuple<id_t,id_t,weight_t>>(as.begin(), as.end());

	for (auto v : m_graph.getVertices()) {
		std::unordered_set<id_t> s;
		s.insert(v);
		m_F.push_back(s);
	}

	std::sort(m_Q.begin(), m_Q.end(), [](auto& a, auto& b) -> bool {
		return std::get<2>(a) < std::get<2>(b);
	});

}

void Krustal::print(void) noexcept {
	std::cout << "S: ";
	for (auto s : m_S) {
		auto [a, b, w] = s;
		printf("<%u %u> ", a, b);
	}
	std::cout << "\nQ: ";
	for (auto q : m_Q) {
		auto [a, b, w] = q;
		printf("<%u %u> ", a, b);
	}
	std::cout << "\nF: ";
	for (auto f : m_F) {
		printf("{ ");
		for (auto v : f) {
			printf("%u ", v);
		}
		printf("}");
	}
	std::cout << std::endl;
}

void Krustal::solve(void) noexcept {

	weight_t peso_total = 0;
	int a = 0;
	while (m_Q.size() != 0 && m_F.size() != 1) {
		printf("=== %i\n", a++);
		Krustal::print();
		auto q = m_Q.front();
		m_Q.pop_front();

		ssize_t aux_a = -1;
		ssize_t aux_b = -1;

		for (auto i = 0ul; i < m_F.size(); i++) {
			auto& t = m_F[i];

			auto [a, b, w] = q;

			auto afound = t.contains(a);
			auto bfound = t.contains(b);

			if (afound && bfound) {
				break;
			}

			if (afound) aux_a = i;
			if (bfound) aux_b = i;
		}

		if (aux_b == -1 && aux_a == -1) {
			continue;
		}

		assert(aux_b != -1 || aux_a != -1);

		m_F[aux_a].insert(m_F[aux_b].begin(), m_F[aux_b].end());
		m_F.erase(m_F.begin() + aux_b);

		peso_total += std::get<2>(q);

		m_S.push_back(q);
	}

	printf("=== %i\n", a++);
	Krustal::print();
	std::cout << "Peso total: " << peso_total << std::endl;
}
