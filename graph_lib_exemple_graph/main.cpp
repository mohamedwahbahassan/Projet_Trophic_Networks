#include "grman/grman.h"


#include "graph.h"

int main()
{
    int graph = 1;
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    Graph testFichier;
    testFichier.chargerFichier(graph);


    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
    while ( !key[KEY_ESC] )
    {
        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        testFichier.update();

        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
    }

    testFichier.sauverFichier(graph);

    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


