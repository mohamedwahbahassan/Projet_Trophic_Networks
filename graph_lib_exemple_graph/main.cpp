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
    testFichier.ordredebase();
    testFichier.remplir_tab_adj_sym();
    std::cout << "coucou" << std::endl;


    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )

    while ( !fin )
    {
        /*if(key[KEY_K])
        {
            testFichier.getAllCombin(testFichier.get_serie());
            testFichier.affichage_tab_combin();


        }*/
//        std::cout << "coucou" << std::endl;

        testFichier.evolution(&pause,&rest_evolution,&t1); // évolution dynamique des populations
        fin = testFichier.boutons(); //wrapper de gestion des boutons
  //      std::cout << "coucou" << std::endl;


        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        testFichier.update();
    //     std::cout << "coucou1" << std::endl;
        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();

      //  std::cout << "coucou" << std::endl;

    }
    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


















