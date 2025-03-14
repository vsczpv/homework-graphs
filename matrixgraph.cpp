#include "matrixgraph.hpp"

bool MatrixGraph::inserirVertice(std::string label) noexcept {
	return m_stg.put(label);
}

bool MatrixGraph::removerVertice(id_t idx) noexcept {
	return m_stg.yank(idx);
}

bool MatrixGraph::inserirAresta(id_t A, id_t B, weight_t peso) noexcept {

	// TODO
	assert(!m_pond && !m_dir);

	if (B > m_stg.max || A > m_stg.max)
		return true;

	// TODO: Generalize
	if (m_stg[B][A] != 0 && m_stg[A][B] != 0)
		return true;

	m_stg[B][A] = peso;
	m_stg[A][B] = peso;

	return false;
}

bool MatrixGraph::removerAresta (id_t A, id_t B) noexcept {

	// TODO
	assert(!m_pond && !m_dir);

	if (B > m_stg.max || A > m_stg.max)
		return true;

	// TODO: Generalize
	if (m_stg[B][A] == 0 || m_stg[A][B] == 0)
		return true;

	m_stg[B][A] = 0;
	m_stg[A][B] = 0;

	return false;
}

std::optional<std::string>
MatrixGraph::labelVertice(id_t idx) const noexcept {

	if (idx > m_stg.labels.size() || m_stg.labels[idx].first == false)
		return std::nullopt;
	else
		return m_stg.labels[idx].second;
}

bool MatrixGraph::existeAresta(id_t A, id_t B) const noexcept {

	if (B > m_stg.max || A > m_stg.max)
		return false;

	if (m_stg[B][A] != 0)
		return true;

	else
		return false;
}

std::optional<weight_t>
MatrixGraph::pesoAresta(id_t A, id_t B) const noexcept {

	if (B > m_stg.max || A > m_stg.max)
		return std::nullopt;

	if (m_stg[B][A] == 0)
		return std::nullopt;

	return m_stg[B][A];
}

std::optional<std::vector<id_t>>
MatrixGraph::retornarVizinhos(id_t idx) const noexcept {

	// TODO
	return std::nullopt;
}

void MatrixGraph::imprimeGrafo(void) const noexcept {

}
