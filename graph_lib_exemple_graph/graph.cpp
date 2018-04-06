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
    m_top_box.set_dim(795,600);

    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);


    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,590);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(715,590);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);
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
        nomFichier = "banquise";
    else if (ordre == 2)
        nomFichier = "desert";
    else if(ordre == 3)
        nomFichier = "savane";

    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    nomFichier = "fichier/" + nomFichier;
    std::string fichier;
    int idx,x,y,vert1,vert2;
    double value;
    ///add by jojo
    float r,c;
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

            std::cout << y << "\n";

            fsommets>> r; /// Rythme Croissance
            fsommets>> c; ///Coeff Pondéré

                      ///add by jojo
            std::cout << r << "\n";
            std::cout << c << "\n";

            fsommets>> picture_name;

            add_interfaced_vertex(idx, value, x, y, picture_name,idx,r,c); ///Changed by jojo
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
    fichier = nomFichier+"_Aretes.txt"; //modif du nom de l'appel fichier
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
        nomFichier = "banquise";
    else if (ordre == 2)
        nomFichier = "desert";

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

            std::cout << "\n rythme " << it->second.m_rythmeCroissance;

            std::cout << "\n coeff " << it->second.m_coeffPondere;

            fsommets << it->second.m_rythmeCroissance << " ";
            fsommets << it->second.m_coeffPondere << " ";

            fsommets << it->second.m_interface->m_img.get_pic_name();
        }
        fsommets.close();
    }
    else
    {
        std::cout << "impossible de ouvrir le fichier" << std::endl;
    }


    /***********************************************************
                SAUVEGARDE DES ARETES
    ***********************************************************/
    fichier = nomFichier+"_Aretes.txt";
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
        elt.second.post_update();

    for (auto &elt : m_edges)
        elt.second.post_update();

}

/// Aide à l'ajout de sommets interfacés
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx, float r, float c)
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
    std::cout << "\n avant le vertex idx = " << idx << " r = " << r << " c = " << c;
    m_vertices[idx] = Vertex(value, vi, r, c);
    std::cout << "\n dans le vertex idx = " << idx << " r = " << m_vertices[idx].m_rythmeCroissance << "c = "<< m_vertices[idx].m_coeffPondere;
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

/*************************************************
*********** SIMULATION DE POPULATION *************
**************************************************/

/// Fonction: permet de calculer la population en temps réel en fonction de la population de départ

void Graph::CalculPop()
{

    for (auto &e : m_vertices)
    {
        e.second.m_capacite=0;
        e.second.m_quantiteConsomme=0;

        std::cout << "SOMMET"<< e.first << std::endl;
        std::cout << "valeur initiale" << e.second.m_value<< std::endl;

///Calcul de la capacité de portage de l'environnement

        for(unsigned int i=0; i<e.second.m_in.size(); i++)
        {
           // std::cout << "poids des arcs entrant" << std::endl;
           // std::cout << (m_edges[e.second.m_in[i]].m_weight)/100 << std::endl;
           // std::cout << "valeur des sommet desquelles partent l'arc:" << std::endl;
           // std::cout << m_vertices[m_edges[e.second.m_in[i]].m_from].m_value << std::endl;

           double capaciteinit= e.second.m_capacite ;

            e.second.m_capacite = e.second.m_capacite

            /// ex: K lapin = Coeff(herbe->lapin)* N herbe :  Capacité = Poids de l'arc entrante * valeur du sommet 1 de l'arc entrante
            + (m_edges[e.second.m_in[i]].m_weight)/10 * m_vertices[m_edges[e.second.m_in[i]].m_from].m_value; ///m_edges[e.second.m_in[i]].m_from = indice du sommet 1 de l'arc entrante

            std::cout << "capacite de portage = " << capaciteinit << "+" << (m_edges[e.second.m_in[i]].m_weight)/10 << "*"<< m_vertices[m_edges[e.second.m_in[i]].m_from].m_value<< "=" << e.second.m_capacite<< std::endl;

        }


///Calcul de la quantité consommée

       for(unsigned int i=0; i<e.second.m_out.size(); i++)

        {
            /*std::cout << "Poids des arcs sortant" << std::endl;
            std::cout << (m_edges[e.second.m_out[i]].m_weight)/100 << std::endl;
            std::cout << "valeur des sommet vers lesquelles poitent l'arc:" << std::endl;
            std::cout << m_vertices[m_edges[e.second.m_out[i]].m_to].m_value << std::endl;*/

            double quantiteinit =e.second.m_quantiteConsomme;
            e.second.m_quantiteConsomme = e.second.m_quantiteConsomme

            /// ex: K2 herbe= Coeff (herbe->lapin)* N lapin : Quantité consommée = Poids de l'arc sortante * valeur du sommet 2 de l'arc sortante
             + (m_edges[e.second.m_out[i]].m_weight)/10 * m_vertices[m_edges[e.second.m_out[i]].m_to].m_value;

            std::cout << "quantite consomme=" << quantiteinit << "+" << (m_edges[e.second.m_out[i]].m_weight)/10 << "*" << m_vertices[m_edges[e.second.m_out[i]].m_to].m_value << "=" << e.second.m_quantiteConsomme << std::endl;
        }

///Calcul de l'évolution de la population à partir d'un certain temps t

        double valueinit= e.second.m_value;

        if(e.second.m_capacite!=0)
        {

         e.second.m_value = e.second.m_value
         + e.second.m_rythmeCroissance * e.second.m_value * (1-(e.second.m_value/e.second.m_capacite)) ///R * N (1 - N/K)
         - e.second.m_quantiteConsomme * e.second.m_coeffPondere; ///R2*CoeffPondere

        }
        else
        {
            ///Si la capacité de portage de l'environnement est égale à 0, l'espèce meurt

            std::cout << "division par 0, l'espèce meurt de toute maniere" << std::endl;
            //e.second.m_value = 0;
        }

        ///Si on obtient une population négative, value=0

        if(e.second.m_value < 0)
        {
            e.second.m_value=0;
        }
        std::cout << "\ncalcul nouvelle valeur:" << valueinit << "+" << e.second.m_rythmeCroissance << "*" << valueinit << "*"
        << "(1-" << valueinit << "/" << e.second.m_capacite<<")" <<"-" << e.second.m_quantiteConsomme << "*" << e.second.m_coeffPondere<< "=" << e.second.m_value << std::endl;
        std::cout << "1: capacite: " << e.second.m_capacite << std::endl;
        std::cout << "2: quantite consomme: " << e.second.m_quantiteConsomme<< std::endl;
        std::cout << "3:nouvelle valeur:" <<e.second.m_value << std::endl;
        std::cout << std::endl << std::endl;

    }

}




