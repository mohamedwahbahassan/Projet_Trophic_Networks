#include "graph.h"
#include <fstream>
#include <iostream>






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
        nomFichier = "banquise1";
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
    std::cout << "fichier ouvert1" << std::endl;
    if(fsommets)
    {
        std::cout << "firchier ouvert2" << std::endl;
        while(!fsommets.eof())
        {
            fsommets>>idx;
            fsommets>>value;
            fsommets>>x;
            fsommets>>y;

            //std::cout << y << "\n";

            fsommets>> r; /// Rythme Croissance
            fsommets>> c; ///Coeff Pondéré

            ///add by jojo
            //std::cout << r << "\n";
            //std::cout << c << "\n";

            fsommets>> picture_name;
            std::cout << "\n" << idx << " " <<  value<< " " << x<<" " << y;

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

            //std::cout << "\n rythme " << it->second.m_rythmeCroissance;

            //std::cout << "\n coeff " << it->second.m_coeffPondere;

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






