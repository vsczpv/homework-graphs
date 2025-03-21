#ifndef LIST_GRAPH_WRAPPER_HPP_
#define LIST_GRAPH_WRAPPER_HPP_

#include "listgraph.hpp"
#include "igraph.hpp"

// TODO: Not use a wrapper
class ListGraphWrapper : public IGraph {
private:

	Grafo m_grafo;

public:

	ListGraphWrapper(bool pond, bool dir);

	bool inserirVertice(std::string label)                 noexcept override;
	bool removerVertice(id_t idx)                          noexcept override;
	bool inserirAresta (id_t A, id_t B, weight_t peso = 1) noexcept override;
	bool removerAresta (id_t A, id_t B)                    noexcept override;

	std::optional<std::string> labelVertice(id_t idx) noexcept override;

	bool existeAresta(id_t A, id_t B) noexcept override;

	std::optional<weight_t> pesoAresta(id_t A, id_t B) noexcept override;

	std::optional<std::vector<id_t>> retornarVizinhos(id_t idx) noexcept override;

	void imprimeGrafo(void) noexcept override;

	bool pond(void) noexcept override;
	bool dir (void) noexcept override;
};

#endif // LIST_GRAPH_WRAPPER_HPP_
