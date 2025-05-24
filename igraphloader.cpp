#include <cassert>
#include "igraphloader.hpp"
#include "matrixgraph.hpp"
#include "listgraph.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <set>

IGraphType igraphtype_from_bool(bool b)
{
	if (b) return IGraphType::Matrix;
	else   return IGraphType::List;
}

IGraph* IGraphLoader::from_file(std::filesystem::path path) noexcept
{

	setvbuf(stdout, NULL, _IONBF, 0);

	std::ifstream file;
	file.open(path, std::ios::in);

	if (!file.is_open())
		return NULL;

	IGraph* res = NULL;
	auto filesz = std::filesystem::file_size(path);
	size_t read = 0;
	size_t mod = filesz / 100;
	size_t vertsproc = 0;
	size_t edgesproc = 0;

	if (mod == 0)
		mod = 1;

	std::string line;
	int V, A, D, P;

	std::deque<std::tuple<id_t, id_t, weight_t>> edges;
	std::set<id_t> verts;

	{
		if (!std::getline(file, line)) goto rid_of_file;
		read += line.length() + 1;
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

		read += line.length() + 1;

		if (read % mod == 0)
			printf("Carregando arquivo... %03lu%%\r", read * 100 / filesz);

		std::istringstream iss(line);
		id_t v0, v1;
		weight_t pond;

		iss >> v0 >> v1;
		iss >> pond;

		edges.push_back(std::make_tuple(v0, v1, pond));

		assert(verts.insert(v0).first != verts.end());
		assert(verts.insert(v1).first != verts.end());
	}

	puts("");

	for (auto v : verts)
	{
		vertsproc++;
		auto vertdiv = verts.size() / 100;
		vertdiv = vertdiv == 0 ? 1 : vertdiv;
		if (vertsproc % (vertdiv) == 0)
			printf("Interpretando vertices... %03lu%%   \r", (vertsproc * 100 / verts.size()));
		if (res->inserirVertice(std::to_string(v)))
			goto rid_of_file;
	}

	puts("");

	for (auto e : edges)
	{
		edgesproc++;
		auto edgediv = edges.size() / 100;
		edgediv = edgediv == 0 ? 1 : edgediv;
		if (edgesproc % (edgediv) == 0)
			printf("Interpretando arestas...  %03lu%%   \r", (edgesproc * 100 / edges.size()));
		if (res->inserirAresta(*res->labelidx(std::to_string(std::get<0>(e))), *res->labelidx(std::to_string(std::get<1>(e))), std::get<2>(e)))
			goto rid_of_file;
	}

	puts("");

	setvbuf(stdout, stdout_buffer, _IOLBF, STDOUT_BUFFER_SZ);

	file.close();

	return res;

rid_of_file:

	file.close();

	if (res != NULL)
		delete res;

	return NULL;
}

