#include "igraph.hpp"

void DSATUR::create_colors() noexcept {

	/*
	m_colors.clear();
	for (unsigned int i = 0; i < m_colors_number; i++) {
		m_colors[i] = Color(0,0,0);
	}
	*/

	   // Gera uma lista de cores com uma tamanho expoente de 2 que seja suficiente para armazenar todas as cores.
    m_colors.clear();

    Color color_aux(0, 0, 0);
    unsigned int passo_div_r = 0, passo_div_g = 0 , passo_div_b = 0;

    while(true) {

        static int i = 0;
        switch (i % 3) {
            case 0:
                passo_div_r++;
                break;
            case 1:
                passo_div_g++;
                break;
            case 2:
                passo_div_b++;
                break;
        }
        i++;
        if((passo_div_r + 1) * (passo_div_g + 1) * (passo_div_b + 1) > m_colors_number) break;
    };
    unsigned int cont = 0;
    for (unsigned int r = 0x00; r <= passo_div_r; r++) {
        if (r) color_aux.r = r * (0xff / passo_div_r); else color_aux.r = 0x00;

        for (unsigned int g = 0x00; g <= passo_div_g; g++) {
            if(g) color_aux.g = g * (0xff / passo_div_g); else color_aux.g = 0x00;

            for (unsigned int b = 0x00; b <= passo_div_b; b++) {
                if (b) color_aux.b = b * (0xff / passo_div_b); else color_aux.b = 0x00;

//                m_colors.push_back(std::make_pair(cont, color_aux));
				m_colors[cont] = color_aux;
                cont++;
            }
        }
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

		if (m_closed == m_graph.getVertices().size())
			return;

		auto& v = m_control_table.front();

		id_t colorid;
		bool success = false;
		for (colorid = 0; colorid < m_colors_number; colorid++) {
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

		m_closed++;
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

	m_closed        = 0;
	m_colors_number = 0;
	m_burst_time    = 0;

	std::sort(m_control_table.begin(), m_control_table.end(), [](auto& a, auto& b) {
		return a.second.degree > b.second.degree;
	});

	puts("");

	setvbuf(stdout, NULL, _IONBF, 0);

	while (DSATUR::table_open(m_control_table)) {
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
	/*
	std::cout << std::dec;
	for (auto& v : m_control_table) {
		std::cout << v.first << "\t" << " -> " << (v.second.color.has_value() ? std::to_string(*v.second.color) : "ø") << std::endl;
	}
	*/
	for (auto v : m_control_table) {
		std::cout << std::dec;
        std::cout << v.first << "\t" << " -> ";

        std::cout.width(6);
        std::cout.fill('0');
		if (v.second.color)
		{
//			printf("\033[38;2;%");
			std::cout << std::hex << m_colors[*v.second.color].get_color() << std::endl;
		}
		else
			std::cout << "ø" << std::endl;
    }

    std::cout << std::dec;
}

int DSATUR::get_colors_number() noexcept
{
	return m_colors_number;
}

void DSATUR::reset() noexcept
{

	m_closed = 0;

	for (auto& e : m_control_table) {
		e.second.color = std::nullopt;
		e.second.satur = 0;
	}

	std::sort(m_control_table.begin(), m_control_table.end(), [](auto& a, auto& b) {
		return a.second.degree > b.second.degree;
	});
}

