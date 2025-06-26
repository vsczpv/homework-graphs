
#include "igraph.hpp"

FordFn::FordFn(IGraph& original, id_t F, id_t S) noexcept
	: m_residual(std::unique_ptr<IGraph>(original.duplicate()))
	, m_F(F)
	, m_S(S)
{}

std::optional<weight_t> FordFn::process_path(id_t curr) noexcept
{

	m_visited.insert(curr);

	auto nei = m_residual->retornarVizinhos(curr);

	if (!nei || nei->size() == 0)
		return std::nullopt;

	for (auto n : *nei) {
		std::optional<weight_t> res = std::nullopt;
		if (m_visited.find(n) == m_visited.end()) {
			if (n == m_S)
			{
				m_visited.insert(n);
				res = *m_residual->pesoAresta(curr, n);
			}
			else
			{
				auto deep = FordFn::process_path(n);

				if (!deep)
					continue;

				auto wei  = m_residual->pesoAresta(curr, n);
				auto smal = *deep < *wei ? *deep : *wei;

				res = smal;
			}
		}


		// BUG: A subtração é feita antes de se saber qual é o menor valor
		if (res)
		{
			auto cp = *m_residual->pesoAresta(curr, n);
			cp -= *res;

			m_residual->removerAresta(curr, n);
			m_residual->removerAresta(n, curr);

			m_residual->inserirAresta(curr, n, cp);
			m_residual->inserirAresta(n, curr, *res);


			return res;
		}
	}

	return std::nullopt;

}


bool FordFn::step(void) noexcept
{

	std::deque <id_t> path = { m_F };

	view_path(*m_residual, m_F, path);

	int pause;
	std::cin >> pause;

	m_visited.clear();

	auto pmax = FordFn::process_path(m_F);

	if (!pmax)
		return true;

	m_curr_max += *pmax;

	return false;
}

weight_t FordFn::max(void) noexcept {

	while (FordFn::step() == false) {}

	return m_curr_max;
}
