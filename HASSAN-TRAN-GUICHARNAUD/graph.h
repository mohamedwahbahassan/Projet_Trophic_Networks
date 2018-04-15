#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

/**************************************************************
    Ici sont proposées 3 classes fondamentales
            Vertex (=Sommet)
            Edge (=Arête ou Arc)
            Graph (=Graphe)

    Les arêtes et les sommets et le graphe qu'ils constituent
    "travaillent" étroitement ensemble : pour cette raison les
    Vertex et Edge se déclarent amis (friend) de Graph pour que
    ce dernier puisse librement accéder aux membres (y compris
    protected ou private) de Vertex et Edge.

    Ces Classes peuvent êtres complétées. Il est également possible
    de les dériver mais il est malheureusement assez difficile
    de dériver le "triplet" des 3 classes en maintenant des relations
    cohérentes ( rechercher "c++ class covariance" et "c++ parallel inheritance"
    pour commencer .. ). Il est donc sans doute préférable, si possible,
    de "customiser" ces classes de base directement, sans héritage.

    Le modèle proposé permet de représenter un graphe orienté éventuellement
    pondéré, les arcs portent une ou des informations supplémentaire(s).
    Les relations/navigations Arcs -> Sommets et Sommets -> Arcs se font
    dans les 2 sens et utilisent des INDICES et NON PAS DES ADRESSES (pointeurs)
    de telle sorte que la topologie du graphe puisse être assez facilement
    lue et écrite en fichier, et bénéficie d'une bonne lisibilité en cas de bugs...

    Chaque arc possède 2 attributs principaux (en plus de son éventuelle pondération)
        -> m_from (indice du sommet de départ de l'arc )
        -> m_to (indice du sommet d'arrivée de l'arc )

    Chaque sommet possède 2 liste d'arcs (en plus de ses attributs "internes", marquages...)
        -> m_in (liste des indices des arcs arrivant au sommet : accès aux prédécesseurs)
        -> m_out (liste des indices des arcs partant du sommet : accès aux successeurs)

    Cependant le problème des indices (par rapport aux pointeurs) et qu'en cas
    de destruction d'une entité (un arc et/ou un sommet sont enlevés du graphe) alors :

    - Soit il faut reprendre toute la numérotation pour "boucher le trou"
      (par exemple on a supprimé le sommet n°4, le sommet n°5 devient le 4, 6 devient 5 etc...)
      ce qui pose des problèmes de stabilité et de cohérence, et une difficulté à ré-introduire
      le(s) même(s) élément supprimé (au même indice)

    - Soit on admet que la numérotation des sommets et arcs n'est pas contigue, càd qu'il
      peut y avoir des "trous" : sommets 0 1 5 7 8, pas de sommets 2 ni 3 ni 4 ni 6. La numérotation
      est stable mais on ne peut plus utiliser un simple vecteur pour ranger la liste de tous
      les arcs et tous les sommets aux indices correspondants, on peut utiliser une map
      qui associe un objet arc ou sommet à des indices arbitraires (pas forcément contigus)

    C'est cette 2ème approche qui est proposée ici : dans la classe graphe vous trouverez
        -> map<int, Edge>   m_edges
        -> map<int, Vertex> m_vertices    (le pluriel de vertex est vertices)

    Il faudra être attentif au fait que par rapport à un simple vecteur, le parcours des éléments
    ne pourra PAS se faire avec un simple for (int i=0; i<m_edges.size(); ++i) ...m_edges[i]...
    et que les parcours à itérateur ne donneront pas directement des Edge ou des Vertex
    mais des pairs, l'objet d'intérêt se trouvant dans "second" ("first" contenant l'indice)
                for (auto &it = m_edges.begin(); it!=m_edges.end(); ++it) ...it->second...
    ou bien     for (auto &e : m_edges) ...e.second...

    Il n'est pas obligatoire d'utiliser ces classes pour le projet, vous pouvez faire les votres

    Au niveau de l'interface, on dissocie une classe interface associée à chaque classe fondamentale
    de telle sorte qu'il soit possible de travailler avec des graphes non représentés à l'écran
    Imaginons par exemple qu'on doive générer 1000 permutations de graphes pour tester des
    combinaisons, on ne souhaite pas représenter graphiquement ces 1000 graphes, et les
    interfaces pèsent lourd en ressource, avec cette organisation on est libre de réserver ou
    pas une interface de présentation associée aux datas (découplage données/interface)

***********************************************************************************************/

#include <vector>
#include <map>
#include <string>
#include <memory>
#include "grman/grman.h"

/***************************************************
                    VERTEX
****************************************************/

class VertexInterface
{
    // Les (methodes des) classes amies pourront accéder
    // directement aux attributs (y compris privés)
    friend class Vertex;
    friend class EdgeInterface;
    friend class Graph;

private :

    /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de déclarer
    /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
    /// le paramétrer ( voir l'implémentation du constructeur dans le .cpp )
    // La boite qui contient toute l'interface d'un sommet
    grman::WidgetBox m_top_box;

    // Un slider de visualisation/modification de la valeur du sommet
    grman::WidgetVSlider m_slider_value;

    // Un label de visualisation de la valeur du sommet
    grman::WidgetText m_label_value;

    // Une image de "remplissage"
    grman::WidgetImage m_img;

    // Un label indiquant l'index du sommet
    grman::WidgetText m_label_idx;

    // Une boite pour le label précédent
    grman::WidgetText m_box_label_idx;
    //une boite croix pour supr le somet
    grman::WidgetCheckBox m_box_close;

public :
    // Le constructeur met en place les éléments de l'interface
    // voir l'implémentation dans le .cpp
    VertexInterface(int idx, int x, int y, std::string pic_name="", int pic_idx=0);
};


class Vertex
{
    // Les (methodes des) classes amies pourront accéder
    // directement aux attributs (y compris privés)
    friend class Graph;
    friend class VertexInterface;
    friend class Edge;
    friend class EdgeInterface;

private :
    /// liste des indices des arcs arrivant au sommet : accès aux prédécesseurs
    std::vector<int> m_in;
    /// liste des indices des arcs partant du sommet : accès aux successeurs
    std::vector<int> m_out;

    /// un exemple de donnée associée au sommet, changed by jojo
    double m_value; ///N: nombre d'individus dans la population
    double m_capacite; /// K: capacité de portage de l'environnement
    double m_quantiteConsomme; ///K2: quantité de N consommée par les autres espèces
    float m_rythmeCroissance; ///r: rythme de croissance differente pour chaque population
    float m_coeffPondere; /// ar le modèle proposé n'est pas étalonné sur des échelles homogènes

    bool m_dejaFortementConexe = false;
    int m_borderColor = VERT;
    /// pour k-connexité qui permet de supprimer un sommet pour essayer la connexité sans le supprimer depuis la map vertice
    bool m_actif= true;

    /// le POINTEUR sur l'interface associée, nullptr -> pas d'interface
    std::shared_ptr<VertexInterface> m_interface = nullptr;

    // Docu shared_ptr : https://msdn.microsoft.com/fr-fr/library/hh279669.aspx
    // La ligne précédente est en gros équivalent à la ligne suivante :
    // VertexInterface * m_interface = nullptr;

public:

    /// Les constructeurs sont à compléter selon vos besoin...
    /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
    // Vertex (double value=0, VertexInterface *interface=nullptr) :
    //   m_value(value), m_interface(interface)   {  }

    ///Constructeur changed by jojo
    Vertex (double value=0, VertexInterface *interface=nullptr, float rythmeCroissance=0, float coeffPondere=0) :
        m_value(value), m_interface(interface),m_rythmeCroissance(rythmeCroissance), m_coeffPondere(coeffPondere)   {  }

    /// Vertex étant géré par Graph ce sera la méthode update de graph qui appellera
    /// le pre_update et post_update de Vertex (pas directement la boucle de jeu)
    /// Voir l'implémentation Graph::update dans le .cpp
    void pre_update();
    void post_update();

    bool get_deja_fortement_connexe(){return m_dejaFortementConexe;}
    int get_borderColor() {return m_borderColor;}

    void set_deja_fortement_connexe(bool val) {m_dejaFortementConexe = val;}
    void set_borderColor(int val) {m_borderColor = val;}

};


/***************************************************
                    EDGE
****************************************************/

class EdgeInterface
{
    // Les (methodes des) classes amies pourront accéder
    // directement aux attributs (y compris privés)
    friend class Edge;
    friend class Graph;
private :

    /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de déclarer
    /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
    /// le paramétrer ( voir l'implémentation du constructeur dans le .cpp )

    // Le WidgetEdge qui "contient" toute l'interface d'un arc
    grman::WidgetEdge m_top_edge;

    // Une boite pour englober les widgets de réglage associés
    grman::WidgetBox m_box_edge;

    // Un slider de visualisation/modification du poids valeur de l'arc
    grman::WidgetVSlider m_slider_weight;

    // Un label de visualisation du poids de l'arc
    grman::WidgetText m_label_weight;

    //box de fermeture de arete
    grman::WidgetCheckBox m_box_Edge_close;

    bool m_autor_supr;

public :

    // Le constructeur met en place les éléments de l'interface
    // voir l'implémentation dans le .cpp
    EdgeInterface(Vertex& from, Vertex& to);
};


class Edge
{
    // Les (methodes des) classes amies pourront accéder
    // directement aux attributs (y compris privés)
    friend class Graph;
    friend class EdgeInterface;
    friend class Vertex;

private :
    /// indice du sommet de départ de l'arc
    int m_from;

    /// indice du sommet d'arrivée de l'arc
    int m_to;

    /// un exemple de donnée associée à l'arc, on peut en ajouter d'autres...
    double m_weight;

    bool m_autorisation_supr_edge = false;

    /// le POINTEUR sur l'interface associée, nullptr -> pas d'interface
    std::shared_ptr<EdgeInterface> m_interface = nullptr;
    /// un booléen qui permet de supprimer les aretes dans l'interface sans réellement les effacer dans la map edge
    bool m_actif = true;

public:
    /// Les constructeurs sont à compléter selon vos besoin...
    /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
    Edge (double weight=0, EdgeInterface *interface=nullptr) :
        m_weight(weight), m_interface(interface)  {  }

    /// Edge étant géré par Graph ce sera la méthode update de graph qui appellera
    /// le pre_update et post_update de Edge (pas directement la boucle de jeu)
    /// Voir l'implémentation Graph::update dans le .cpp
    void pre_update();
    void post_update();

    int get_from() {return m_from;}
    int get_to() {return m_to;}
    int get_weight() {return m_weight;}
};


/***************************************************
                    GRAPH
****************************************************/
class GraphInterface
{
    friend class Graph;

private :

    /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de déclarer
    /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
    /// le paramétrer ( voir l'implémentation du constructeur dans le .cpp )
    //image de fond
    grman::WidgetImage m_imgP;
    /// La boite qui contient toute l'interface d'un graphe
    grman::WidgetBox m_top_box;

    /// Dans cette boite seront ajoutés les (interfaces des) sommets et des arcs...
    grman::WidgetBox m_main_box;
    /// Dans cette boite seront ajoutés des boutons de contrôle etc...
    grman::WidgetBox m_tool_box;

    /*******************************
    BOUTONS
    *******************************/
    grman::WidgetButton m_Button_Save; //bouton sauver
    grman::WidgetText m_Text_Save; //test "sauver

    grman::WidgetButton m_Button_Restaurer; //bouton restaurer
    grman::WidgetText m_Text_Restaurer;

    grman::WidgetButton m_Button_Quit; //bouton quiter
    grman::WidgetText m_Text_Quit;

    grman::WidgetButton m_Button_Graph_Pop; //bouton quiter
    grman::WidgetText m_Text_Graph_Pop;

    grman::WidgetButton m_Button_Ajout_Vertex; //bouton ajouter vertex
    grman::WidgetText m_Text_Ajout_Vertex;

    grman::WidgetButton m_Button_K_Connexite; //bouton K connexite
    grman::WidgetText m_Text_K_Connexite;

    grman::WidgetButton m_Button_Ajout_Edge; //bouton ajouter edge
    grman::WidgetText m_Text_Ajout_Edge;

    grman::WidgetButton m_Button_Montrer_Composantes_Connexe; //bouton ajouter edge
    grman::WidgetText m_Text_Montrer_Composantes_Connexe;

    grman::WidgetButton m_Button_Graph_Reduit; //bouton graph reduit
    grman::WidgetText m_Text_Graph_Reduit;

    grman::WidgetButton m_Button_Supr_Arette;//bouton supprimer arete
    grman::WidgetText m_Text_Supr_arette;

    grman::WidgetButton m_Button_Vit_Evolution;//Boutons d'évolition
    grman::WidgetText m_Text_Vit_Evolution;
    grman::WidgetButton m_Button_Vit_Evolution_plus;
    grman::WidgetText m_Text_Vit_Evolution_plus;
    grman::WidgetButton m_Button_Vit_Evolution_moins;
    grman::WidgetText m_Text_Vit_Evolution_moins;

    // A compléter éventuellement par des widgets de décoration ou
    // d'édition (boutons ajouter/enlever ...)


public :
    // Le constructeur met en place les éléments de l'interface
    // voir l'implémentation dans le .cpp
    GraphInterface(int x, int y, int w, int h);

    std::string m_fond;
};

class Graph
{
    friend class Vertex;
    friend class VertexInterface;
    friend class Edge;
    friend class EdgeInterface;
private :
    /// La "liste" des arêtes
    std::map<int, Edge> m_edges;

    /// La liste des sommets
    std::map<int, Vertex> m_vertices;

    /// le POINTEUR sur l'interface associée, nullptr -> pas d'interface
    std::shared_ptr<GraphInterface> m_interface = nullptr;

    /// un tableau dynamique de 2 dimensions pour l'adjacence
    std::vector<std::vector<int>> m_tab_adj;

    /// tableau dynamique de 2 dimensions pour la forte connexe
    std::vector<std::vector<int>> m_tab_forte_connexite;

    /// le nombre de sommets d'un graphe
    int m_ordre;

    bool m_restaurer_graph = false;

    bool m_sauver = false;

    bool m_quiter = false;

    bool m_SuprEdge = false;

    bool m_AjoutVertex = false;

    bool m_AjoutEdge = false;

    bool m_montrerComposantesConnexe = false;

    bool m_graphReduit = false;

    bool m_stopGraphReduit = false;

    bool m_mouse_prec = false;

    int m_CurrentGraph;

    bool m_playEvolution = false;

    bool m_vitEvolutionPlus = false;

    bool m_vitEvolutionMoins = false;

    /// tableau dynamique de 2 dimensions pour l'adjacence en supposant que le graphe est non orienté
    std::vector<std::vector<int>> m_tab_adj_sym;
    /// tableau dynamique indiquant depuis un sommet de référence s'il est connexe ou pas aux autres sommets (binaire 0 ou 1)
    std::vector<int> m_tab_connexe;
    /// tableau dynamique indiquant quels sommets qu'on pourra supprimer pour rendre le graphe non connexe
    std::vector <std::string> m_tabCombin;


public:

    /// Les constructeurs sont à compléter selon vos besoin...
    /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
    Graph (GraphInterface *interface=nullptr) :
        m_interface(interface)
    {
    }
    void add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name="", int pic_idx=0, float rythmeCroissance = 0, float coeffPondere = 0);  ///changed by jojo
    void add_interfaced_edge(int idx, int vert1, int vert2, double weight=0);

    void chargerFichier(int ordre,int restauration);
    void sauverFichier(int ordre,int restauration);

    void remove_edge(int eidx, int cas);
    void remove_vertex(int eidx, int cas);

    void vider_graph();

    void add_vertex(std::string image);
    void add_edge(int from, int to);

    void WraperBoutons();

    void changement_int(float val);

    ///methode pour remplir le tableau d'adjaence
    void remplir_tab_adj();

    /// methode permettant de trouver une compsante fortement connexe
    std::vector<int> uneComposanteFortementConnexe(int s);
    /// methode permettant de trouver toutes les compsantes fortement connexe
    void toutesLesComposantesFortementConnexes();
    void affichageTableauForteConnexite();

    void affichageForteConnexiteInterface(bool actif);
    void GraphReduit();

    void menu_ajout_vertex();
    void menu_ajout_edge();


    /// methode pour recupérer l'ordre à un instant t
    void ordredebase();
    /// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
    void update();
    bool boutons();
    bool MenuPrincipal();

    ///Fonction qui calcul la population en fonction des populations des autres sommets et coefficients des autres arcs
    void CalculPop();
    void EnregistrementGraph();
    void AffPop();

    //RAZ
    void RAZ_quiter(){m_quiter = false;}
    void RAZ_restaurer_graph(){m_restaurer_graph = false;}
    void RAZ_sauver(){m_sauver = false;}
    void RAZ_playEvolution() {m_playEvolution = false;}
    void RAZ_vitEvolution() {m_vitEvolutionMoins = false; m_vitEvolutionPlus = false;}
    void RAZ_montrerComposantesFortementConnexe(){m_montrerComposantesConnexe = false;}
    void RAZ_graphReduit() {m_graphReduit = false;}

    void evolution(bool* pause, int*rest_evolution, int* t1);

    //geters
    void set_current_graph(int var) {m_CurrentGraph = var;}

    //getters
    bool get_autorisation_supr_edge() {return m_SuprEdge;}
    bool get_restaurer_graph () {return m_restaurer_graph;}
    bool get_sauver () {return m_sauver;}
    bool get_quiter () {return m_quiter;}
    bool get_montrerComposantesFortementConnexe () {return m_montrerComposantesConnexe;}
    bool get_graphReduit() {return m_graphReduit;}
    bool get_playEvolution() {return m_playEvolution;}
    bool get_vitEvolutionMoins() {return m_vitEvolutionMoins;}
    bool get_vitEvolutionPlus() {return m_vitEvolutionPlus;}
    int get_current_graph() {return m_CurrentGraph;}

    /// methode pour remplir le tableau d'adjacence symétrique
    void remplir_tab_adj_sym();
    /// methode pour remplir le tableau de connexité
    std::vector<int> remplir_tab_connexe(int s);
    /// methode pour savoir si le graphe est connexe ou pas en retournant 0 ou 1
    bool Graphe_connexe();
    /// methode pour trouver un sommet de reference qui existe dans l'interface d'une maniere arbitraire
    int chercher_sommet_actif();
    /// methode pour trouver toutes les combinaisons possibles d'un k precis
    void getCombinFork(const std::string &serie, size_t i, std::string result,int* compteur);
    /// methode pour trouver toutes les combianaisons possibles de tous les k possibles en appelant le sous programma au dessus
    void getAllCombin(std::string serie);
    std::string get_serie();

    /// on affiche le tableau conteant les sommets de k-connexité
    void affichage_tab_combin();
    /// on initialise ce tableau
    void initialise_tab_combine();
};


#endif // GRAPH_H_INCLUDED
