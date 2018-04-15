#include "graph.h"
#include <fstream>
#include <iostream>


/// Tableau ddjance remlpi � partir des maps de sommets et d'arr�tes, modifiable en cours d'execution du code
void Graph::remplir_tab_adj()
{
    int i=0, j=0;
    //std::cout << "ordre est " << m_ordre << std::endl;
    bool ok = true;

    /// on initialise le tableau de taille ordre*ordre � 0
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

            /// si le sommet de partant n'est pas le m�me que celui d'arriv�e
            if(it->first!=im->first)
            {
                /// on parcourt toutes les aretes sortant du sommet partant
                for(int n=0; n<it->second.m_out.size(); n++)
                {
                    /// on parcourt toutes les aretes entrant du sommet d'arriv�e
                    for (int g=0; g<im->second.m_in.size(); g++)
                    {
                        /// et si on trouve une arete sortant du sommet partant qui est la m�me de celle d'arriv�e
                        if(it->second.m_out[n]==im->second.m_in[g])
                        {
                            /// on attribue 1 au tableau d'adjacence de ligne du sommet de partant et de colonne du sommet d'arriv�e
                            m_tab_adj[it->first][im->first]=1;
                           // n=it->second.m_out.size();
                           // g=im->second.m_in.size();

                        }
                    }
                }

            }
            /// sinon si le sommet de partant est le m�me que celui d'arriv�e
            else
            {
                /// on attrbue 1 au tableau d'adjacenet de ligne du sommet de partant et de colonne d'arriv�e car un sommet est forc�ment adjacent � lui m�me
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
    /// composante fortement connexe  � retourner
    std::vector <int> c;
    std::vector <int> marques; /// tableau dynamique indiquant si les sommets sont marqu�s ou non
    int x,y; /// num�ros de sommets interm�diaires des composantes connexes
    int ajoute=1; /// bool�en indiquant si une nouvelle composante connexe est ajout�e
    int i;


    c1.resize(m_ordre);
    c2.resize(m_ordre);
    c.resize(m_ordre);
    marques.resize(m_ordre);

    /// on initialise toutes les tableaux � 0
    for(i=0; i<m_ordre; i++)
    {
        c1[i]=0;
        c2[i] =0;
        c[i]=0;
        marques[i]=0;
    }

    /// on attribue 1 au tableau de connexit� pour le sommet de reference car il est forc�ment connexe � lui-m�me
    c1[s]=1;
    c2[s]=1;

    /// tant que toutes les composantes connexe ne sont pas ajout�es, on effectue ce qui est dans la boucle
    while(ajoute)
    {
        /// on met ajoute � 0 et s'il y a une nouvelle composante donc le code va rentrer dans la condition if et ajoute revient � 1 ce qui permettra l'exection de la boucle du nouveau
        ajoute =0;

        /// on parcours de 0 � l'ordre du graphe
        for(x=0; x<m_ordre; x++)
        {
            /// si on trouve un sommet connexe directe partant de s et non marqu�
            if(!marques[x] && c1[x])
            {
                /// on le marque
                marques[x]=1;
                /// on parcours de nouveau de 0 � l'ordre du graphe
                for(y=0; y<m_ordre; y++)
                {
                    /// si on trouve un sommet adjacent au sommet x et non marqu�
                    if(m_tab_adj[x][y]&& !marques[y])
                    {
                        ///on ajoute ce sommet en tant que sommet connexe avec le sommet de s en partant de s
                        c1[y]=1;
                        /// on remet ajoute � 1
                        ajoute=1;
                    }
                }

            }
        }
    }

    ///on re-intialise le tableau de marquage � 0
    ajoute = 1;
    for(x=0; x<m_ordre; x++)
    {
        marques[x]=0;
    }
    /// m�me principe que la connexit� partant directement de s mais pour le cas o� on arrive indirectement � s
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
    /// on associe ces 2 tableaux dans un seul tableau pour obtenir une composante fortement connexe pour le sommet S s'il existe
    for(x=0; x<m_ordre; x++)
    {
        c[x]= c1[x] & c2[x];
    }

    return c;

}

void Graph::toutesLesComposantesFortementConnexes()
{
    std::vector<std::vector<int>> tabc; /// tableau dynamique des composantes fortements connexes � retourner
    std::vector<int> marque; /// tableau dynamique indiquant si les sommets sont marqu�s ou non
    int x,y;  /// num�ros de sommets interm�diaires des composantes connexes

    marque.resize(m_ordre);
    tabc.resize(m_ordre);
    /// on initialise les deux tableaux � 0
    for(x=0; x<m_ordre; x++)
    {
        marque[x]=0;
        tabc[x].resize(m_ordre);
        for(y=0; y<m_ordre; y++)
        {
            tabc[x][y]=0;
        }
    }

    /// on parcourt de 0 � l'ordre du graphe
    for(x=0; x<m_ordre; x++)
    {
        /// on attribue comme sommet de ref pour le sous prog Unecomposantefortementconnexe x
        /// et on remplit tabc � la ligne x par le tableau qu'on recup�re depuis le sous prog UneComposanteFortementConnexe
        tabc[x]=uneComposanteFortementConnexe(x);
        /// et on marque x
        marque[x]=1;
        /// on parcours de nouveau de 0 � l'ordre du graphe
        for(y=0; y<m_ordre; y++)
        {
            /// si le sommet de partant x et le sommet d'arrivee y sont fortement connexe et y est non marqu�
            if(tabc[x][y] && !marque[y])
            {
                /// on marque y
                marque[y]=1;
            }
        }
    }

    /// on retourne le tableau contenant toutes les composantes fortement connexe du graphe
    m_tab_forte_connexite=tabc;

}

void Graph::ordredebase()
{
    int indice;
    /// on recup�re l'ordre du graphe
    for (auto a=m_vertices.begin() ; a != m_vertices.end(); a++)
    {
        indice = a->first;
    }
    m_ordre = indice +1;
    //std::cout << "ordre est " << m_ordre << std::endl;
}



