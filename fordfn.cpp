
#include "igraph.hpp"

FordFn::FordFn(IGraph& original, id_t F, id_t S) noexcept
	: m_residual(std::unique_ptr<IGraph>(original.duplicate()))
	, m_F(F)
	, m_S(S)
{}

std::optional<weight_t> FordFn::process_path(id_t curr, weight_t cw) noexcept
{

	m_visited.insert(curr);

	auto nei = m_residual->retornarVizinhos(curr);

	if (!nei || nei->size() == 0)
	{
		return std::nullopt;
	}

	for (auto n : *nei) {
		std::optional<weight_t> res = std::nullopt;
		if (m_visited.contains(n) == false) {
			if (n == m_S)
			{
				m_visited.insert(n);
				auto dres = *m_residual->pesoAresta(curr, n);
				res = dres < cw ? dres : cw;
			}
			else
			{
				auto wei  = *m_residual->pesoAresta(curr, n);
				     wei  = wei < cw ? wei : cw;

				auto deep = FordFn::process_path(n, wei);

				if (!deep)
					continue;

				auto smal = *deep < wei ? *deep : wei;

				res = smal;
			}
		}

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

//	std::deque <id_t> path = { m_F };

//	view_path(*m_residual, m_F, path);
//
//	int pause;
//	std::cin >> pause;

	m_visited.clear();

	auto pmax = FordFn::process_path(m_F, (float) INT64_MAX);

	if (!pmax)
		return true;

	m_curr_max += *pmax;

	return false;
}

weight_t FordFn::max(void) noexcept {

	while (FordFn::step() == false) {}

	return m_curr_max;
}
