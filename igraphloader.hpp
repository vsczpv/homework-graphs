#ifndef IGRAPHLOADER_HPP_
#define IGRAPHLOADER_HPP_

#include <filesystem>

#include "igraph.hpp"

enum class IGraphType {
	Matrix,
	List
};

IGraphType igraphtype_from_bool(bool b);

class IGraphLoader {
private:
	IGraphType m_type;
public:
	IGraphLoader(IGraphType type)
		: m_type(type)
	{}

	IGraph* from_file(std::filesystem::path path) noexcept;
};

#endif // IGRAPHLOADER_HPP_
