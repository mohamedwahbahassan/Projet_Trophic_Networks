#include "graph.h"
#include "utilitaire.h"
#include <fstream>
#include <iostream>


/************************************************************************
Type : M�thode
Utilit� : menu de choix du graph
Return : bool�en de sortie du programme ou non
Cr�ateur :L�o
Statut : fini
***********************************************************************/
bool Graph::MenuPrincipal()
{
    BITMAP* buffer = create_bitmap(SCREEN_W,SCREEN_H); // buffer de fond
    BITMAP* images[NB_IMAGES_MENU];//tableau d'image pour l'animation menu
    BITMAP* menu = charger_image("pics/menu/ecran_d'accueil.bmp");//image de menu de base
    BITMAP* menu_coli = charger_image("pics/menu/menu_carte_coli.bmp");//carte de colision
    chargement_images_menu(images);//chargement des images

    bool fin = false; //bool�en

    blit(menu,buffer,0,0,0,0,SCREEN_W,SCREEN_H);

    while(!fin) // tant que on ne clique pas su le bouton "quit"
    {
        blit(menu,buffer,0,0,0,0,SCREEN_W,SCREEN_H);

        if (getpixel(menu_coli,mouse_x,mouse_y) == DESERT) // en fonction de la position de la souris, l'image du menu change
            blit(images[0],buffer,0,0,0,0,SCREEN_W,SCREEN_H);
        if (getpixel(menu_coli,mouse_x,mouse_y) == SAVANE)
            blit(images[1],buffer,0,0,0,0,SCREEN_W,SCREEN_H);
        if (getpixel(menu_coli,mouse_x,mouse_y) == BANQUISE)
            blit(images[2],buffer,0,0,0,0,SCREEN_W,SCREEN_H);
        if (getpixel(menu_coli,mouse_x,mouse_y) == QUIT)
            blit(images[3],buffer,0,0,0,0,SCREEN_W,SCREEN_H);

        if (mouse_b&1) // tenqt de clique
        {
            if (getpixel(menu_coli,mouse_x,mouse_y) == DESERT) //si on clique sur le bontion desert, charge le fichier desert
            {
                chargerFichier(2,0);
                set_current_graph(2);
                fin = true;
            }
            if (getpixel(menu_coli,mouse_x,mouse_y) == SAVANE)//si on clique sur le bontion savanne, charge le fichier savanne
            {
                chargerFichier(3,0);
                set_current_graph(3);
                fin = true;
            }
            if (getpixel(menu_coli,mouse_x,mouse_y) == BANQUISE)//si on clique sur le bontion banquise, charge le fichier banquise
            {
                chargerFichier(1,0);
                set_current_graph(1);
                fin = true;
            }
            if (getpixel(menu_coli,mouse_x,mouse_y) == QUIT)//si on clique sur le bontion quiter, fin change pour quiter
                return true;
        }

        blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
        //fin = true;
    }
    return false;
}


/************************************************************************
Type : M�thode
Utilit� : menu d'execution des boutons de la boite a outils
Return : bool�en de sortie du programme ou non
Cr�ateur :L�o
Statut : fini
***********************************************************************/
bool Graph::boutons()
{
    if (get_quiter() == true) /// on a appuy� suir le bouton quiter
    {
        // std::cout << "\n quiter main";
        vider_graph();
        RAZ_quiter();
        return MenuPrincipal();//cas ou l'utilisateur veut quitter
    }
    if (get_montrerComposantesFortementConnexe() == true) /// on a appuy� sur le bouton pour montrer les composantes fortement connexe
    {
        remplir_tab_adj();
        toutesLesComposantesFortementConnexes();
        affichageForteConnexiteInterface(true);
        RAZ_montrerComposantesFortementConnexe();
    }
    else if (!mouse_b&1)
        affichageForteConnexiteInterface(false); /// cas ou o'on reste appuy� sur le bouton. on attends de relacher le bouton avant d'effacer les couleurs

    if (get_graphReduit() == true) /// on a appuy� sur le bouton graph r�duit
    {
        remplir_tab_adj();
        toutesLesComposantesFortementConnexes();
        GraphReduit();
        m_stopGraphReduit = true;
        RAZ_graphReduit();
    }
    else if (!mouse_b&1 && m_stopGraphReduit == true)
    {
        vider_graph();
        chargerFichier(get_current_graph(),2);
        m_stopGraphReduit = false;
    }
    if (get_restaurer_graph() == true) /// on a appuy� sur le bouton restaurer
    {
        std::cout << "\n restaurer main";
        vider_graph();
        chargerFichier(get_current_graph(),1);
        RAZ_restaurer_graph();
    }
    if (get_sauver() == true) /// on a appuy� sur le bouton sauvegarder
    {
        std::cout << "\n sauver main";
        sauverFichier(get_current_graph(),0);
        vider_graph();
        chargerFichier(get_current_graph(),0);
        RAZ_sauver();
    }
    /*
    if (key[KEY_A])
    {

    m_interface = std::make_shared<GraphInterface>(50, 1, 750, 600);
    }
    */
    return false;
}


/************************************************************************
Type : void
Utilit� : gestion de la vitesse d'�volition
Return : none
Cr�ateur :L�o
Statut : fini
***********************************************************************/
void Graph::evolution(bool* pause, int*rest_evolution, int* t1)
{
/// SIMULATION DE L'EVOLUTION DES POPULATIONS
    std::string nomFichier;
    if (m_AjoutVertex) //bouton ajouter sommet
    {
        menu_ajout_vertex();
        m_AjoutVertex = false;
    }
    if (m_AjoutEdge) //bouton ajouter arete
    {
        menu_ajout_edge();
        m_AjoutEdge = false;
    }

    if(m_playEvolution && *pause == true) /// quand on clique, change l'�tat du boolen play/pause
    {
        *pause = false;
        m_playEvolution = false;
        std::cout << "\npause";
    }
    else if (m_playEvolution) // lancement de l'enregistrement des valeurs pour le graphique
    {
        *pause = true;
        m_playEvolution = false;
        std::cout << "\nplay";

        if (m_CurrentGraph == 1) //chargement du bon fichier
            nomFichier = "banquise";
        else if (m_CurrentGraph == 2)
            nomFichier = "desert";
        else if(m_CurrentGraph == 3)
            nomFichier = "savane";

        nomFichier ="fichier/" + nomFichier+"_evol_graph.txt";


        std::ofstream fichier (nomFichier, std::ios::out);
        if(!fichier)
            std::cerr << "pb lors de l'ouverture de fichier" << std::endl;
        else
        {
            fichier << m_vertices.size() << " ";
            for (auto &e : m_vertices)
            {
                fichier << e.first << " "; // on efface tout le fichier, et on �crit les infos de base pour la lecture
            }
        }
        fichier.close();
    }
/// accelerer/ralentir la vitesse d'�volution
    if(m_vitEvolutionMoins == true) ///acc�lere
    {
        *rest_evolution = *rest_evolution + 10;
        m_vitEvolutionMoins = false;
    }
    if(m_vitEvolutionPlus == true) ///ralentir
    {
        *rest_evolution = *rest_evolution - 10;
        if (*rest_evolution < 20)
            *rest_evolution = 20;
        m_vitEvolutionPlus = false;
    }

    if (*t1 + *rest_evolution <= clock())
    {
        if (*pause == true) // on �crit l'�colution
        {
            CalculPop();
            EnregistrementGraph();
        }
        *t1 = clock();
    }
}

/************************************************************************
Type : M�thode
Utilit� : menu de choix de l'arete
Return : none
Cr�ateur :L�o
Statut : fini
***********************************************************************/
void Graph::menu_ajout_edge()
{
    int from,to; // entiers de d�part et d'arriv�e de l'arete
    bool fin = false; // bool�e de sortie de la boucle
    bool bfrom = false,bto = false; // booleen de verification de sommet
    int idx;
    for (auto a=m_edges.begin() ; a != m_edges.end(); a++) // on mets l'indice du sommet la ou un indice n'existe pas encore
    {
        idx = a->first;
    }
    idx = idx + 1 ;
    std::cout << std::endl << std::endl << "Vous voulez ajouter une arete. Saisisez 2 fois la meme valeur pour sortir";
    while (fin == false) // tant que la v�rification n'est pas valid�
    {
        std::cout << std::endl << " Veuillez saisir l'indice du sommet de depart : ";
        std::cin >> from;//on demande le sommet de d�part
        std::cout << std::endl << "Veuillez maintenant saisir l'indice du sommet d'arrive : ";
        std::cin >> to;//et le sommet d'arriv�
        if (from == to) // on v�rifie que le sommet de d�part et l'arrive ne sont pas les memes : si c'est le cas, on sors sans ajouter d'arete
        {
            fin = true;
        }
        else
        {
            for (auto&it : m_vertices) // on verifie que le sommet existe bien
            {
                if (it.first == from)
                    bfrom = true;
            }
            for (auto&it : m_vertices) // on verifie que le sommet existe bien
            {
                if (it.first == to)
                    bto = true;
            }
            if (bto == true && bfrom == true) // si les 2 sommets existent bien, on valide la sortie
                fin = true;
            else
            {
                bto = false; // RAZ
                bfrom = false;//RAZ
                std::cout << std::endl << "un ou plusieurs des sommets que vous avez saisis n'existent pas";
            }
        }
    }
    if (bto == true && bfrom == true)
        add_interfaced_edge(idx,from,to,0); // si les sommets on �t� valid�, on ajoute le sommet

}

/************************************************************************
Type : M�thode
Utilit� : menu de choix du sommet
Return : none
Cr�ateur :L�o
Statut : fini
***********************************************************************/
void Graph::menu_ajout_vertex()
{
    /*
    std::string chaine = "test";
    int test = 12;
    chaine = chaine + std::to_string(test);
    std::cout<< "\n" << chaine;
    */
    //std::cout << "\najout de vertex";

    bool fin = false; // bool�e de fin
    int color; // recuperation de la couleur de la carte de coli
    int idx ; // index
    for (auto a=m_vertices.begin() ; a != m_vertices.end(); a++) // on mets l'index la ou il n'en existe pas deja
    {
        idx = a->first;
    }
    idx = idx + 1 ;

    BITMAP* menu = charger_image("pics/menu/choix_de_vertex.bmp"); // on charges les bonnes immages
    BITMAP* menu_coli = charger_image("pics/menu/choix_de_vertex_coli.bmp");
    blit(menu,screen,0,0,0,0,SCREEN_W,SCREEN_H);
    while (!fin)
    {
        //std::cout << "\n" << getpixel(menu_coli,mouse_x,mouse_y);
        if (mouse_b&1)
        {
            color = getpixel(menu_coli,mouse_x,mouse_y); // si on clique on r�cup�re la couleur de la carte de coli

            /// On la compare avec la database

            ///DESERT
            if (color == makecol(0,0,0))
                add_interfaced_vertex(idx,50,0,0,"desert/adax.jpg",0,0.08,0.1);
            if (color == makecol(127,127,127))
                add_interfaced_vertex(idx,50,0,0,"desert/dromadaire.jpg",0,0.08,0.2);
            if (color == makecol(136,0,21))
                add_interfaced_vertex(idx,50,0,0,"desert/fourmi.jpg",0,0.08,0.1);
            if (color == makecol(237,28,36))
                add_interfaced_vertex(idx,50,0,0,"desert/gazelle.jpg",0,0.08,0.2);
            if (color == makecol(255,242,0))
                add_interfaced_vertex(idx,50,0,0,"desert/guepard.jpg",0,0.06,1);
            if (color == makecol(34,177,76))
                add_interfaced_vertex(idx,50,0,0,"desert/scorpion.jpg",0,0.07,1);
            if (color == makecol(0,162,232))
                add_interfaced_vertex(idx,50,0,0,"desert/vegetaux.jpg",0,0.25,0.3);

            ///SAVANE
            if (color == makecol(63,72,204))
                add_interfaced_vertex(idx,50,0,0,"savane/buffle.jpg",0,0.08,0.01);
            if (color == makecol(195,195,195))
                add_interfaced_vertex(idx,50,0,0,"savane/gnou.jpg",0,0.08,0.01);
            if (color == makecol(239,228,176))
                add_interfaced_vertex(idx,50,0,0,"savane/lion.jpg",0,0.06,1);
            if (color == makecol(112,146,190))
                add_interfaced_vertex(idx,50,0,0,"savane/vegetaux.jpg",0,0.25,0.03);
            if (color == makecol(163,73,164))
                add_interfaced_vertex(idx,50,0,0,"savane/elephant.jpg",0,0.06,1);
            if (color == makecol(185,122,87))
                add_interfaced_vertex(idx,50,0,0,"savane/hyene.jpg",0,0.06,1);
            if (color == makecol(181,230,29))
                add_interfaced_vertex(idx,50,0,0,"savane/phacochere.jpg",0,0.08,0.01);
            if (color == makecol(255,174,201))
                add_interfaced_vertex(idx,50,0,0,"savane/fourmi.jpg",0,0.1,0.01);
            if (color == makecol(255,201,14))
                add_interfaced_vertex(idx,50,0,0,"savane/leopard.jpg",0,0.06,1);
            if (color == makecol(153,217,234))
                add_interfaced_vertex(idx,50,0,0,"savane/zebre.jpg",0,0.08,0.01);

            ///BANQUISE
            if (color == makecol(120,149,155))
                add_interfaced_vertex(idx,50,0,0,"banquise/krill.jpg",0,0.25,0.3);
            if (color == makecol(100,180,228))
                add_interfaced_vertex(idx,50,0,0,"banquise/morse.jpg",0,0.08,0.1);
            if (color == makecol(33,46,77))
                add_interfaced_vertex(idx,50,0,0,"banquise/phoque.jpg",0,0.08,0.1);
            if (color == makecol(204,74,0))
                add_interfaced_vertex(idx,50,0,0,"banquise/poisson.jpg",0,0.15,0.2);
            if (color == makecol(238,177,69))
                add_interfaced_vertex(idx,50,0,0,"banquise/macareux.jpg",0,0.08,0.1);
            if (color == makecol(19,45,77))
                add_interfaced_vertex(idx,50,0,0,"banquise/ours.jpg",0,0.06,1);
            if (color == makecol(223,230,230))
                add_interfaced_vertex(idx,50,0,0,"banquise/pingouin.jpg",0,0.08,0.1);
            if (color == makecol(200,191,231))
                add_interfaced_vertex(idx,50,0,0,"banquise/renard.jpg",0,0.06,1);

            while(mouse_b&1) {} // on bloque le temps du clique
            fin = true; // on sors
        }
    }
}


