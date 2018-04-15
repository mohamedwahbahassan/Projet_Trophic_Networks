#include "graph.h"
#include <fstream>
#include <iostream>


void Graph::remplir_tab_adj_sym()
{
    int i=0,j=0, n=0, g=0;
    m_tab_adj_sym.resize(m_ordre);
    for(i=0; i<m_ordre; i++)
    {
        m_tab_adj_sym[i].resize(m_ordre);
        for(j=0; j<m_ordre; j++)
        {
            m_tab_adj_sym[i][j]=0;
        }
    }

    for(auto it=m_vertices.begin(); it!=m_vertices.end(); it++)
    {
        for(auto im=m_vertices.begin(); im!=m_vertices.end(); im++)
        {
            if(it->second.m_actif && im->second.m_actif)
            {
                if(it->first != im->first)
                {
                    for(n=0; n<it->second.m_out.size(); n++)
                    {
                        for(g=0; g<im->second.m_in.size(); g++)
                        {
                            if(it->second.m_out[n]==im->second.m_in[g])
                            {

                                m_tab_adj_sym[it->first][im->first]=1;
                                m_tab_adj_sym[im->first][it->first]=1;
                            }

                        }
                    }
                }
                else
                {
                    m_tab_adj_sym[it->first][im->first]=1;

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

int Graph::chercher_sommet_actif()
{
    int s;
    for(auto &e: m_vertices)
    {
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

    for(auto &e : m_vertices)
    {
        actif.push_back(e.second.m_actif);
    }
    /*  for(int i=0; i<m_ordre; i++)
      {
          std::cout << "la valeur de "<< i <<" est " << actif[i] << std::endl;
      }*/

    for(x=0; x<m_ordre; x++)
    {
        marques[x]=0;
    }
    for(x=0; x<m_ordre; x++)
    {
        c[x]=0;
        /* if(actif[x]==false)
         {
             c[x]=1;
         }*/
    }
    c[s]=1;

    while(ajoute)
    {
        ajoute=0;
        for(x=0; x<m_ordre; x++)
        {
            if(!marques[x]&& c[x])
            {
                marques[x]=1;

                for(y=0; y<m_ordre; y++)
                {
                    if(m_tab_adj_sym[x][y]&& !marques[y]&& m_vertices[x].m_actif)
                    {
                        c[y]=1;
                        ajoute =1;
                    }
                }
            }
        }
    }

    for(x=0; x<m_ordre; x++)
    {
        std::cout<<"connexite " << x << " = " << c[x] << std::endl;
    }
    std::cout << "fini" << std::endl;



    return c;
}


bool Graph::Graphe_connexe()
{
    int cmp=0;
    bool connexe;

    for(auto &e: m_vertices)
    {
        std::cout << e.second.m_actif << " ";
    }

    std::cout << std::endl;

    m_tab_connexe=remplir_tab_connexe(chercher_sommet_actif());

    for(int i=0; i<m_tab_connexe.size(); i++)
    {
        if(m_tab_connexe[i]==1)
        {
            cmp++;
        }
    }
    if(cmp== m_tab_connexe.size())
    {
        connexe=true;
    }
    else
    {
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
                std::cout<< " le graphe n'est plus connexe"<< std::endl;
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

    std::cout << "je suis combin" << std::endl;
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


  /*  for(int i=0; i<m_ordre; i++)
    {
        for(int j=0; j<m_ordre; j++)
        {
            std::cout << m_tab_adj_sym[i][j] << " ";
        }
        std::cout << std::endl;
    }*/

 /*   for(int i=0; i<m_ordre; i++)
    {
        stdm_tab_connexe[i]
    }
*/



    m_tab_adj_sym.clear();
    m_tab_connexe.clear();

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
    std::string serie ="";
    for (auto &e : m_vertices)
    {
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
