#include "listgraphwrapper.hpp"

bool ListGraphWrapper::pond(void) noexcept {
	return m_grafo.ponderado;
}

bool ListGraphWrapper::dir (void) noexcept {
	return m_grafo.direcionado;
}

ListGraphWrapper::ListGraphWrapper(bool pond, bool dir)
{
	m_grafo = Grafo();
	m_grafo.ponderado   = pond;
	m_grafo.direcionado = dir;
}

bool ListGraphWrapper::inserirVertice(std::string label) noexcept {
	return !m_grafo.inserirVertice(label);
}

bool ListGraphWrapper::removerVertice(id_t idx) noexcept {
	return !m_grafo.removerVertice(idx);
}

bool ListGraphWrapper::inserirAresta(id_t A, id_t B, weight_t peso) noexcept {
	return !m_grafo.inserirAresta(A, B, peso);
}
bool ListGraphWrapper::removerAresta (id_t A, id_t B) noexcept {
	return !m_grafo.removerAresta(A, B);
}

std::optional<std::string> ListGraphWrapper::labelVertice(id_t idx) noexcept {
	try {
		return m_grafo.labelVertice(idx);
	} catch(...) {
		return std::nullopt;
	}
}

bool ListGraphWrapper::existeAresta(id_t A, id_t B) noexcept {
	return !m_grafo.existeAresta(A, B);
}

std::optional<weight_t> ListGraphWrapper::pesoAresta(id_t A, id_t B) noexcept {
	int res = m_grafo.pesoAresta(A, B);
	if (res == -1)
		return std::nullopt;
	else
		return (weight_t) res;
}

std::optional<std::vector<id_t>> ListGraphWrapper::retornarVizinhos(id_t idx) noexcept {
	// TODO: Error handling;
	return m_grafo.retornarVizinhos(idx);
}

void ListGraphWrapper::imprimeGrafo(void) noexcept {
	m_grafo.imprimeGrafo();
}
