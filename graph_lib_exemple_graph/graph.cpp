#include "graph.h"
#include <fstream>
#include <iostream>



/// eidx index of edge to remove
void Graph::remove_edge(int eidx,int cas)
{
/// référence vers le Edge à enlever
    Edge &supr=m_edges[eidx];
//std::cout << "Removing edge " << eidx << " " << remed.m_from << "->" << remed.m_to << " " << remed.m_weight << std::endl;

/// Tester la cohérence : nombre d'arc entrants et sortants des sommets 1 et 2
//std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
//std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
//std::cout << m_edges.size() << std::endl;

/// test : on a bien des éléments interfacés
    if (m_interface && supr.m_interface)
    {
        m_interface->m_main_box.remove_child( supr.m_interface->m_top_edge );
    }
/// Il reste encore à virer l'arc supprimé de la liste des entrants et sortants des 2 sommets to et from !
/// References sur les listes de edges des sommets from et to
    if (cas ==1)
    {
        std::vector<int> &from = m_vertices[supr.m_from].m_out;
        std::vector<int> &to = m_vertices[supr.m_to].m_in;
        from.erase( std::remove(from.begin(), from.end(), eidx ), from.end() );
        to.erase( std::remove( to.begin(), to.end(), eidx ), to.end() );
        m_edges.erase( eidx );

    }
    if(cas==2)
    {
        m_edges[eidx].m_actif=false;
    }


/// Tester la cohérence : nombre d'arc entrants et sortants des sommets 1 et 2
    /*std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    std::cout << m_edges.size() << std::endl;*/

}

void Graph::remove_vertex(int vdx,int cas)
{
    ///Reference vers le sommet (vertex) a enlever
    Vertex supr=m_vertices[vdx];
    ///on supprime toutes les aretes liées a ce sommet
    for (int i = supr.m_in.size() -1 ; i >= 0  ; i --)
    {
        remove_edge(supr.m_in[i],cas);
    }

    for (int i = supr.m_out.size() -1 ; i >= 0  ; i --)
    {
        remove_edge(supr.m_out[i],cas);
    }
    ///on supprime tous les elements interfacés
    if (m_interface && supr.m_interface)
    {
        std::cout << "je supprime un sommet" << std::endl;
        m_interface->m_main_box.remove_child( supr.m_interface->m_top_box );
    }
    if(cas ==1)
    {
        ///on supprime le sommet
        m_vertices.erase(vdx);
    }
    if(cas==2)
    {
        m_vertices[vdx].m_actif=false;

        std::cout<<"ON SUPPRIME LE VERTEX "<< vdx << std::endl;

    }

}



void Graph::add_vertex(std::string image)
{
    int indice = 0;
    for (auto a=m_vertices.begin() ; a != m_vertices.end(); a++)
    {
        indice = a->first;
    }

    add_interfaced_vertex(indice + 1, 30, 100, 100, image);

}

void Graph::add_edge(int from, int to)
{
    int indice;
    for (auto a=m_edges.begin() ; a != m_edges.end(); a++)
        indice = a->first;
    //std::cout << "coucou13\n";
    add_interfaced_edge(indice + 1,from,to,50);
    //std::cout << "coucou 14";
}

void Graph::vider_graph()
{
    // std::cout << "\ncoucou";

    while (m_vertices.size() != 0)
    {
        //  std ::cout << "\n" << m_vertices.begin()->first;
        remove_vertex(m_vertices.begin()->first,1);
    }
}






