
#include "graph.h"
#include <fstream>
#include <iostream>










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
        int MortVieillesse= 0.1*e.second.m_value; /// 10 % de la population meurt chq année

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
            std::cout << e.second.m_capacite << std::endl;
            e.second.m_value = 0;
        }

        ///Si on obtient une population négative, value=0

        if(e.second.m_value < 0)
        {
            e.second.m_value=0;
        }

        ///AFFICHAGE DES CALCULS
        std::cout << "\ncalcul nouvelle valeur:" << valueinit << "+" << e.second.m_rythmeCroissance << "*" << valueinit << "*"
                  << "(1-" << valueinit << "/" << e.second.m_capacite<<")" <<"-" << e.second.m_quantiteConsomme << "*" << e.second.m_coeffPondere<< "=" << e.second.m_value << std::endl;
        std::cout << "1: capacite: " << e.second.m_capacite << std::endl;
        std::cout << "2: quantite consomme: " << e.second.m_quantiteConsomme<< std::endl;
        std::cout << "3:nouvelle valeur:" <<e.second.m_value << std::endl;
        std::cout << std::endl << std::endl;

    }

}










