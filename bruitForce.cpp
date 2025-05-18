

#include "igraph.hpp"

#include <time.h>


#define NSEC_PER_SEC    (1000000000)

BruitForce::BruitForce(IGraph& graph)
    : m_graph(graph)
{}

BruitForce& BruitForce::next_possibility() noexcept { 

    for (auto v : m_output_list) {
        // vai somando 1 a um número m_colors_number-ecimal, o qual o id de cor de cada vértice é uma casa do número
        if (v.second == m_colors_number - 1) {
            m_output_list.at(v.first).second = 0;

            if(v.first == m_output_list.back().first){
                m_colors_number++;

            }
        }
        else {
            m_output_list.at(v.first).second = v.second + 1;
            break;
        }
    }
    return *this;
}

void BruitForce::create_colors() noexcept {
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

BruitForce& BruitForce::color_graph() noexcept {

    /* get initial time */
    double time1;
    time1 = (double) clock();
    time1 = time1 / CLOCKS_PER_SEC;


    /* Reset graph list */
    auto verts = m_graph.getVertices();
    m_output_list.clear();
    for (auto v : verts){
        this->m_output_list.push_back(std::make_pair(v, 0));
    }
    m_colors_number = 1;
    m_burst_time    = 0; 


    /* Color the Graph */
    while (!is_valid()) {
        next_possibility();
    }
    
    
    /* Get time to execute */
    m_burst_time = ( ((double) clock()) / CLOCKS_PER_SEC) - time1;


    /* Create table of used colors*/
    create_colors();
    return *this;
}

bool BruitForce::is_valid() noexcept {
    for (auto v : m_output_list) {
        auto vizinhos = m_graph.retornarVizinhos(v.first);
        if (vizinhos) {
            for (auto vv : *vizinhos) {
                if (m_output_list[vv].second == v.second) return false;
            }
        }
    }
    return true;
}

void BruitForce::print_output_list() noexcept {
    
    for (auto v : m_output_list) {
        std::cout << v.first << "\t" << " -> ";
        
        std::cout.width(6);
        std::cout.fill('0');
        std::cout << std::hex << m_colors.at(v.second).second.get_color() << std::endl;
    }
}



double BruitForce::get_burst_time() noexcept {
    return m_burst_time;
}
int BruitForce::get_colors_number() noexcept {
    return m_colors_number;
}
std::vector<std::pair<id_t, Color>> BruitForce::get_colors() noexcept {
    return m_colors;
}
std::vector<std::pair<id_t, id_t>> BruitForce::get_output_list() noexcept {
    return m_output_list;
}