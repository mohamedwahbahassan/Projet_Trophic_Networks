#include "graph.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "utilitaire.h"

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

       // std::cout << "SOMMET"<< e.first << std::endl;
       // std::cout << "valeur initiale" << e.second.m_value<< std::endl;

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

          //  std::cout << "capacite de portage = " << capaciteinit << "+" << (m_edges[e.second.m_in[i]].m_weight)/100 << "*"<< m_vertices[m_edges[e.second.m_in[i]].m_from].m_value<< "=" << e.second.m_capacite<< std::endl;
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
            + (m_edges[e.second.m_out[i]].m_weight)/100 * m_vertices[m_edges[e.second.m_out[i]].m_to].m_value;

            ///AFFICHAGE DU CALCUL DE LA QUANTITE CONSOMMEE
           // std::cout << "quantite consomme=" << quantiteinit << "+" << (m_edges[e.second.m_out[i]].m_weight)/100 << "*" << m_vertices[m_edges[e.second.m_out[i]].m_to].m_value << "=" << e.second.m_quantiteConsomme << std::endl;
        }

///Calcul de l'évolution de la population à partir d'un certain temps t

        double valueinit= e.second.m_value;
        float MortVieillesse= 0.05*e.second.m_value; /// 10 % de la population meurt chq année

        if(e.second.m_capacite!=0)
        {

            e.second.m_value = e.second.m_value
                               + e.second.m_rythmeCroissance * e.second.m_value * (1-(e.second.m_value/e.second.m_capacite)) ///R * N (1 - N/K)
                               - e.second.m_quantiteConsomme * e.second.m_coeffPondere ///R2*CoeffPondere
                               - MortVieillesse; ///Mort de Vieillesse

        }

        ///si la capacité de portage est égale à 0 et que ce n'est pas un végétal (on ne peut pas diviser par 0)
        if ((e.second.m_capacite==0) && e.first!=0)
        {
            ///Si la capacité de portage de l'environnement est égale à 0, l'espèce meurt
           // std::cout << e.second.m_capacite << std::endl;
            e.second.m_value = 0;
        }

        ///Si on obtient une population négative, value=0

        if(e.second.m_value < 0)
        {
            e.second.m_value=0;
        }


        ///AFFICHAGE DES CALCULS
        /*
        std::cout << "\ncalcul nouvelle valeur:" << valueinit << " + " << e.second.m_rythmeCroissance << " * " << valueinit << " * "
                  << "(1-" << valueinit << " / " << e.second.m_capacite<<" ) " <<" - " << e.second.m_quantiteConsomme << " * " << e.second.m_coeffPondere<< " - " << MortVieillesse <<" = " << e.second.m_value << std::endl;
        std::cout << "1: capacite: " << e.second.m_capacite << std::endl;
        std::cout << "2: quantite consomme: " << e.second.m_quantiteConsomme<< std::endl;
        std::cout << "3: naissance(s): " << e.second.m_rythmeCroissance * e.second.m_value * (1-(e.second.m_value/e.second.m_capacite)) << std::endl;
        std::cout << "4: mort(s) par vieillesse: " << MortVieillesse << std::endl;
        std::cout << "NOUVELLE VALEUR: " <<e.second.m_value << std::endl;
        std::cout << std::endl << std::endl;
*/
    }

}


void Graph::EnregistrementGraph()
{

    std::string nomFichier;
    if (m_CurrentGraph == 1)
        nomFichier = "banquise";
    else if (m_CurrentGraph == 2)
        nomFichier = "desert";
    else if(m_CurrentGraph == 3)
        nomFichier = "savane";

        nomFichier ="fichier/" + nomFichier+"_evol_graph.txt";

    std::ofstream fichier (nomFichier, std::ios::app);
if(!fichier)
    std::cerr << "pb lors de l'ouverture de fichier" << std::endl;
    else{
    for (auto &e : m_vertices)
    {
        fichier << " " << e.second.m_value;
    }
    }
    fichier.close();
}

void Graph::AffPop(){


    std::string nomFichier;
    BITMAP*buffer1 = charger_image("pics/fondgraph.bmp"); // on mets le fond
    float var,nb;

    if (m_CurrentGraph == 1) // on ouvre le bon fichier
        nomFichier = "banquise";
    else if (m_CurrentGraph == 2)
        nomFichier = "desert";
    else if(m_CurrentGraph == 3)
        nomFichier = "savane";

        nomFichier ="fichier/" + nomFichier+"_evol_graph.txt";

        std::vector<std::vector<float> > vec;

    std::ifstream fichier (nomFichier, std::ios::in);
if(!fichier)
    std::cerr << "pb lors de l'ouverture de fichier" << std::endl;
    else{
            fichier >> var; // on enrefistre les valeurs de lecture dans un vecteur a 2 dimentions
            for (int i = 0 ; i < var ; i ++)
                vec.push_back( std::vector <float> ()) ;

            for (int i = 0 ; i < var ; i ++)
                vec[i].push_back( COULEURALEATOIRE ) ;

         while(!fichier.eof())
        {
            for (int i = 0 ; i < var ; i ++)
            {
                fichier >> nb; // on enregistre les valeurs du graph a la suite du vecteur a 2 dimentions
                vec[i].push_back(nb);
            }
        }
    }
    fichier.close(); // on ferme le fivhier (propreté quand meme ^^ )

for (int j = 0 ; j < var ; j++){ // affiche le graph par dessus l'image de fond
for (int i = 2; i < vec[2].size()-3;i++)
{ // on quadriple les lignes pour avoir une ligne plus épaisse
    line (buffer1, 91 + i*900/vec[0].size()  ,  545 - vec[j][i]*5  ,  91 + (i+1)*900/vec[j].size()  ,  545 - vec[j][i+1]*5  ,  vec[j][0]);
    line (buffer1, 90 + i*900/vec[0].size()  ,  544 - vec[j][i]*5  ,  90 + (i+1)*900/vec[j].size()  ,  544 - vec[j][i+1]*5  ,  vec[j][0]);
    line (buffer1, 91 + i*900/vec[0].size()  ,  544 - vec[j][i]*5  ,  91 + (i+1)*900/vec[j].size()  ,  544 - vec[j][i+1]*5  ,  vec[j][0]);
    line (buffer1, 90 + i*900/vec[0].size()  ,  545 - vec[j][i]*5  ,  90 + (i+1)*900/vec[j].size()  ,  545 - vec[j][i+1]*5  ,  vec[j][0]);
}

}
    blit(buffer1,screen,0,0,0,0,SCREEN_W,SCREEN_H);
while(mouse_b&1){} // on bloque tant que on reste appuyé sur la souris

}


/*void Graph::simulate_Kconnexite(std::vector<std::string> tabCombin)
{

int choix;

std::cout << "quelle combinaison voulez vous testez?" << std::endl;
std::cin >> choix ;


for(unsigned int i=0; i<tabCombin[choix].size();i++)
{
    std::cout<< "je met la population de mon sommet "<< tabCombin[choix][i] << " à 0" << std::endl;
    m_vertices[tabCombin[choix][i]].m_value=0;
}
    CalculPop();
}*/

