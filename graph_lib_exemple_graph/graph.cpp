#include "graph.h"
#include <fstream>
#include <iostream>

/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les éléments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de réglage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 100.0);  // Valeurs arbitraires, à adapter...
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    // Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( std::to_string(idx) );

    m_top_box.add_child(m_box_close);
    m_box_close.set_dim(10,10);
    m_box_close.set_pos(116,0);
    m_box_close.set_bg_color(VERT);
}


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

/// Le constructeur met en place les éléments de l'interface
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }
    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de réglage associés
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de réglage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 100.0);  // Valeurs arbitraires, à adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


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

/// Ici le constructeur se contente de préparer un cadre d'accueil des
/// éléments qui seront ensuite ajoutés lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1020,600);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(100,590);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(920,590);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);

    m_top_box.add_child(m_Button_Save);
    m_Button_Save.set_dim(80,40);
    m_Button_Save.set_bg_color(ROUGE);
    m_Button_Save.set_pos(10,10);

    m_Button_Save.add_child(m_Text_Save);
    m_Text_Save.set_pos(30,15);
    m_Text_Save.set_message("SAVE");
}




/************************************************************************
Type : Méthode
Utilité : charge les sommets et les arettes a partir d'un fichier
Créateur :Léo
Statut : fini
***********************************************************************/
void Graph::chargerFichier(int ordre)
{

    std::string nomFichier;
    if (ordre == 0)
    {
        std::cout << "quel est le nom de votre fichier : ";
        std::cin >> nomFichier;
    }
    else if (ordre == 1)
        nomFichier = "graph1";
    else if (ordre == 2)
        nomFichier = "patate";

    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    nomFichier = "fichier/" + nomFichier;
    std::string fichier;
    int idx,x,y,vert1,vert2;
    double value;
    std::string picture_name;

    /***********************************************************
                CHARGEMENT DES SOMMETS
    ***********************************************************/
    fichier = nomFichier+"_Sommets.txt";
    std::ifstream fsommets (fichier, std::ios::in);
    if(fsommets)
    {
        while(!fsommets.eof())
        {
            fsommets>>idx;
            fsommets>>value;
            fsommets>>x;
            fsommets>>y;
            fsommets>> picture_name;

            add_interfaced_vertex(idx, value, x, y, picture_name);

        }
        fsommets.close();
    }
    else
    {
        std::cout << "impossible de ouvrir le fichier" << fichier << std::endl;
    }

    /***********************************************************
                CHARGEMENT DES ARETTES
    ***********************************************************/
    fichier = nomFichier+"_Arettes.txt"; //modif du nom de l'appel fichier
    std::ifstream farettes (fichier, std::ios::in);
    if(farettes)
    {
        while(!farettes.eof())
        {
            farettes>>idx;
            farettes>>vert1;
            farettes>>vert2;
            farettes>>value;

            add_interfaced_edge(idx, vert1, vert2, value);
        }
        farettes.close();
    }
    else
    {
        std::cout << "impossible de ouvrir le fichier" << fichier << std::endl;
    }
}



/************************************************************************
Type : Méthode
Utilité : sauve les sommets et les arettes dans un fichier
Créateur :Léo
Statut : fini
***********************************************************************/
void Graph::sauverFichier(int ordre)
{

    std::string nomFichier;
    if (ordre == 0)
    {
        std::cout << "quel est le nom de votre fichier : ";
        std::cin >> nomFichier;
    }
    else if (ordre == 1)
        nomFichier = "graph1";
    else if (ordre == 2)
        nomFichier = "patate";

    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    nomFichier = "fichier/" + nomFichier;
    std::string fichier;

    /***********************************************************
                SAUVEGARDE DES SOMMETS
    ***********************************************************/
    fichier = nomFichier+"_Sommets.txt";
    std::ofstream fsommets(fichier, std::ios::out);
    if(fsommets)
    {
        for (auto it = m_vertices.begin(); it!=m_vertices.end(); ++it)
        {
            fsommets << "\n";
            fsommets << it->first << " " ;
            fsommets << it->second.m_value << " ";
            fsommets << it->second.m_interface->m_top_box.get_posx() +2 << " ";
            fsommets << it->second.m_interface->m_top_box.get_posy() +2 << " ";
            fsommets << it->second.m_interface->m_img.get_pic_name();

        }
        fsommets.close();
    }
    else
    {
        std::cout << "impossible de ouvrir le fichier" << std::endl;
    }


    /***********************************************************
                SAUVEGARDE DES ARETTES
    ***********************************************************/
    fichier = nomFichier+"_Arettes.txt";
    std::ofstream farettes(fichier, std::ios::out);
    if(farettes)
    {
        for (auto it = m_edges.begin(); it!=m_edges.end(); ++it)
        {
            farettes << "\n";
            farettes << it->first << " " ;
            farettes << it->second.m_from << " ";
            farettes << it->second.m_to << " ";
            farettes << it->second.m_weight;

        }
        farettes.close();
    }
    else
    {
        std::cout << "impossible de ouvrir le fichier" << std::endl;
    }

}

/// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
    if (!m_interface)
        return;

    for (auto &elt : m_vertices)
        elt.second.pre_update();

    for (auto &elt : m_edges)
        elt.second.pre_update();

    m_interface->m_top_box.update();

    for (auto &elt : m_vertices)
    {
        elt.second.post_update();
        if (elt.second.m_interface->m_box_close.get_value() == true)
            remove_vertex(elt.first);

    }
    for (auto &elt : m_edges)
        elt.second.post_update();

    if (m_interface->m_Button_Save.get_value()==true)
        {std::cout << "\non sauve";
        m_interface->m_Button_Save.set_value(false);
        }



}

/// Aide à l'ajout de sommets interfacés
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Création d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
}

/// Aide à l'ajout d'arcs interfacés
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight)
{
    if ( m_edges.find(idx)!=m_edges.end() )
    {
        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }

    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }

    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei);

    m_edges[idx].m_from = id_vert1;
    m_edges[idx].m_to = id_vert2;

    m_vertices[id_vert1].m_out.push_back(idx);
    m_vertices[id_vert2].m_in.push_back(idx);


}


/// eidx index of edge to remove
void Graph::remove_edge(int eidx)
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
    std::vector<int> &from = m_vertices[supr.m_from].m_out;
    std::vector<int> &to = m_vertices[supr.m_to].m_in;
    from.erase( std::remove(from.begin(), from.end(), eidx ), from.end() );
    to.erase( std::remove( to.begin(), to.end(), eidx ), to.end() );

    m_edges.erase( eidx );

/// Tester la cohérence : nombre d'arc entrants et sortants des sommets 1 et 2
    /*std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    std::cout << m_edges.size() << std::endl;*/

}

void Graph::remove_vertex(int vdx)
{
    ///Reference vers le sommet (vertex) a enlever
    Vertex supr=m_vertices[vdx];
    ///on supprime toutes les aretes liées a ce sommet
    for (int i = supr.m_in.size() -1 ; i >= 0  ; i --)
    {
        remove_edge(supr.m_in[i]);
    }

    for (int i = supr.m_out.size() -1 ; i >= 0  ; i --)
    {
        remove_edge(supr.m_out[i]);
    }
    ///on supprime tous les elements interfacés
    if (m_interface && supr.m_interface)
    {
        m_interface->m_main_box.remove_child( supr.m_interface->m_top_box );
    }
///on supprime le sommet
    m_vertices.erase(vdx);
}


void Graph::add_vertex(std::string image)
{
    int indice = 0;
    for (auto a=m_vertices.begin() ; a != m_vertices.end(); a++)
        indice = a->first;

    add_interfaced_vertex(indice + 1 , 30, 100, 100, image);

}

void Graph::add_edge(int from, int to)
{
    int indice;
    for (auto a=m_edges.begin() ; a != m_edges.end(); a++)
        indice = a->first;
    std::cout << "coucou13\n";
    add_interfaced_edge(indice + 1,from,to,50);
    std::cout << "coucou 14";
}

void Graph::renouvellement_ordre()
{
    m_ordre=0;

    for(auto it=m_vertices.begin(); it!=m_vertices.end(); it++)
    {
        m_ordre++;
    }
    // std::cout << m_ordre << std::endl;
}


/// Tableau d'adjance remlpi à partir des maps de sommets et d'arrêtes, modifiable en cours d'execution du code
void Graph::remplir_tab_adj()
{
    int i=0, j=0;
    /// 1er parcours du map de sommets
    for(auto it=m_vertices.begin(); it!=m_vertices.end(); it++)
    {
        j=0;
        /// alloué le tableau d'adjance
        m_tab_adj.resize(m_ordre);
        /// 2eme parcours du map de sommets
        for(auto im=m_vertices.begin(); im!=m_vertices.end(); im++)
        {
            /// alloué le tableau d'adjance d'indice i
            m_tab_adj[i].resize(m_ordre,0);
            /// si le sommet de partant n'est pas le même que celui d'arrivée
            if(i!=j)
            {
                /// on parcours le tableau d'arêtes
                for (int k=0; k<m_edges.size(); k++)
                {
                    /// si on trouve une arête qui a le même sommet partant que i et le même sommet entrant que j alors notre tableau d'adjance[i][j] = 1
                    if(m_edges[k].m_from == i  && m_edges[k].m_to==j)
                    {
                        m_tab_adj[i][j]=1;
                        k=m_edges.size();
                    }
                    /// sinon tableau d'adjance [i][j]=0
                    else
                    {
                        m_tab_adj[i][j]=0;
                    }
                }
            }
            /// sinon si i = j alors tableau d'adjance = 1 car un sommet est forcément adjacent à lui même
            else
            {
                m_tab_adj[i][j]=1;
            }
            /// affichage du tableau d'adjance en console POUR TESTER !!!!
            std::cout << m_tab_adj[i][j] << " ";
            j++;
        }
        std::cout << std::endl;
        i++;
    }
}
