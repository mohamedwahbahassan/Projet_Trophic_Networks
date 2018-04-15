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
    //m_top_box.set_border_color(ROUGE);
    // m_top_box.set_border_color(VERT);
    // Le slider de réglage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 100.0);  // Valeurs arbitraires, à adapter...
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


    // la croix de supression
    //if (m_autor_supr == true )

    m_top_edge.add_child(m_box_Edge_close);
    m_box_Edge_close.set_dim(5,5);
    m_box_Edge_close.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_Edge_close.set_bg_color(ROUGE);


}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de préparer un cadre d'accueil des
/// éléments qui seront ensuite ajoutés lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    int ordre = y;
    y = 0;

    if (ordre%10 == 1)
        m_fond = "banquise.jpg";
    if (ordre%10 == 2)
        m_fond = "desert.jpg";
    if(ordre%10 == 3)
        m_fond = "savane.jpg";

    m_top_box.set_dim(1020,600);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

        m_top_box.add_child( m_imgP );
    m_imgP.set_pic_name(m_fond);
    m_imgP.set_gravity_x(grman::GravityX::Right);


    /**************************************
            BOITE PRINCIPALE
    **************************************/
    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(920,590);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

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

    m_Text_Save.set_pos(30,5);
    m_Text_Save.set_message("SAVE");

    m_Button_Save.add_child(m_Button_Restaurer); //bouton
    m_Button_Restaurer.set_dim(70,15);
    m_Button_Restaurer.set_bg_color(BLANC);
    m_Button_Restaurer.set_pos(3,20);
    m_Button_Restaurer.add_child(m_Text_Restaurer);//text du bouton save

    m_Text_Restaurer.set_pos(0,0);
    m_Text_Restaurer.set_message("RESTAURER");

    /**************************************
            BOUTON K CONNEXITE
    **************************************/
    m_top_box.add_child(m_Button_K_Connexite); //bouton voir graph pop
    m_Button_K_Connexite.set_dim(80,40);
    m_Button_K_Connexite.set_bg_color(ROUGE);
    m_Button_K_Connexite.set_pos(9,200);
    m_Button_K_Connexite.add_child(m_Text_K_Connexite);

    m_Text_K_Connexite.set_pos(7,15);
    m_Text_K_Connexite.set_message("K-Connexe");

    /**************************************
            BOUTON VOIR GRAPH POP
    **************************************/
    m_top_box.add_child(m_Button_Graph_Pop); //bouton voir graph pop
    m_Button_Graph_Pop.set_dim(80,40);
    m_Button_Graph_Pop.set_bg_color(ROUGE);
    m_Button_Graph_Pop.set_pos(9,250);
    m_Button_Graph_Pop.add_child(m_Text_Graph_Pop);

    m_Text_Graph_Pop.set_pos(7,15);
    m_Text_Graph_Pop.set_message("Graph Pop");

    /**************************************
            BOUTON QUIT
    **************************************/
    m_top_box.add_child(m_Button_Quit); //bouton
    m_Button_Quit.set_dim(80,40);
    m_Button_Quit.set_bg_color(BLEU);
    m_Button_Quit.set_pos(9,500);
    m_Button_Quit.add_child(m_Text_Quit);//text du bouton save

    m_Text_Quit.set_pos(20,15);
    m_Text_Quit.set_message("QUIT");

    /**********************************************
            BOUTON MONTRER COMPOSANTES CONNEXE
    **********************************************/
    m_top_box.add_child(m_Button_Montrer_Composantes_Connexe); //bouton composantes connexe
    m_Button_Montrer_Composantes_Connexe.set_dim(35,40);
    m_Button_Montrer_Composantes_Connexe.set_bg_color(BLEU);
    m_Button_Montrer_Composantes_Connexe.set_pos(9,300);
    m_Button_Montrer_Composantes_Connexe.add_child(m_Text_Montrer_Composantes_Connexe);//texte composantes connexe

    m_Text_Montrer_Composantes_Connexe.set_pos(3,15);
    m_Text_Montrer_Composantes_Connexe.set_message("MCC");

    /**********************************************
            BOUTON MONTRER GRAPH REDUIT
    **********************************************/
    m_top_box.add_child(m_Button_Graph_Reduit); //bouton composantes connexe
    m_Button_Graph_Reduit.set_dim(35,40);
    m_Button_Graph_Reduit.set_bg_color(BLEU);
    m_Button_Graph_Reduit.set_pos(55,300);
    m_Button_Graph_Reduit.add_child(m_Text_Graph_Reduit);//texte composantes connexe

    m_Text_Graph_Reduit.set_pos(10,15);
    m_Text_Graph_Reduit.set_message("GR");

    /**************************************
            BOUTON AJOUT VERTEX
    **************************************/
    m_top_box.add_child(m_Button_Ajout_Vertex); //bouton
    m_Button_Ajout_Vertex.set_dim(80,40);
    m_Button_Ajout_Vertex.set_bg_color(BLEU);
    m_Button_Ajout_Vertex.set_pos(9,400);
    m_Button_Ajout_Vertex.add_child(m_Text_Ajout_Vertex);//text du bouton save

    m_Text_Ajout_Vertex.set_pos(0,15);
    m_Text_Ajout_Vertex.set_message(" SOMMET +");

    /**************************************
            BOUTON AJOUT EDGE
    **************************************/
    m_top_box.add_child(m_Button_Ajout_Edge); //bouton
    m_Button_Ajout_Edge.set_dim(80,40);
    m_Button_Ajout_Edge.set_bg_color(ROSE);
    m_Button_Ajout_Edge.set_pos(9,350);
    m_Button_Ajout_Edge.add_child(m_Text_Ajout_Edge);//text du bouton save

    m_Text_Ajout_Edge.set_pos(0,15);
    m_Text_Ajout_Edge.set_message(" ARETE +");

    /**************************************
            BOUTON SUPR ARETTE
    **************************************/
    /*
    m_top_box.add_child(m_Button_Supr_Arette); //bouton
    m_Button_Supr_Arette.set_dim(80,40);
    m_Button_Supr_Arette.set_bg_color(JAUNE);
    m_Button_Supr_Arette.set_pos(9,65);
    m_Button_Supr_Arette.add_child(m_Text_Supr_arette);//text du bouton save

    m_Text_Supr_arette.set_pos(1,15);
    m_Text_Supr_arette.set_message("SUPR ARETE");
    */

    /**********************************************
            BOUTON GESTION VITESSE EVOLUTION
    **********************************************/
    m_top_box.add_child(m_Button_Vit_Evolution); //bouton
    m_Button_Vit_Evolution.set_dim(80,80);
    m_Button_Vit_Evolution.set_bg_color(BLANC);
    m_Button_Vit_Evolution.set_pos(9,100);
    m_Button_Vit_Evolution.add_child(m_Text_Vit_Evolution);//text du bouton save

    m_Text_Vit_Evolution.set_pos(3,15);
    m_Text_Vit_Evolution.set_message("EVOLUTION");

    m_Button_Vit_Evolution.add_child(m_Button_Vit_Evolution_plus); //bouton
    m_Button_Vit_Evolution_plus.set_dim(20,20);
    m_Button_Vit_Evolution_plus.set_bg_color(VERT);
    m_Button_Vit_Evolution_plus.set_pos(40,40);
    m_Button_Vit_Evolution_plus.add_child(m_Text_Vit_Evolution_plus);//text du bouton save

    m_Text_Vit_Evolution_plus.set_pos(5,5);
    m_Text_Vit_Evolution_plus.set_message("+");

    m_Button_Vit_Evolution.add_child(m_Button_Vit_Evolution_moins); //bouton
    m_Button_Vit_Evolution_moins.set_dim(20,20);
    m_Button_Vit_Evolution_moins.set_bg_color(ROUGE);
    m_Button_Vit_Evolution_moins.set_pos(10,40);
    m_Button_Vit_Evolution_moins.add_child(m_Text_Vit_Evolution_moins);//text du bouton save

    m_Text_Vit_Evolution_moins.set_pos(5,5);
    m_Text_Vit_Evolution_moins.set_message("-");
}

void Graph::WraperBoutons()
{
    if (m_interface->m_Button_Vit_Evolution.get_value()==true) /// play/ pause evolution
    {
        m_playEvolution = true;
        m_interface->m_Button_Vit_Evolution.set_value(false);
    }
    if (m_interface->m_Button_K_Connexite.get_value()==true) /// play/ pause evolution
    {
        m_interface->m_Button_K_Connexite.set_value(false);
                    ordredebase();
            getAllCombin(get_serie());
            affichage_tab_combin();
            initialise_tab_combine();
    }
    if (m_interface->m_Button_Graph_Pop.get_value()==true) /// play/ pause evolution
    {
        AffPop();
        m_interface->m_Button_Graph_Pop.set_value(false);
    }

    if (m_interface->m_Button_Vit_Evolution_moins.get_value()==true) /// ralentissement evolution
    {
        std::cout << "\nmoins vite";
        m_vitEvolutionMoins = true;
        m_interface->m_Button_Vit_Evolution_moins.set_value(false);
    }

    if (m_interface->m_Button_Vit_Evolution_plus.get_value()==true) /// acceleration evolution
    {
        std::cout << "\nplus vite";
        m_vitEvolutionPlus = true;
        m_interface->m_Button_Vit_Evolution_plus.set_value(false);
    }

    if (m_interface->m_Button_Save.get_value()==true) /// sauvegarde
    {
        std::cout << "\non sauve";
        m_sauver = true;
        m_interface->m_Button_Save.set_value(false);
    }
    if (m_interface->m_Button_Graph_Reduit.get_value()==true) /// sauvegarde
    {
        std::cout << "\ngraph_réduit";
        m_graphReduit = true;
        m_interface->m_Button_Graph_Reduit.set_value(false);
    }

    if (m_interface->m_Button_Restaurer.get_value()==true) ///restauration
    {
        std::cout << "\non sauve";
        m_restaurer_graph = true;
        m_interface->m_Button_Restaurer.set_value(false);
    }

    if (m_interface->m_Button_Quit.get_value()==true) /// quiter
    {
        std::cout << "\non quite";
        m_quiter = true;
        m_interface->m_Button_Quit.set_value(false);
    }

    if (m_interface->m_Button_Montrer_Composantes_Connexe.get_value()==true) /// montrer composantes connexe
    {
        std::cout << "\non montre les composantes fortement connexe";
        m_montrerComposantesConnexe = true;
        m_interface->m_Button_Montrer_Composantes_Connexe.set_value(false);
    }

    if (m_interface->m_Button_Ajout_Vertex.get_value()==true) /// ajouter sommet
    {
        std::cout << "\nvertex +";
        m_AjoutVertex= true;
        m_interface->m_Button_Ajout_Vertex.set_value(false);
    }
    if (m_interface->m_Button_Ajout_Edge.get_value()==true) /// ajouter arete
    {
        std::cout << "\nedge +";
        m_AjoutEdge= true;
        m_interface->m_Button_Ajout_Edge.set_value(false);
    }

    if (m_interface->m_Button_Supr_Arette.get_value()==true) ///supprimer arete (pas tres tres bon)
    {
        m_edges[7].m_autorisation_supr_edge = true;
        m_interface->m_Button_Supr_Arette.set_value(false);
    }

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
    //   std::cout << "salut 2 ";

    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    // std::cout << "coucou2";
    m_ordre ++;

    ///
//   remplir_tab_adj();
    // std::cout << "nouveau tableau" << std::endl;
    //std::cout << "\n avant le vertex idx = " << idx << " r = " << r << " c = " << c;
    m_vertices[idx] = Vertex(value, vi, r, c);
    //std::cout << "\n dans le vertex idx = " << idx << " r = " << m_vertices[idx].m_rythmeCroissance << "c = "<< m_vertices[idx].m_coeffPondere;
    // std::cout << "coucou2";
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

void Graph::affichageForteConnexiteInterface(bool actif)
{
    int i,j;

    if (actif == true)
    {
        for(auto it=m_vertices.begin(); it!=m_vertices.end(); it++)
        {
            it -> second.set_deja_fortement_connexe(false);
        }

        for(int i=0; i<m_ordre-1; i++)
        {
            for(j=i+1; j<m_ordre; j++)
            {
                if(m_tab_forte_connexite[i]==m_tab_forte_connexite[j])
                {
                    // std::cout << "\nforte connexite entre " << i << " et " << j;
                    if (m_vertices[i].get_deja_fortement_connexe() == true && m_vertices[j].get_deja_fortement_connexe() == true)
                    {
//std::cout << "\n1 : forte connexite entre " << i << " et " << j;
                    }
                    else if (m_vertices[i].get_deja_fortement_connexe() == true)
                    {
                        //std::cout << "\n2 : forte connexite entre " << i << " et " << j;
                        m_vertices[j].set_borderColor(m_vertices[i].get_borderColor());
                        m_vertices[j].set_deja_fortement_connexe(true);
                    }
                    else if (m_vertices[j].get_deja_fortement_connexe() == true)
                    {
                        //std::cout << "\n3 : forte connexite entre " << i << " et " << j;
                        m_vertices[i].set_borderColor(m_vertices[j].get_borderColor());
                        m_vertices[i].set_deja_fortement_connexe(true);
                    }
                    else
                    {
                        //   std::cout << "\n4 : forte connexite entre " << i << " et " << j;
                        m_vertices[i].set_borderColor(COULEURALEATOIRE);
                        m_vertices[i].set_deja_fortement_connexe(true);

                        m_vertices[j].set_borderColor(m_vertices[i].get_borderColor());
                        m_vertices[j].set_deja_fortement_connexe(true);
                    }
                }
            }
        }

        for(auto it=m_vertices.begin(); it!=m_vertices.end(); it++)
        {
            if (it->second.get_deja_fortement_connexe() == true)
                it->second.m_interface->m_top_box.set_border_color(it->second.get_borderColor());
            else
                it->second.m_interface->m_top_box.set_border_color(COULEURALEATOIRE);
        }
    }
    else
    {
        for(auto it=m_vertices.begin(); it!=m_vertices.end(); it++)
        {

            it->second.m_interface->m_top_box.set_border_color(GRISCLAIR);
        }
    }
}


/************************************************************************
Type :
Utilité : création du graph réduit
Return : none
Créateur :Léo
Statut : fini
***********************************************************************/
void Graph::GraphReduit()
{
    sauverFichier(get_current_graph(),2); // on sauve le fichier pour garder en memoire quand on lache le bouton
    vider_graph(); // on clear le graph
    chargerFichier(get_current_graph(),2); // on ouvre a nouveau le fichier
    int idx,from,to,weight; // variables utilisées pour l'algo

    for(int i=0; i<m_ordre-1; i++)
    {
        for(int j=i+1; j<m_ordre; j++)
        {
            if(m_tab_forte_connexite[i]==m_tab_forte_connexite[j])
            {
                for (auto it=m_edges.begin(); it!=m_edges.end() ; it++) // cas de forte connexité entre 2 sommets
                {
                    if(it->second.get_from() == j && it->second.get_to() == i) // si un des 2 sommets a déja été visité
                    {
                        remove_edge(it->first,1);
                     //   std::cout << "\ncas n1 : " << it->first;
                    }
                    else if (it->second.get_from() == i && it->second.get_to() == j) // si l'autre l'a été
                    {
                        remove_edge(it->first,1);
                   //     std::cout << "\ncas n2 " << it->first;
                    }
                    else if (  it->second.get_from() == j ) // si aucun des 2 n'a ete visité on en supprime un, et on mets ses marqueurs a vrai
                    {
                      ///  std::cout << "\ncas n3 "  << it->first;
                        idx = it->first;
                        from = it->second.get_from();
                        to = it->second.get_to();
                        weight = it->second.get_weight();
                        remove_edge(idx,1);
                        add_interfaced_edge(idx,i,to,weight);
                    }
                    else if ( it->second.get_to() == j)
                    {
                    //    std::cout << "\ncas n4 " << it->first;
                        idx = it->first;
                        from = it->second.get_from();
                        to = it->second.get_to();
                        weight = it->second.get_weight();
                        remove_edge(idx,1);
                        add_interfaced_edge(idx,from,i,weight);
                    }

                   // std::cout << "\nedge : " << it->first << " from : " << it->second.get_from() << " to : " << it->second.get_to() ;
                }
                                for (auto it=m_edges.begin(); it!=m_edges.end() ; it++) // on reconnemce au cas ou certains sommets on été sauté
                {
                    if(it->second.get_from() == j && it->second.get_to() == i)
                    {
                        remove_edge(it->first,1);
                       // std::cout << "\ncas n1 : " << it->first;
                    }
                    else if (it->second.get_from() == i && it->second.get_to() == j)
                    {
                        remove_edge(it->first,1);
                     //   std::cout << "\ncas n2 " << it->first;
                    }
                    else if (  it->second.get_from() == j )
                    {
                        //std::cout << "\ncas n3 "  << it->first;
                        idx = it->first;
                        from = it->second.get_from();
                        to = it->second.get_to();
                        weight = it->second.get_weight();
                        remove_edge(idx,1);
                        add_interfaced_edge(idx,i,to,weight);
                    }
                    else if ( it->second.get_to() == j)
                    {
                       // std::cout << "\ncas n4 " << it->first;
                        idx = it->first;
                        from = it->second.get_from();
                        to = it->second.get_to();
                        weight = it->second.get_weight();
                        remove_edge(idx,1);
                        add_interfaced_edge(idx,from,i,weight);
                    }

                 //   std::cout << "\nedge : " << it->first << " from : " << it->second.get_from() << " to : " << it->second.get_to() ;
                }

               // std::cout << "\nremoving vertex n" <<j;
                remove_vertex(j,1); // si un sommets est fortement connexe, on le supprime
            }
        }
    }
}










