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
    // if ( supr_edge() == true ){
    m_top_edge.add_child(m_box_Edge_close);
    m_box_Edge_close.set_dim(10,10);
    m_box_Edge_close.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_Edge_close.set_bg_color(JAUNE);
    //    }

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


    ///
//   remplir_tab_adj();
    // std::cout << "nouveau tableau" << std::endl;
    //std::cout << "\n avant le vertex idx = " << idx << " r = " << r << " c = " << c;
    m_vertices[idx] = Vertex(value, vi, r, c);
    //std::cout << "\n dans le vertex idx = " << idx << " r = " << m_vertices[idx].m_rythmeCroissance << "c = "<< m_vertices[idx].m_coeffPondere;

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








