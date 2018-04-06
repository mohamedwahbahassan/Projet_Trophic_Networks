#include "graph.h"
#include <fstream>
#include <iostream>







/// Tableau d'adjance remlpi à partir des maps de sommets et d'arrêtes, modifiable en cours d'execution du code
void Graph::remplir_tab_adj()
{
    int i=0, j=0;
    std::cout << "ordre est " << m_ordre << std::endl;
    bool ok = true;

    m_tab_adj.resize(m_ordre);
    for(i=0; i<m_ordre; i++)
    {
        m_tab_adj[i].resize(m_ordre,0);
        for(j=0; j<m_ordre; j++)
        {
            m_tab_adj[i][j]=0;
        }
    }

    /// 1er parcours du map de sommets
    i=0;
    for(auto it=m_vertices.begin(); it!=m_vertices.end(); it++)
    {
        j=0;
        /// alloué le tableau d'adjance
        /// 2eme parcours du map de sommets
        for(auto im=m_vertices.begin(); im!=m_vertices.end(); im++)
        {
            /// alloué le tableau d'adjance d'indice i

            /// si le sommet de partant n'est pas le même que celui d'arrivée
            if(i!=j)
            {

                /// on parcours le tableau d'arêtes
                for (auto k=m_edges.end(); k!=m_edges.begin(); k--)
                {

                        /// si on trouve une arête qui a le même sommet partant que i et le même sommet entrant que j alors notre tableau d'adjance[i][j] = 1
                        if(k->second.m_from == it->first  && k->second.m_to==im ->first)
                        {
                            m_tab_adj[it->first][im->first]=1;
                           // k=m_edges.begin();
                        }
                        else
                        {
                            m_tab_adj[it->first][im->first]=0;

                        }


                }
            }
            /// sinon si i = j alors tableau d'adjance = 1 car un sommet est forcément adjacent à lui même
            else
            {
                m_tab_adj[it->first][im->first]=1;
            }
            j++;
        }
        i++;
    }

    for(i=0; i<m_ordre; i++)
    {
        for(j=0; j<m_ordre; j++)
        {
            std::cout<< m_tab_adj[i][j] << " ";
        }
        std::cout << std::endl;
    }



}

std::vector <int> Graph::uneComposanteFortementConnexe(int s)
{
    /// composantes connexes directes partant de s et indirectes arrivant vers s
    std::vector<int> c1;
    std::vector<int> c2;
    /// composante fortement connexe  à retourner
    std::vector <int> c;
    std::vector <int> marques; /// tableau dynamique indiquant si les sommets sont marqués ou non
    int x,y; /// numéros de sommets intermédiaires des composantes connexes
    int ajoute=1; /// booléen indiquant si une nouvelle composante connexe est ajoutée
    int i;


    c1.resize(m_ordre);
    c2.resize(m_ordre);
    c.resize(m_ordre);
    marques.resize(m_ordre);

    for(i=0; i<m_ordre; i++)
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

        for(x=0; x<m_ordre; x++)
        {
            if(!marques[x] && c1[x])
            {
                marques[x]=1;
                for(y=0; y<m_ordre; y++)
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
    for(x=0; x<m_ordre; x++)
    {
        marques[x]=0;
    }
    while(ajoute)
    {
        ajoute=0;
        for (x=0; x<m_ordre; x++)
        {

            if(!marques[x] && c2[x])
            {
                marques[x]=1;
                for(y=0; y<m_ordre; y++)
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




    for(x=0; x<m_ordre; x++)
    {
        c[x]= c1[x] & c2[x];
    }

    return c;

}

void Graph::toutesLesComposantesFortementConnexes()
{
    std::vector<std::vector<int>> tabc; /// tableau dynamique des composantes fortements connexes à retourner
    std::vector<int> marque; /// tableau dynamique indiquant si les sommets sont marqués ou non
    int x,y;  /// numéros de sommets intermédiaires des composantes connexes

    std::cout << "l'ordre est " << m_ordre << std::endl;
    marque.resize(m_ordre);
    tabc.resize(m_ordre);
    for(x=0; x<m_ordre; x++)
    {
        marque[x]=0;
        tabc[x].resize(m_ordre);
        for(y=0; y<m_ordre; y++)
        {
            tabc[x][y]=0;
        }
    }

    for(x=0; x<m_ordre; x++)
    {
        tabc[x]=uneComposanteFortementConnexe(x);
        marque[x]=1;
        for(y=0; y<m_ordre; y++)
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
    std::cout << " Tableau de forte connexité" << std::endl;
    for(int i=0; i<m_ordre; i++)
    {
        for(int j=0; j<m_ordre; j++)
        {
            std::cout << m_tab_forte_connexite[i][j] << " ";
        }
        std::cout << std::endl;
    }
}



void Graph::affichageForteConnexiteInterface()
{
    int i,j;
    for(auto it=m_vertices.begin(); it!=m_vertices.end(); it++)
    {
        it -> second.m_interface->m_top_box.set_border_color(VERT);
    }


    for(int i=0; i<m_ordre-1; i++)
    {
        for(j=i+1; j<m_ordre; j++)
        {
            if(m_tab_forte_connexite[i]==m_tab_forte_connexite[j])
            {
                m_vertices[i].m_interface->m_top_box.set_border_color(ROUGE);
                m_vertices[j].m_interface->m_top_box.set_border_color(ROUGE);
            }

        }


    }
}







void Graph::ordredebase()
{
    m_ordre = m_vertices.size();
    std::cout << "ordre est " << m_ordre << std::endl;
}







