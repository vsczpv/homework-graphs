#include "matrixgraph.hpp"
#include <iostream>

std::optional<id_t> MatrixGraph::labelidx(std::string label) noexcept
{
	return m_stg.labelidx(label);
}

bool MatrixGraph::pond(void) noexcept {
	return m_pond;
}

bool MatrixGraph::dir (void) noexcept {
	return m_dir;
}

bool MatrixGraph::inserirVertice(std::string label) noexcept {
	return m_stg.put(label);
}

bool MatrixGraph::removerVertice(id_t idx) noexcept {
	for (id_t i = 0; i < m_stg.max; i++)
		this->removerAresta(idx, i),
		this->removerAresta(i, idx);
	return m_stg.yank(idx);
}

bool MatrixGraph::inserirAresta(id_t A, id_t B, weight_t peso) noexcept {

	if (!m_pond) peso = 1;

	if (B > m_stg.max || A > m_stg.max)
		return true;

	// TODO: Generalize
	if (m_stg[B][A] != 0)
		return true;

	if (!m_dir && m_stg[A][B] != 0)
		return true;

	if (!m_stg.labels[A].first || !m_stg.labels[B].first)
		return true;

	m_stg[B][A] = peso;

	if (!m_dir)
		m_stg[A][B] = peso;

	return false;
}

bool MatrixGraph::removerAresta (id_t A, id_t B) noexcept {

	if (B > m_stg.max || A > m_stg.max)
		return true;

	// TODO: Generalize
	if (m_stg[B][A] == 0)
		return true;

	if (!m_dir && m_stg[A][B] == 0)
		return true;

	m_stg[B][A] = 0;
	if (!m_dir)
		m_stg[A][B] = 0;

	return false;
}

std::optional<std::string>
MatrixGraph::labelVertice(id_t idx) noexcept {

	if (idx > m_stg.labels.size() || m_stg.labels[idx].first == false)
		return std::nullopt;
	else
		return m_stg.labels[idx].second;
}

bool MatrixGraph::existeAresta(id_t A, id_t B) noexcept {

	if (B > m_stg.max || A > m_stg.max)
		return false;

	if (m_stg[B][A] != 0)
		return true;

	else
		return false;
}

std::optional<weight_t>
MatrixGraph::pesoAresta(id_t A, id_t B) noexcept {

	if (B > m_stg.max || A > m_stg.max)
		return std::nullopt;

	if (m_stg[B][A] == 0)
		return std::nullopt;

	return m_stg[B][A];
}

std::optional<std::vector<id_t>>
MatrixGraph::retornarVizinhos(id_t idx) noexcept {

	if (idx > m_stg.max)
		return std::nullopt;

	if (m_stg.labels[idx].first == false)
		return std::nullopt;

	std::vector<id_t> res;

	// TODO: Generalize
	for (id_t j = 0; j < m_stg.max; j++)
		if (m_stg[j][idx] != 0)
			res.push_back(j);

	return res;
}

void MatrixGraph::imprimeGrafo(void) noexcept {

	std::printf("    ");

	for (auto lbl : m_stg.labels)
		if (lbl.first)
			std::printf("%-4s", lbl.second.c_str());

	std::puts("");

	for (id_t y = 0; y < m_stg.max; y++) {
		if (m_stg.labels[y].first) {
			std::printf("%-4s", m_stg.labels[y].second.c_str());
			for (id_t x = 0; x < m_stg.max; x++) {
				if (!m_stg.labels[x].first) continue;
				auto num = m_stg[y][x];
				std::printf("%-4g", num);
			}
			std::printf("\t\tID %i\n", y);
		}
	}

}


std::vector<id_t> MatrixGraph::getVertices() noexcept {
    std::vector<id_t> allVertices;
	for (id_t i = 0; i < m_stg.labels.size(); i++)
		if (m_stg.labels[i].first)
			allVertices.push_back(i);
    return allVertices;
}
