#include "igraph.hpp"

void NoOrder::create_colors() noexcept {

	m_colors.clear();
	for (unsigned int i = 0; i < m_colors_number; i++) {
		m_colors[i] = Color(0,0,0);
	}

}

void NoOrder::next_coloration() noexcept
{

	for (auto& v : m_control_table) {

		if (v.second.closed())
			continue;

		id_t colorid = m_colors_number - 1;
		bool success = false;

		if (this->is_colorable(v.first, colorid))
			success = true;

		if (success != true)
			continue;

		v.second.color = colorid;
	}
}

bool NoOrder::is_colorable(id_t vertex, id_t color) noexcept
{
	auto nei = m_graph.retornarVizinhos(vertex);
	if (nei) {
		for (auto n : *nei) {
			auto nv = NoOrder::table_contains(m_control_table, n);
			if (nv != nullptr)
			{
				if (nv->color)
				{

					if (*nv->color == color)
						return false;
				}
			} else {
				abort();
			}
		}
	}

	return true;
}

NoOrder::NoOrderTableElement* NoOrder::table_contains(std::vector<std::pair<id_t, NoOrder::NoOrderTableElement>>& t, id_t id) noexcept
{
	for (auto& e : t) {
		if (e.first == id)
			return &(e.second);
	}
	return nullptr;
}


NoOrder& NoOrder::color_graph() noexcept
{

	double time  = (double) clock() / CLOCKS_PER_SEC;

	auto verts = m_graph.getVertices();

	m_control_table.clear();

	for (auto v : verts) {

//		auto degree = m_graph.retornarVizinhos(v).value_or({}).size();
		m_control_table.push_back(std::make_pair(
			v,
			NoOrder::NoOrderTableElement {
				.color  = std::nullopt,
			}
		));

	}

	m_colors_number = 0;
	m_burst_time    = 0;

	puts("");

	setvbuf(stdout, NULL, _IONBF, 0);

	while (NoOrder::table_open(m_control_table)) {
//		this->reset();
		if (m_colors_number >= m_graph.getVertices().size())
			abort();
		m_colors_number++;

		printf("Qtd. cores atual: %04u   \r", m_colors_number);

		this->next_coloration();
	}

	puts("");

	setvbuf(stdout, stdout_buffer, _IOLBF, STDOUT_BUFFER_SZ);

	m_burst_time = (double) clock() / CLOCKS_PER_SEC - time;

	this->create_colors();
	return *this;
}

bool NoOrder::table_closed(std::vector<std::pair<id_t, NoOrder::NoOrderTableElement>>& t) noexcept
{

	for (auto& e : t) {
		if (e.second.open())
			return false;
	}

	return true;
}

bool NoOrder::table_open(std::vector<std::pair<id_t, NoOrder::NoOrderTableElement>>& t) noexcept
{
	return !NoOrder::table_closed(t);
}

double NoOrder::get_burst_time() noexcept {
	return m_burst_time;
}

void NoOrder::print_output_list() noexcept
{
	std::cout << std::dec;
	for (auto& v : m_control_table) {
		std::cout << v.first << "\t" << " -> " << (v.second.color.has_value() ? std::to_string(*v.second.color) : "ø") << std::endl;
	}
}

int NoOrder::get_colors_number() noexcept
{
	return m_colors_number;
}

void NoOrder::reset() noexcept
{
	for (auto& e : m_control_table)
		e.second.color = std::nullopt;
}
