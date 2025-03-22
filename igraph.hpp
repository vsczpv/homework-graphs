#ifndef IGRAPH_HPP_
#define IGRAPH_HPP_

#include <string>
#include <vector>
#include <optional>
#include <cstdint>

using weight_t = float;
using id_t     = uint32_t;

class IGraph {

public:

	virtual bool        inserirVertice(std::string label)                 noexcept = 0;
	virtual bool        removerVertice(id_t idx)                          noexcept = 0;
	virtual bool        inserirAresta (id_t A, id_t B, weight_t peso = 1) noexcept = 0;
	virtual bool        removerAresta (id_t A, id_t B)                    noexcept = 0;

	virtual std::optional<std::string>
	                    labelVertice  (id_t idx)                    noexcept = 0;

	virtual bool        existeAresta  (id_t A, id_t B)              noexcept = 0;

	virtual std::optional<weight_t>
						pesoAresta    (id_t A, id_t B)              noexcept = 0;

	virtual std::optional<std::vector<id_t>>
	                   retornarVizinhos(id_t idx)                   noexcept = 0;

	virtual void imprimeGrafo(void) noexcept = 0;

	virtual bool pond(void) noexcept = 0;
	virtual bool dir (void) noexcept = 0;

	virtual ~IGraph() = default;
};

#endif // IGRAPH_HPP_
