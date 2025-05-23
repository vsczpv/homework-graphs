#include "igraph.hpp"

void DSATUR::create_colors() noexcept {

	m_colors.clear();
	for (unsigned int i = 0; i < m_colors_number; i++) {
		m_colors[i] = Color(0,0,0);
	}

}

void DSATUR::resort(void) noexcept {

	std::sort(m_control_table.begin(), m_control_table.end(),
	[](
		std::pair<id_t, DSATUR::DSATURTableElement>& a,
		std::pair<id_t, DSATUR::DSATURTableElement>& b
	) {

		auto values = [](auto& a, auto& b) {
				if (a.second.satur == b.second.satur)
					return a.second.degree > b.second.degree;
				else
					return a.second.satur  > b.second.satur;
		};

		if (a.second.color.has_value() == false)
		{
			if (b.second.color.has_value() == false)
				return values(a, b);

			else
				return true; // a > b
		}

		else
		{
			if (b.second.color.has_value() == false)
				return false; // a < b
			else
				return values(a, b);
		}

		return true;
	});
}

void DSATUR::redo_satur(id_t vertex) noexcept
{

	auto nei = m_graph.retornarVizinhos(vertex);

	assert(nei.has_value() != false);

	std::set<id_t> colors = {};

	for (auto& n : *nei) {
		auto nptr = DSATUR::table_contains(m_control_table, n);
		assert(nptr != nullptr);
		if (nptr->color)
			colors.insert(*(nptr->color));
	}

	DSATUR::table_contains(m_control_table, vertex)->satur = colors.size();

	return;
}

void DSATUR::next_coloration() noexcept
{
	while (true) {

		if (DSATUR::table_closed(m_control_table))
			return;

		auto& v = m_control_table.front();

		id_t colorid;
		bool success = false;
		for (auto e : m_colors) {
			colorid = e.first;
			if (this->is_colorable(v.first, colorid))
			{
				success = true;
				break;
			}
		}

		if (success != true)
			return;

		v.second.color = colorid;

		auto nei = m_graph.retornarVizinhos(v.first);

		if (nei) for (auto& n : *nei) {
			this->redo_satur(n);
		}

		this->resort();
	}
}

bool DSATUR::is_colorable(id_t vertex, id_t color) noexcept
{
	auto nei = m_graph.retornarVizinhos(vertex);
	if (nei) {
		for (auto n : *nei) {
			auto nv = DSATUR::table_contains(m_control_table, n);
			if (nv != nullptr)
			{
				if (nv->color)
				{
//					std::cout << "DA" << *nv->color << " " << color << std::endl;
					if (*nv->color == color)
						return false;
				}
			} else {
				abort();
			}
		}
	}
//	std::cout << "DB is true" << std::endl;
	return true;
}

DSATUR::DSATURTableElement* DSATUR::table_contains(std::vector<std::pair<id_t, DSATUR::DSATURTableElement>>& t, id_t id) noexcept
{
	for (auto& e : t) {
		if (e.first == id)
			return &(e.second);
	}
	return nullptr;
}


DSATUR& DSATUR::color_graph() noexcept
{

	double time  = (double) clock() / CLOCKS_PER_SEC;

	auto verts = m_graph.getVertices();

	m_control_table.clear();

	for (auto v : verts) {

		auto degree = m_graph.retornarVizinhos(v).value_or(std::vector<id_t>{}).size();
		m_control_table.push_back(std::make_pair(
			v,
			DSATUR::DSATURTableElement {
				.degree = static_cast<int>(degree),
				.satur  = 0,
				.color  = std::nullopt,
			}
		));

	}

	m_colors_number = 0;
	m_burst_time    = 0;

	std::sort(m_control_table.begin(), m_control_table.end(), [](auto& a, auto& b) {
		return a.second.degree > b.second.degree;
	});

	while (DSATUR::table_open(m_control_table)) {
		this->reset();
		if (m_colors_number >= m_graph.getVertices().size())
			abort();
		m_colors_number++;
		this->create_colors();
		this->next_coloration();
	}

	m_burst_time = (double) clock() / CLOCKS_PER_SEC - time;

	return *this;
}

bool DSATUR::table_closed(std::vector<std::pair<id_t, DSATUR::DSATURTableElement>>& t) noexcept
{

	for (auto& e : t) {
		if (e.second.open())
			return false;
	}

	return true;
}

bool DSATUR::table_open(std::vector<std::pair<id_t, DSATUR::DSATURTableElement>>& t) noexcept
{
	return !DSATUR::table_closed(t);
}

double DSATUR::get_burst_time() noexcept {
	return m_burst_time;
}

void DSATUR::print_output_list() noexcept
{
	std::cout << std::dec;
	for (auto& v : m_control_table) {
		std::cout << v.first << "\t" << " -> " << (v.second.color.has_value() ? std::to_string(*v.second.color) : "ø") << std::endl;
	}
}

int DSATUR::get_colors_number() noexcept
{
	return m_colors_number;
}

void DSATUR::reset() noexcept
{
	for (auto& e : m_control_table) {
		e.second.color = std::nullopt;
		e.second.satur = 0;
	}

	std::sort(m_control_table.begin(), m_control_table.end(), [](auto& a, auto& b) {
		return a.second.degree > b.second.degree;
	});
}

