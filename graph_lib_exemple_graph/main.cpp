#include "grman/grman.h"
#include "graph.h"
#include "utilitaire.h"
#include <time.h>

int main()
{

    int graph =1;
    int t1 = clock();
    int rest_evolution = 100;
    bool pause = false;

    bool fin = true;

    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    Graph testFichier;
    fin = testFichier.MenuPrincipal(); //on demande à l'utilisateur quel graph il souhaite ouvrir
    testFichier.ordredebase(); //enregistrement de l'ordre du graph


    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )

    do
    {

        testFichier.evolution(&pause,&rest_evolution,&t1); // évolution dynamique des populations
        fin = testFichier.boutons(); //wrapper de gestion des boutons


        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        testFichier.update();
        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();


    }
    while ( !fin );
    grman::fermer_allegro();
    return 0;
}
END_OF_MAIN();

