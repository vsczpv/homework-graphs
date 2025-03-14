#ifndef MATRIX_GRAPH_HPP_
#define MATRIX_GRAPH_HPP_

#include <cassert>
#include <utility>

#include "igraph.hpp"

#define MATRIX_MAX 32

using std::pair;

struct MatrixGraphStorage {
	size_t max;
	weight_t *M; /* sizeof (weight_t) * cols * rows; */
	std::vector<pair<bool, std::string>> labels;

	/**
	 * operator[]
	 *
	 * Rows then Columns
	 *
	 * eg.:
	 *
	 * int y = 3, x = 5;
	 *
	 * int val = matrix[y][x];
	 *
	 */
	weight_t* operator[](unsigned long long int idx) const {
		return &M[idx * max];
	};
	bool yank(id_t idx) {
		if (idx > this->labels.size())
			return true;
		if (this->labels[idx].first == false)
			return true;
		this->labels[idx].first = false;
		return false;
	}
	bool put(std::string label) {
		for (id_t i = 0; i < this->max; i++)
			if (this->labels[i].first == false)
			{
				this->labels[i].first  = true,
				this->labels[i].second = label;
				return false;
			}
		return true;
	}
	std::optional<id_t> labelidx(std::string label) {
		for (id_t i = 0; i < labels.size(); i++)
			if (this->labels[i].first && this->labels[i].second == label)
				return i;
		return std::nullopt;
	};
	MatrixGraphStorage(int max) {
		this->M      = new int[max * max];
		this->max    = max;
		this->labels = std::vector<pair<bool, std::string>>{};
	}
};

class MatrixGraph : public IGraph {
private:

	MatrixGraphStorage m_stg;
	bool m_pond, m_dir;

public:

	MatrixGraph(bool pond, bool dir)
		: m_stg (MATRIX_MAX),
		  m_pond(pond),
		  m_dir (dir)

	{}

	bool inserirVertice(std::string label)                 noexcept;
	bool removerVertice(id_t idx)                          noexcept;
	bool inserirAresta (id_t A, id_t B, weight_t peso = 1) noexcept;
	bool removerAresta (id_t A, id_t B)                    noexcept;

	std::optional<std::string> labelVertice(id_t idx) const noexcept;

	bool existeAresta(id_t A, id_t B) const noexcept;

	std::optional<weight_t> pesoAresta(id_t A, id_t B) const noexcept;

	std::optional<std::vector<id_t>> retornarVizinhos(id_t idx) const noexcept;

	void imprimeGrafo(void) const noexcept;
};

#endif // MATRIX_GRAPH_HPP_
