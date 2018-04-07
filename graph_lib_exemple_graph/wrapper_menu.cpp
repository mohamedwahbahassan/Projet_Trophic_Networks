#include "graph.h"
#include "utilitaire.h"
#include <fstream>
#include <iostream>



bool Graph::MenuPrincipal()
{
    BITMAP* buffer = create_bitmap(SCREEN_W,SCREEN_H);
    BITMAP* images[NB_IMAGES_MENU];
    BITMAP* menu = charger_image("pics/menu/ecran_d'accueil.bmp");
    BITMAP* menu_coli = charger_image("pics/menu/menu_carte_coli.bmp");
    chargement_images_menu(images);


    bool fin = false;

    blit(menu,buffer,0,0,0,0,SCREEN_W,SCREEN_H);

    while(!fin)
    {
        blit(menu,buffer,0,0,0,0,SCREEN_W,SCREEN_H);

        if (getpixel(menu_coli,mouse_x,mouse_y) == DESERT)
            blit(images[0],buffer,0,0,0,0,SCREEN_W,SCREEN_H);
        if (getpixel(menu_coli,mouse_x,mouse_y) == SAVANE)
            blit(images[1],buffer,0,0,0,0,SCREEN_W,SCREEN_H);
        if (getpixel(menu_coli,mouse_x,mouse_y) == BANQUISE)
            blit(images[2],buffer,0,0,0,0,SCREEN_W,SCREEN_H);
        if (getpixel(menu_coli,mouse_x,mouse_y) == QUIT)
            blit(images[3],buffer,0,0,0,0,SCREEN_W,SCREEN_H);

        if (mouse_b&1)
        {
            if (getpixel(menu_coli,mouse_x,mouse_y) == DESERT)
            {
                chargerFichier(2,false);
                set_current_graph(2);
                fin = true;
            }
            if (getpixel(menu_coli,mouse_x,mouse_y) == SAVANE)
            {
                chargerFichier(3,false);
                set_current_graph(3);
                fin = true;
            }
            if (getpixel(menu_coli,mouse_x,mouse_y) == BANQUISE)
            {
                chargerFichier(1,false);
                set_current_graph(1);
                fin = true;
            }
            if (getpixel(menu_coli,mouse_x,mouse_y) == QUIT)
                return true;



        }


        blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
        //fin = true;
    }
    return false;

}



bool Graph::boutons()
{
    if (get_quiter() == true) /// on a appuyé suir le bouton quiter
    {
        // std::cout << "\n quiter main";
        vider_graph();
        RAZ_quiter();
        return MenuPrincipal();//cas ou l'utilisateur veut quitter
    }

    if (get_montrerComposantesFortementConnexe() == true) /// on a appuyé sur le bouton pour montrer les composantes fortement connexe
    {
    remplir_tab_adj();
    toutesLesComposantesFortementConnexes();
    affichageForteConnexiteInterface();
    RAZ_montrerComposantesFortementConnexe();
    }

    if (get_restaurer_graph() == true) /// on a appuyé sur le bouton restaurer
    {
        std::cout << "\n restaurer main";
        vider_graph();
        chargerFichier(get_current_graph(),true);
        RAZ_restaurer_graph();
    }
    if (get_sauver() == true) /// on a appuyé sur le bouton sauvegarder
    {
        std::cout << "\n sauver main";
        sauverFichier(get_current_graph(),false);
        vider_graph();
        chargerFichier(get_current_graph(),false);
        RAZ_sauver();
    }

    return false;
}


void Graph::evolution(bool* pause, int*rest_evolution, int* t1)
{
/// SIMULATION DE L'EVOLUTION DES POPULATIONS

    if (m_AjoutVertex)
    {
        menu_ajout_vertex();
        m_AjoutVertex = false;
    }
    if (m_AjoutEdge)
    {
        menu_ajout_edge();
        m_AjoutEdge = false;
    }

    if(m_playEvolution && *pause == true)
    {
        *pause = false;
        m_playEvolution = false;
        std::cout << "\npause";
    }
    else if (m_playEvolution)
    {
        *pause = true;
        m_playEvolution = false;
        std::cout << "\nplay";
    }
/// accelerer/ralentir la vitesse d'évolution
    if(m_vitEvolutionMoins == true) ///accélere
    {
        std::cout << " + ";
        *rest_evolution = *rest_evolution + 50;
        m_vitEvolutionMoins = false;
    }
    if(m_vitEvolutionPlus == true) ///ralentir
    {
        std::cout << " - ";
        *rest_evolution = *rest_evolution - 50;
        m_vitEvolutionPlus = false;
    }

    if (*t1 + *rest_evolution <= clock())
    {

        if (*pause == true)
            CalculPop();
        *t1 = clock();

    }
}

void Graph::menu_ajout_edge()
{
    int from,to;
    bool fin = false;
    bool bfrom = false,bto = false;
    int idx = m_edges.end()->first +1;
    std::cout << std::endl << std::endl << "Vous voulez ajouter une arete. Saisisez 2 fois la meme valeur pour sortir";

    while (fin == false)
    {
        std::cout << std::endl << " Veuillez saisir l'indice du sommet de depart : ";
        std::cin >> from;
        std::cout << std::endl << "Veuillez maintenant saisir l'indice du sommet d'arrive : ";
        std::cin >> to;
        if (from == to)
        {
            fin = true;
        }
        else
        {
            for (auto&it : m_vertices)
            {
                if (it.first == from)
                    bfrom = true;
            }
            for (auto&it : m_vertices)
            {
                if (it.first == to)
                    bto = true;
            }
            if (bto == true && bfrom == true)
                fin = true;
            else
            {
                bto = false;
                bfrom = false;
                std::cout << std::endl << "un ou plusieurs des sommets que vous avez saisis n'existent pas";
            }
        }
    }

    if (bto == true && bfrom == true)
        add_interfaced_edge(idx,from,to,0);

}



void Graph::menu_ajout_vertex()
{
    /*
    std::string chaine = "test";
    int test = 12;
    chaine = chaine + std::to_string(test);
    std::cout<< "\n" << chaine;
    */
    //std::cout << "\najout de vertex";


    bool fin = false;
    int color;
    int idx = m_vertices.end()->first +1;
    BITMAP* menu = charger_image("pics/menu/choix_de_vertex.bmp");
    BITMAP* menu_coli = charger_image("pics/menu/choix_de_vertex_coli.bmp");

    blit(menu,screen,0,0,0,0,SCREEN_W,SCREEN_H);

    while (!fin)
    {
        //std::cout << "\n" << getpixel(menu_coli,mouse_x,mouse_y);
        if (mouse_b&1)
        {
            color = getpixel(menu_coli,mouse_x,mouse_y);

            ///DESERT
            if (color == makecol(0,0,0))
                add_interfaced_vertex(idx,50,0,0,"desert/adax.jpg",0,1,1);
            if (color == makecol(127,127,127))
                add_interfaced_vertex(idx,50,0,0,"desert/dromadaire.jpg",0,1,1);
            if (color == makecol(136,0,21))
                add_interfaced_vertex(idx,50,0,0,"desert/fourmi.jpg",0,1,1);
            if (color == makecol(237,28,36))
                add_interfaced_vertex(idx,50,0,0,"desert/gazelle.jpg",0,1,1);
            if (color == makecol(255,242,0))
                add_interfaced_vertex(idx,50,0,0,"desert/guepard.jpg",0,1,1);
            if (color == makecol(34,177,76))
                add_interfaced_vertex(idx,50,0,0,"desert/scorpion.jpg",0,1,1);
            if (color == makecol(0,162,232))
                add_interfaced_vertex(idx,50,0,0,"desert/vegetaux.jpg",0,1,1);

            ///SAVANE
            if (color == makecol(63,72,204))
                add_interfaced_vertex(idx,50,0,0,"savane/buffle.jpg",0,1,1);
            if (color == makecol(195,195,195))
                add_interfaced_vertex(idx,50,0,0,"savane/gnou.jpg",0,1,1);
            if (color == makecol(239,228,176))
                add_interfaced_vertex(idx,50,0,0,"savane/lion.jpg",0,1,1);
            if (color == makecol(112,146,190))
                add_interfaced_vertex(idx,50,0,0,"savane/vegetaux.jpg",0,1,1);
            if (color == makecol(163,73,164))
                add_interfaced_vertex(idx,50,0,0,"savane/elephant.jpg",0,1,1);
            if (color == makecol(185,122,87))
                add_interfaced_vertex(idx,50,0,0,"savane/hyene.jpg",0,1,1);
            if (color == makecol(181,230,29))
                add_interfaced_vertex(idx,50,0,0,"savane/phacochere.jpg",0,1,1);
            if (color == makecol(255,174,201))
                add_interfaced_vertex(idx,50,0,0,"savane/fourmi.jpg",0,1,1);
            if (color == makecol(255,201,14))
                add_interfaced_vertex(idx,50,0,0,"savane/leopard.jpg",0,1,1);
            if (color == makecol(153,217,234))
                add_interfaced_vertex(idx,50,0,0,"savane/zebre.jpg",0,1,1);


            ///BANQUISE
            if (color == makecol(120,149,155))
                add_interfaced_vertex(idx,50,0,0,"banquise/krill.jpg",0,1,1);
            if (color == makecol(100,180,228))
                add_interfaced_vertex(idx,50,0,0,"banquise/morse.jpg",0,1,1);
            if (color == makecol(33,46,77))
                add_interfaced_vertex(idx,50,0,0,"banquise/phoque.jpg",0,1,1);
            if (color == makecol(204,74,0))
                add_interfaced_vertex(idx,50,0,0,"banquise/poisson.jpg",0,1,1);
            if (color == makecol(238,177,69))
                add_interfaced_vertex(idx,50,0,0,"banquise/macareux.jpg",0,1,1);
            if (color == makecol(19,45,77))
                add_interfaced_vertex(idx,50,0,0,"banquise/ours.jpg",0,1,1);
            if (color == makecol(223,230,230))
                add_interfaced_vertex(idx,50,0,0,"banquise/pingouin.jpg",0,1,1);
            if (color == makecol(200,191,231))
                add_interfaced_vertex(idx,50,0,0,"banquise/renard.jpg",0,1,1);

            while(mouse_b&1) {}
            fin = true;
        }
    }
}


