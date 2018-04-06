#include "grman/grman.h"
#include "graph.h"
#include "utilitaire.h"
#include <time.h>






void MenuPrincipal()
{

}



int main()
{
    int graph =1;

    int t1 = clock();
    int rest_evolution = 2000;
    bool pause = false;

    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    MenuPrincipal();

    Graph testFichier;
    testFichier.chargerFichier(graph);
    testFichier.remplir_tab_adj();

    //testFichier.uneComposanteFortementConnexe(7);
    testFichier.toutesLesComposantesFortementConnexes();

    testFichier.affichageTableauForteConnexite();

    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )

    while ( !key[KEY_ESC] )
    {

/// SIMULATION DE L'EVOLUTION DES POPULATIONS

        if(key[KEY_SPACE] && pause == true)
        {
            pause = false;
            while (key[KEY_SPACE]) {}
        }
        else if (key[KEY_SPACE])
        {
            pause = true;
            while (key[KEY_SPACE]) {}
        }


/// accelerer/ralentir la vitesse d'évolution

        if(key[KEY_Q]) ///accélere
        {
            rest_evolution = rest_evolution - 10;
            std::cout << "\n rest = " << rest_evolution;
        }
        if(key[KEY_D]) ///ralentir
        {
            rest_evolution = rest_evolution + 10;
            std::cout << "\n rest = " << rest_evolution;
        }

        if (t1 + rest_evolution <= clock())
        {

            if (pause == true)
                testFichier.CalculPop();

            t1 = clock();

        }



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


















