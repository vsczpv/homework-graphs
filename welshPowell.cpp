

#include "igraph.hpp"

#include <time.h>
#include <stdio.h>


#define NSEC_PER_SEC    (1000000000)

WelshPowell::WelshPowell(IGraph& graph)
    : m_graph(graph)
{}


void WelshPowell::create_colors() noexcept {
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

                m_colors.push_back(std::make_pair(cont, color_aux));
                cont++;
            }
        }
    }

}

void WelshPowell::next_coloration() noexcept {

    
    //std::cout << "FUNC\n";
    for(auto v = m_control_table.begin(); v != m_control_table.end(); ) { // para cada vértice
        if(is_colorable(v->first, m_colors_number - 1)) {        // se não tem vizinho com a cor atual
           v->second.color = m_colors_number - 1;               // colore

            m_final_table.insert(std::pair<id_t, WelshPowell::PowelTableElement>(v->first, v->second)); // adiciona na tabela de saída
            v = m_control_table.erase(v);                     // remove da tabela de controle
        } else {
            ++v;
        }
    }


}


WelshPowell& WelshPowell::color_graph() noexcept {

    /* get initial time */
    double time1;
    time1 = (double) clock();
    time1 = time1 / CLOCKS_PER_SEC;


    /* Reset graph list */
    auto verts = m_graph.getVertices();
    m_final_table.clear();
    m_control_table = {};
    for (auto v : verts){
        m_control_table.push_back(std::pair<id_t, WelshPowell::PowelTableElement>(v, WelshPowell::PowelTableElement( m_graph.retornarVizinhos(v).value_or(std::vector<id_t> {}).size(), 0)));
    }
    m_colors_number = 0;
    m_burst_time    = 0;
    m_number_of_colored_vertices = 0;

    

    /* Order by degree */
    m_control_table.sort([](auto& a, auto& b) {
        return a.second.degree > b.second.degree;
    });

    /*for(auto v : m_control_table) {
        std::cout << v.first << " -> " << v.second.degree << std::endl;
    }
    std::cout << "----------------------------------------" << std::endl;*/



    /* Color by Welsh Powell */

	puts("");

	setvbuf(stdout, NULL, _IONBF, 0);

    while(!m_control_table.empty()) {
        m_colors_number++; // cria nova cor

		printf("Qtd. cores atual: %04u   \r", m_colors_number);

        next_coloration();
    }

    puts("");

	setvbuf(stdout, stdout_buffer, _IOLBF, STDOUT_BUFFER_SZ);
    
    
    /* Get time to execute */
    m_burst_time = ( ((double) clock()) / CLOCKS_PER_SEC) - time1;


    /* Create table of used colors*/
    create_colors();
    return *this;
}

bool WelshPowell::is_colorable(id_t vertex, id_t color) noexcept {
    
        auto vizinhos = m_graph.retornarVizinhos(vertex);
        if (vizinhos) { // Se retornou vizinhos
            for (auto vv : *vizinhos) { // Para cada vizinho
                if (m_final_table.find(vv) != m_final_table.end() && m_final_table.find(vv)->second.color == color) return false; // Verifica se tem a cor atual
            }
        }
    
    return true;
}

void WelshPowell::print_output_list() noexcept {
    
    for (auto v : m_final_table) {
		std::cout << std::dec;
        std::cout << v.first << "\t" << " -> ";
        
        std::cout.width(6);
        std::cout.fill('0');
        std::cout << std::hex << m_colors.at(v.second.color).second.get_color() << std::endl;
    }

    std::cout << std::dec;
}



double WelshPowell::get_burst_time() noexcept {
    return m_burst_time;
}
int WelshPowell::get_colors_number() noexcept {
    return m_colors_number;
}
std::vector<std::pair<id_t, Color>> WelshPowell::get_colors() noexcept {
    return m_colors;
}
std::map<id_t, WelshPowell::PowelTableElement> WelshPowell::get_output_list() noexcept {
    return m_final_table;
}
