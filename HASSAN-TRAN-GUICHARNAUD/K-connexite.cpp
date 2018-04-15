#include "graph.h"
#include <fstream>
#include <iostream>


void Graph::remplir_tab_adj_sym()
{
    int i=0,j=0, n=0, g=0;
    /// on initialise le tableau de taille ordre*ordre à 0
    m_tab_adj_sym.resize(m_ordre);
    for(i=0; i<m_ordre; i++)
    {
        m_tab_adj_sym[i].resize(m_ordre);
        for(j=0; j<m_ordre; j++)
        {
            m_tab_adj_sym[i][j]=0;
        }
    }
    /// 1er parcours du map de sommets
    for(auto it=m_vertices.begin(); it!=m_vertices.end(); it++)
    {
        /// 2eme parcours du map de sommets
        for(auto im=m_vertices.begin(); im!=m_vertices.end(); im++)
        {
            /// si les deux sommets sont actifs (ne sont pas supprimés de l'interface pour tester la k-connexité)
            if(it->second.m_actif && im->second.m_actif)
            {
                /// si le sommet de partant n'est pas le même que celui d'arrivée
                if(it->first != im->first)
                {
                    /// on parcourt toutes les aretes sortant du sommet partant
                    for(n=0; n<it->second.m_out.size(); n++)
                    {
                         /// on parcourt toutes les aretes entrant du sommet d'arrivée
                        for(g=0; g<im->second.m_in.size(); g++)
                        {
                            /// et si on trouve une arete sortant du sommet partant qui est la même de celle d'arrivée
                            if(it->second.m_out[n]==im->second.m_in[g])
                            {
                                /// on attribue 1 au tableau d'adjacence de ligne du sommet de partant et de colonne du sommet d'arrivée
                                m_tab_adj_sym[it->first][im->first]=1;
                                /// on attribue 1 au tableau d'adjacence de ligne du sommet d'arrivée et de colonne du sommet de partant
                                m_tab_adj_sym[im->first][it->first]=1;
                            }

                        }
                    }
                }
                /// sinon si le sommet de partant est le même que celui d'arrivée
                else
                {
                    /// on attrbue 1 au tableau d'adjacenet de ligne du sommet de partant et de colonne d'arrivée car un sommet est forcément adjacent à lui même
                    m_tab_adj_sym[it->first][im->first]=1;
                    /// on attrbue 1 au tableau d'adjacenet de ligne du sommet d'arrivée et de colonne de partant car un sommet est forcément adjacent à lui même
                    m_tab_adj_sym[im->first][it->first]=1;

                }
            }
        }
    }

    /*  for(i=0; i<m_ordre; i++)
      {
          for(j=0; j<m_ordre; j++)
          {
              std::cout << m_tab_adj_sym[i][j] << " ";
          }
          std::cout << std::endl;
      }
      */
}
/// pour trouver un sommet actif arbitrairement pour qu'il joue le role d'un sommet de référence
int Graph::chercher_sommet_actif()
{
    int s;
    /// on parcours la map de sommet
    for(auto &e: m_vertices)
    {
        /// on retourne le premier sommet actif
        if(e.second.m_actif==1)
        {
            s=e.first;
            return s;
        }
    }
}

std::vector<int> Graph::remplir_tab_connexe(int s)
{
    std::vector<int> c; /// tableau dynamique contenant les composantes connexes
    std::vector <int> marques; /// tableau dynamique indiquant si les sommets sont marqués ou non
    std::vector<bool> actif;
    int x,y; /// numéros des sommets intermédiares des composantes connexes
    int ajoute= 1;/// booléen indiquant si une nouvelle composante connexe est ajoutée

    c.resize(m_ordre);
    marques.resize(m_ordre);

    for(auto &e : m_vertices)
    {
        std::cout << e.second.m_actif << " ";
    }
    std::cout<< std::endl << "dans remplir connexe" << std::endl;

    /// on recupère dans un veteur l'etat de tous les sommets
    for(auto &e : m_vertices)
    {
        actif.push_back(e.second.m_actif);
    }
    /*  for(int i=0; i<m_ordre; i++)
      {
          std::cout << "la valeur de "<< i <<" est " << actif[i] << std::endl;
      }*/

    ///on intialise le tableau de marquage à 0
    for(x=0; x<m_ordre; x++)
    {
        marques[x]=0;
    }
    /// on initialise le tableau de connexité à 0 sauf les sommets desactives à 1
    for(x=0; x<m_ordre; x++)
    {
        c[x]=0;
        if(actif[x]==false)
        {
            c[x]=1;
        }
    }
    /// on attribue 1 au tableau de connexité pour le sommet de reference car il est forcément connexe à lui-même
    c[s]=1;

    /// tant que toutes les composantes connexe ne sont pas ajoutées, on effectue ce qui est dans la boucle
    while(ajoute)
    {
        /// on met ajoute à 0 et s'il y a une nouvelle composante donc le code va rentrer dans la condition if et ajoute revient à 1 ce qui permettra l'exection de la boucle du nouveau
        ajoute=0;
        /// on parcours de 0 à l'ordre du graphe
        for(x=0; x<m_ordre; x++)
        {
            /// si on trouve un sommet connexe à s et non marqué
            if(!marques[x]&& c[x])
            {
                ///on le marque
                marques[x]=1;
                /// on parcours de nouveau de 0 à l'ordre du graphe
                for(y=0; y<m_ordre; y++)
                {
                     /// si on trouve un sommet adjacent au sommet x et non marqué et actif
                    if(m_tab_adj_sym[x][y]&& !marques[y]&& m_vertices[x].m_actif)
                    {
                        ///on ajoute ce sommet en tant que sommet connexe avec le sommet de s
                        c[y]=1;
                        /// on remet ajoute à 1
                        ajoute =1;
                    }
                }
            }
        }
    }

    for(x=0; x<m_ordre; x++)
    {
        std::cout << c[x] << std::endl;
    }
    std::cout << "fini" << std::endl;


/// on retourne le tableau de connexité
    return c;
}


bool Graph::Graphe_connexe()
{
    /// on déclare un compteur de type int et un booléen connexe
    int cmp=0;
    bool connexe;

    for(auto &e: m_vertices)
    {
        std::cout << e.second.m_actif << " ";
    }

    std::cout << std::endl;

    /// on remplit le tableau de connexité en donnant comme paramètre le premier sommet actif dans la map
    m_tab_connexe=remplir_tab_connexe(chercher_sommet_actif());

    /// on parcourt le tableau de connexité
    for(int i=0; i<m_tab_connexe.size(); i++)
    {
        /// si à l'indice i, on a 1
        if(m_tab_connexe[i]==1)
        {
            /// on incremente le compteur
            cmp++;
        }
    }
    /// si le comteur est égal au nombre de ligne de tableau de connexité cad que le graphe est connexe car le sommet de ref est connexe à tous les sommets
    if(cmp== m_tab_connexe.size())
    {
        /// on met le booleen à true
        connexe=true;
    }
    else
    {
        /// sinon on met le booleen à false
        connexe = false;
    }

    return connexe;
}


void Graph::getCombinFork(const std::string &serie, size_t i, std::string result,int* compteur)
{

    std::string combinaison="";

    int cpt;

    for(size_t j=0; j<serie.size(); ++j) ///avec i= k = le nombre d'élements dans n ///j:la taille de la série

    {

        /// Si i=1, afficher les n éléments de la série
        if(i==1)
        {
            std::cout<<"["<<result+serie[j]<<"]"<<std::endl;

            combinaison=result+serie[j];

            for(int i=0; i<combinaison.size(); i++)
            {
                std::cout <<" combinaison de i est " << combinaison[i] << std::endl;
                remove_vertex((combinaison[i]-48),2);
                std::cout<< "le boleen du VERTEX"<< combinaison[i] << "est desormais à" << m_vertices[combinaison[i]-48].m_actif << std::endl;
                //std::cout << "je supprime le vertex "<< combinaison[i]<< std::endl;
                //std::cout << "l'indice est " << m_vertices[combinaison[i]] << " son bool " << m_vertices[combinaison[i]].m_actif << std::endl;
            }



            //Graphe_connexe();
            bool c=Graphe_connexe();

            std::cout << "bool connexe: " << c << std::endl;

            if (c==false)
            {
                m_tabCombin.push_back(combinaison);
                cpt=1;
                *compteur=cpt;

            }
            else
            {
                std::cout<< " le graphe est toujours connexe"<< std::endl;
            }
            for(auto e=m_vertices.begin() ; e!=m_vertices.end(); e++)
            {
                e->second.m_actif = true;
                std::cout << "j active " << e->first << std::endl;
            }
            for(auto e=m_vertices.begin() ; e!=m_vertices.end(); e++)
            {
                e->second.m_actif = true;
            }

            combinaison="";
            continue;

        }

        /// Si la série ne possède pas assez d'élément (on envoie k>n), la fonction crache
        if(serie.size()-j<i)
        {
            std::cout << "il n'existe pas assez d'elements" << std::endl;
            return;
        }

        ///Fonction récursive qui se rappelle elle même, avec la taille de la chaine=j+1
        getCombinFork(serie.substr(j+1), i-1, result+serie[j],compteur);
    }
}


void Graph::getAllCombin(std::string serie)
{
    int compteur=0;

    /// //chercher toutes les combinaisons possible de la série pour chq k
    for(size_t k=1; k<=serie.size(); ++k)
    {

        getCombinFork(serie, k, "",&compteur);


        if(compteur==1)
        {
            std::cout<< "ce graphe est " << k << "-connexe"<<std::endl;
            k=serie.size();
        }
    }

   /* for(auto &e: m_vertices)
    {
        e.second.m_actif = true;
        m_interface->m_main_box.add_child(e.second.m_interface->m_top_box);
    }
        for(auto &e: m_edges)
    {
        e.second.m_actif=true;
        m_interface->m_main_box.add_child(e.second.m_interface->m_top_edge);


    }*/

}

std::string Graph::get_serie()
{
    /// on déclare une variable de type string qu'on le met à vide au début
    std::string serie ="";
    /// on parcourt la map de sommets
    for (auto &e : m_vertices)
    {
        /// on remplit la variable de type string avec toutes les indices de sommets de la map de vertice
        serie = serie + std::to_string(e.first);
    }

    return serie;
}

void Graph::affichage_tab_combin()
{
    for(int i=0; i<m_tabCombin.size(); i++)
    {
        std::cout << m_tabCombin[i] << std::endl;
    }

}


void Graph::initialise_tab_combine()
{
m_tabCombin.clear();
}


