#include "graph.h"
#include <fstream>
#include <iostream>


/***************************************************
                    VERTEX
****************************************************/
/// Gestion du Vertex avant l'appel à l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_value vers le slider associé
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donnée m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}

/// Gestion du Vertex après l'appel à l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_value locale

    m_value = m_interface->m_slider_value.get_value();
}


/***************************************************
                    EDGE
****************************************************/

/// Gestion du Edge avant l'appel à l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_weight vers le slider associé
    m_interface->m_slider_weight.set_value(m_weight);

    m_interface->m_top_edge.modifierTaille(m_weight);
    /// Copier la valeur locale de la donnée m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );

    m_interface->m_autor_supr = false;
}

/// Gestion du Edge après l'appel à l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}

/***************************************************
                    GRAPH
****************************************************/

/// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
    int cas=2;
    int indice = -1;
    if (!m_interface)
        return;

    WraperBoutons();

    for (auto &elt : m_vertices)
        elt.second.pre_update();

    for (auto &elt : m_edges)
        elt.second.pre_update();
    for (auto &elt : m_edges)
        elt.second.m_autorisation_supr_edge = true;

    m_interface->m_top_box.update(); /// a placer au milieu

    for (auto &elt : m_edges)
        elt.second.post_update();

    for (auto elt = m_edges.begin() ; elt != m_edges.end() ; ++elt)
    {

        if (elt->second.m_interface->m_box_Edge_close.get_value() == true && elt->second.m_actif == true)
        {
            indice = elt->first;
            cas=1;
        }
    }

    if(indice != -1)
    {
        remove_edge(indice,cas);
        indice = -1;
    }
    for (auto &elt : m_vertices)
    {
        elt.second.post_update();
        if (elt.second.m_interface->m_box_close.get_value() == true && elt.second.m_actif == true)
        {
            indice = elt.first;
            cas=1;
        }
    }
    if (indice != -1)
    {
        remove_vertex(indice,cas);
        // std::cout << "couccou leo" << std::endl;

        indice = -1;
    }
     remplir_tab_adj_sym();
 //   std::cout << "bool connexe:"   << Graphe_connexe()  << std::endl;
}

