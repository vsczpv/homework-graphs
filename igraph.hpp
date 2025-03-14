#ifndef IGRAPH_HPP_
#define IGRAPH_HPP_

#include <string>
#include <vector>
#include <optional>

using weight_t = int;
using id_t     = uint32_t;

class IGraph {

public:

	virtual bool        inserirVertice(std::string label)                 noexcept;
	virtual bool        removerVertice(id_t idx)                          noexcept;
	virtual bool        inserirAresta (id_t A, id_t B, weight_t peso = 1) noexcept;
	virtual bool        removerAresta (id_t A, id_t B)                    noexcept;

	virtual std::optional<std::string>
	                    labelVertice  (id_t idx)                    const noexcept;

	virtual bool        existeAresta  (id_t A, id_t B)              const noexcept;

	virtual std::optional<weight_t>
						pesoAresta    (id_t A, id_t B)              const noexcept;

	virtual std::optional<std::vector<id_t>>
	                   retornarVizinhos(id_t idx)                   const noexcept;

	virtual void imprimeGrafo(void) const noexcept;
};

#endif // IGRAPH_HPP_
