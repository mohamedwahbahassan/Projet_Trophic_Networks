#include "grman/grman.h"
#include "graph.h"
#include "utilitaire.h"
#include <time.h>


int main()
{


    int t1 = clock();
    int rest_evolution = 2000;
    bool pause = false;


    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");


    Graph testFichier;
    bool fin = testFichier.MenuPrincipal(); //on demande à l'utilisateur quel graph il souhaite ouvrir
    testFichier.ordredebase(); //enregistrement de l'ordre du graph


    testFichier.remplir_tab_adj();
    testFichier.toutesLesComposantesFortementConnexes();
    //testFichier.affichageTableauForteConnexite();
    testFichier.affichageForteConnexiteInterface();


    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )

    while ( !fin )
    {


        testFichier.evolution(&pause,&rest_evolution,&t1); // évolution dynamique des populations
        fin = testFichier.boutons(); //wrapper de gestion des boutons

        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        testFichier.update();
        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();








    }
    // testFichier.add_vertex("clown1.jpg");
    //testFichier.remove_vertex(3);
    //testFichier.add_edge(0,7);
    //testFichier.remove_edge(7);

    //testFichier.sauverFichier(graph);

    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


















