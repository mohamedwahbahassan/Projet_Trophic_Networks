#include "graph.h"
#include <fstream>
#include <iostream>

/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();
    //m_top_box.set_border_color(ROUGE);
    // Le slider de r�glage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 100.0);  // Valeurs arbitraires, � adapter...
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_slider_value.set_border_color_notouch(NOIR);

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

    // la croix de supression
    m_top_box.add_child(m_box_close);
    m_box_close.set_dim(10,10);
    m_box_close.set_pos(116,0);
    m_box_close.set_bg_color(VERT);
}


/// Gestion du Vertex avant l'appel � l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_value vers le slider associ�
    //m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donn�e m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}


/// Gestion du Vertex apr�s l'appel � l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_value locale
    m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
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

    // Une boite pour englober les widgets de r�glage associ�s
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de r�glage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 100.0);  // Valeurs arbitraires, � adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);


    // la croix de supression
    // if ( supr_edge() == true ){
    m_top_edge.add_child(m_box_Edge_close);
    m_box_Edge_close.set_dim(10,10);
    m_box_Edge_close.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_Edge_close.set_bg_color(JAUNE);
    //    }

}


/// Gestion du Edge avant l'appel � l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;


    /// Copier la valeur locale de la donn�e m_weight vers le slider associ�
    m_interface->m_slider_weight.set_value(m_weight);

    m_interface->m_top_edge.modifierTaille(m_weight);
    /// Copier la valeur locale de la donn�e m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
}

/// Gestion du Edge apr�s l'appel � l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de pr�parer un cadre d'accueil des
/// �l�ments qui seront ensuite ajout�s lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{

    m_top_box.set_dim(1020,600);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    /**************************************
            BOITE PRINCIPALE
    **************************************/
    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(920,590);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);

    //m_top_box.add_child( m_imgP );
    //m_imgP.set_pic_name("DSC_0369.jpg");
    //m_imgP.set_gravity_x(grman::GravityX::Right);

    /**************************************
            BOITE A OUTILS
    **************************************/
    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(100,590);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);


    /**************************************
            BOUTON SAVE
    **************************************/
    m_top_box.add_child(m_Button_Save); //bouton
    m_Button_Save.set_dim(80,40);
    m_Button_Save.set_bg_color(ROUGE);
    m_Button_Save.set_pos(9,10);
    m_Button_Save.add_child(m_Text_Save);//text du bouton save

    m_Text_Save.set_pos(30,15);
    m_Text_Save.set_message("SAVE");

    /**************************************
            BOUTON SUPR ARETTE
    **************************************/
    m_top_box.add_child(m_Button_Supr_Arette); //bouton
    m_Button_Supr_Arette.set_dim(80,40);
    m_Button_Supr_Arette.set_bg_color(JAUNE);
    m_Button_Supr_Arette.set_pos(9,70);
    m_Button_Supr_Arette.add_child(m_Text_Supr_arette);//text du bouton save

    m_Text_Supr_arette.set_pos(1,15);
    m_Text_Supr_arette.set_message("SUPR ARETTE");


    /**********************************************
            BOUTON GESTION VITESSE EVOLUTION
    **********************************************/
    m_top_box.add_child(m_Button_Vit_Evolution); //bouton
    m_Button_Vit_Evolution.set_dim(80,80);
    m_Button_Vit_Evolution.set_bg_color(BLANC);
    m_Button_Vit_Evolution.set_pos(9,200);
    m_Button_Vit_Evolution.add_child(m_Text_Vit_Evolution);//text du bouton save

    m_Text_Vit_Evolution.set_pos(3,15);
    m_Text_Vit_Evolution.set_message("EVOLUTION");

    m_Button_Vit_Evolution.add_child(m_Button_Vit_Evolution_plus); //bouton
    m_Button_Vit_Evolution_plus.set_dim(20,20);
    m_Button_Vit_Evolution_plus.set_bg_color(VERT);
    m_Button_Vit_Evolution_plus.set_pos(40,40);
    m_Button_Vit_Evolution_plus.add_child(m_Text_Vit_Evolution_plus);//text du bouton save

    m_Text_Vit_Evolution_plus.set_pos(0,0);
    m_Text_Vit_Evolution_plus.set_message("+");

    m_Button_Vit_Evolution.add_child(m_Button_Vit_Evolution_moins); //bouton
    m_Button_Vit_Evolution_moins.set_dim(20,20);
    m_Button_Vit_Evolution_moins.set_bg_color(ROUGE);
    m_Button_Vit_Evolution_moins.set_pos(10,40);
    m_Button_Vit_Evolution_moins.add_child(m_Text_Vit_Evolution_moins);//text du bouton save

    m_Text_Vit_Evolution_moins.set_pos(0,0);
    m_Text_Vit_Evolution_moins.set_message("-");

}

void Graph::WraperBoutons()
{
    if (m_interface->m_Button_Save.get_value()==true)
    {
        std::cout << "\non sauve";
        m_interface->m_Button_Save.set_value(false);
    }
    if (m_interface->m_Button_Supr_Arette.get_value()==true)
    {
        for (auto &elt : m_edges)
        {
            if (elt.second.m_suprEdge == false)
                elt.second.m_suprEdge = true;
            else
                elt.second.m_suprEdge = false;
            m_interface->m_Button_Supr_Arette.set_value(false);
        }
    }
}


/// La m�thode update � appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
    int indice = -1;
    if (!m_interface)
        return;



    WraperBoutons();

    for (auto &elt : m_vertices)
        elt.second.pre_update();

    for (auto &elt : m_edges)
        elt.second.pre_update();



    m_interface->m_top_box.update(); /// a placer au milieu


    /*   for (auto &elt : m_vertices)
           elt.second.post_update();*/




    for (auto &elt : m_edges)
        elt.second.post_update();


    for (auto elt = m_edges.begin() ; elt != m_edges.end() ; ++elt)
    {
        if (elt->second.m_interface->m_box_Edge_close.get_value() == true)
        {
            indice = elt->first;
        }
    }

    if(indice != -1)
    {
        remove_edge(indice);
        indice = -1;
    }


    for (auto &elt : m_vertices)
    {
        elt.second.post_update();
        if (elt.second.m_interface->m_box_close.get_value() == true)
        {
            indice = elt.first;
        }
    }
    if (indice != -1)
    {
        remove_vertex(indice);
        indice = -1;
    }

}



/************************************************************************
Type : M�thode
Utilit� : charge les sommets et les arettes a partir d'un fichier
Cr�ateur :L�o
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
        nomFichier = "graph2";
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
            std::cout << "\n" << idx << " " <<  value<< " " << x<<" " << y;

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
Type : M�thode
Utilit� : sauve les sommets et les arettes dans un fichier
Cr�ateur :L�o
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



/// Aide � l'ajout de sommets interfac�s
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Cr�ation d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);


    ///
    remplir_tab_adj();
    std::cout << "nouveau tableau" << std::endl;
}

/// Aide � l'ajout d'arcs interfac�s
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
/// r�f�rence vers le Edge � enlever
    Edge &supr=m_edges[eidx];
//std::cout << "Removing edge " << eidx << " " << remed.m_from << "->" << remed.m_to << " " << remed.m_weight << std::endl;

/// Tester la coh�rence : nombre d'arc entrants et sortants des sommets 1 et 2
//std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
//std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
//std::cout << m_edges.size() << std::endl;

/// test : on a bien des �l�ments interfac�s
    if (m_interface && supr.m_interface)
    {
        m_interface->m_main_box.remove_child( supr.m_interface->m_top_edge );
    }
/// Il reste encore � virer l'arc supprim� de la liste des entrants et sortants des 2 sommets to et from !
/// References sur les listes de edges des sommets from et to
    std::vector<int> &from = m_vertices[supr.m_from].m_out;
    std::vector<int> &to = m_vertices[supr.m_to].m_in;
    from.erase( std::remove(from.begin(), from.end(), eidx ), from.end() );
    to.erase( std::remove( to.begin(), to.end(), eidx ), to.end() );
    m_edges.erase( eidx );
/// Tester la coh�rence : nombre d'arc entrants et sortants des sommets 1 et 2
    /*std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    std::cout << m_edges.size() << std::endl;*/

}

void Graph::remove_vertex(int vdx)
{
    ///Reference vers le sommet (vertex) a enlever
    Vertex supr=m_vertices[vdx];
    ///on supprime toutes les aretes li�es a ce sommet
    for (int i = supr.m_in.size() -1 ; i >= 0  ; i --)
    {
        remove_edge(supr.m_in[i]);
    }

    for (int i = supr.m_out.size() -1 ; i >= 0  ; i --)
    {
        remove_edge(supr.m_out[i]);
    }
    ///on supprime tous les elements interfac�s
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
    std::cout << "coucou13\n";
    add_interfaced_edge(indice + 1,from,to,50);
    std::cout << "coucou 14";
}


/// Tableau d'adjance remlpi � partir des maps de sommets et d'arr�tes, modifiable en cours d'execution du code
void Graph::remplir_tab_adj()
{
    int i=0, j=0;

    m_tab_adj.resize(m_vertices.size());
    /// 1er parcours du map de sommets
    for(auto it=m_vertices.begin(); it!=m_vertices.end(); it++)
    {
        j=0;
        /// allou� le tableau d'adjance
        m_tab_adj[i].resize(m_vertices.size(),0);
        /// 2eme parcours du map de sommets
        for(auto im=m_vertices.begin(); im!=m_vertices.end(); im++)
        {
            /// allou� le tableau d'adjance d'indice i

            /// si le sommet de partant n'est pas le m�me que celui d'arriv�e
            if(i!=j)
            {
                /// on parcours le tableau d'ar�tes
                for (int k=0; k<m_edges.size(); k++)
                {
                    /// si on trouve une ar�te qui a le m�me sommet partant que i et le m�me sommet entrant que j alors notre tableau d'adjance[i][j] = 1
                    if(m_edges[k].m_from == it->first  && m_edges[k].m_to==im ->first)
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
            /// sinon si i = j alors tableau d'adjance = 1 car un sommet est forc�ment adjacent � lui m�me
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

std::vector <int> Graph::uneComposanteFortementConnexe(int s)
{
    /// composantes connexes directes partant de s et indirectes arrivant vers s
    std::vector<int> c1;
    std::vector<int> c2;
    /// composante fortement connexe  � retourner
    std::vector <int> c;
    std::vector <int> marques; /// tableau dynamique indiquant si les sommets sont marqu�s ou non
    int x,y; /// num�ros de sommets interm�diaires des composantes connexes
    int ajoute=1; /// bool�en indiquant si une nouvelle composante connexe est ajout�e
    int i;


    c1.resize(m_vertices.size());
    c2.resize(m_vertices.size());
    c.resize(m_vertices.size());
    marques.resize(m_vertices.size());

    for(i=0; i<m_vertices.size(); i++)
    {
        c1[i]=0;
        c2[i] =0;
        c[i]=0;
        marques[i]=0;
    }

    c1[s]=1;
    c2[s]=1;

    while(ajoute)
    {
        ajoute =0;

        for(x=0; x<m_vertices.size(); x++)
        {
            if(!marques[x] && c1[x])
            {
                marques[x]=1;
                for(y=0; y<m_vertices.size(); y++)
                {
                    if(m_tab_adj[x][y]&& !marques[y])
                    {
                        c1[y]=1;
                        ajoute=1;
                    }
                }

            }
        }
    }
    ajoute = 1;
    for(x=0; x<m_vertices.size(); x++)
    {
        marques[x]=0;
    }
    while(ajoute)
    {
        ajoute=0;
        for (x=0; x<m_vertices.size(); x++)
        {

            if(!marques[x] && c2[x])
            {
                marques[x]=1;
                for(y=0; y<m_vertices.size(); y++)
                {
                    if(m_tab_adj[y][x] && !marques[y])
                    {

                        c2[y]=1;
                        ajoute=1;
                    }
                }

            }
        }


    }




    for(x=0; x<m_vertices.size(); x++)
    {
        c[x]= c1[x] & c2[x];
    }

    /* for(x=0; x<m_vertices.size(); x++)
     {
         std::cout << c[x] << std::endl;
     }*/
    return c;

}

void Graph::toutesLesComposantesFortementConnexes()
{
    std::vector<std::vector<int>> tabc; /// tableau dynamique des composantes fortements connexes � retourner
    std::vector<int> marque; /// tableau dynamique indiquant si les sommets sont marqu�s ou non
    int x,y;  /// num�ros de sommets interm�diaires des composantes connexes

    std::cout << "l'ordre est " << m_vertices.size() << std::endl;
    marque.resize(m_vertices.size());
    tabc.resize(m_vertices.size());
    for(x=0; x<m_vertices.size(); x++)
    {
        marque[x]=0;
        tabc[x].resize(m_vertices.size());
        for(y=0; y<m_vertices.size(); y++)
        {
            tabc[x][y]=0;
        }
    }

    for(x=0; x<m_vertices.size(); x++)
    {
        tabc[x]=uneComposanteFortementConnexe(x);
        marque[x]=1;
        for(y=0; y<m_vertices.size(); y++)
        {
            if(tabc[x][y] && !marque[y])
            {
                marque[y]=1;
            }
        }
    }

    m_tab_forte_connexite=tabc;

}

void Graph::affichageTableauForteConnexite()
{
    std::cout << " Tableau de forte connexit�" << std::endl;
    for(int i=0; i<m_vertices.size(); i++)
    {
        for(int j=0; j<m_vertices.size(); j++)
        {
            std::cout << m_tab_forte_connexite[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
