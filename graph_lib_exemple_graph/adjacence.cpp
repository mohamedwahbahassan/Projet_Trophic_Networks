#include "graph.h"
#include <fstream>
#include <iostream>







/// Tableau d'adjance remlpi à partir des maps de sommets et d'arrêtes, modifiable en cours d'execution du code
void Graph::remplir_tab_adj()
{
    int i=0, j=0;
    //std::cout << "ordre est " << m_ordre << std::endl;
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
    for(auto it=m_vertices.begin(); it!=m_vertices.end(); it++)
    {
        /// 2eme parcours du map de sommets
        for(auto im=m_vertices.begin(); im!=m_vertices.end(); im++)
        {

            /// si le sommet de partant n'est pas le même que celui d'arrivée
            if(it->first!=im->first)
            {
                for(int n=0; n<it->second.m_out.size(); n++)
                {
                    for (int g=0; g<im->second.m_in.size(); g++)
                    {
                        if(it->second.m_out[n]==im->second.m_in[g])
                        {
                            m_tab_adj[it->first][im->first]=1;
                           // n=it->second.m_out.size();
                           // g=im->second.m_in.size();

                        }
                    }
                }

            }

            /// sinon si i = j alors tableau d'adjance = 1 car un sommet est forcément adjacent à lui même
            else
            {
                m_tab_adj[it->first][im->first]=1;
            }
        }
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


/*
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
*/




void Graph::ordredebase()
{
    int indice;
    for (auto a=m_vertices.begin() ; a != m_vertices.end(); a++)
    {
        indice = a->first;
    }
    m_ordre = indice +1;
    //std::cout << "ordre est " << m_ordre << std::endl;
}







