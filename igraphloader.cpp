#include <cassert>
#include "igraphloader.hpp"
#include "matrixgraph.hpp"
#include "listgraph.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <set>

IGraphType igraphtype_from_bool(bool b)
{
	if (b) return IGraphType::Matrix;
	else   return IGraphType::List;
}

IGraph* IGraphLoader::from_file(std::filesystem::path path) noexcept
{

	IGraph* res = NULL;

	std::ifstream file;
	file.open(path, std::ios::in);

	if (!file.is_open())
		return NULL;

	std::string line;
	int V, A, D, P;

	std::deque<std::tuple<id_t, id_t, weight_t>> edges;
	std::set<id_t> verts;

	{
		if (!std::getline(file, line)) goto rid_of_file;
		std::istringstream iss(line);
		iss >> V >> A >> D >> P;
	}

	(void) V;
	(void) A;

	switch (m_type)
	{
		case IGraphType::List:
			res = new ListGraph   ((bool) P, (bool) D); break;
		case IGraphType::Matrix:
			res = new MatrixGraph ((bool) P, (bool) D); break;
		default:
			assert(false);
	}

	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		id_t v0, v1;
		weight_t pond;

		iss >> v0 >> v1;
		iss >> pond;

		edges.push_back(std::make_tuple(v0, v1, pond));

		assert(verts.insert(v0).first != verts.end());
		assert(verts.insert(v1).first != verts.end());
	}

	for (auto v : verts)
		if (res->inserirVertice(std::to_string(v)))
			goto rid_of_file;

	for (auto e : edges)
	{

		if (res->inserirAresta(*res->labelidx(std::to_string(std::get<0>(e))), *res->labelidx(std::to_string(std::get<1>(e))), std::get<2>(e)))
			goto rid_of_file;
	}

	file.close();

	return res;

rid_of_file:

	file.close();

	if (res != NULL)
		delete res;

	return NULL;
}

