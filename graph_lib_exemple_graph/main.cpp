#include "grman/grman.h"


#include "graph.h"

int main()
{
    int graph = 1;
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du r�pertoire o� se trouvent les images � charger
    grman::set_pictures_path("pics");

    Graph testFichier;
    testFichier.chargerFichier(graph);


    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement � des frameworks plus avanc�s )
    while ( !key[KEY_ESC] )
    {
        /// Il faut appeler les m�thodes d'update des objets qui comportent des widgets
        testFichier.update();

        /// Mise � jour g�n�rale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
    }
    testFichier.add_vertex("clown1.jpg");
    //testFichier.remove_vertex(3);
    //testFichier.add_edge(0,7);
    //testFichier.remove_edge(7);

    while ( !key[KEY_A] )
    {
        /// Il faut appeler les m�thodes d'update des objets qui comportent des widgets
        testFichier.update();

        /// Mise � jour g�n�rale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
    }

    //testFichier.sauverFichier(graph);

    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


