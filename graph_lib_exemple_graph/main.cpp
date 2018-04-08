#include "grman/grman.h"
#include "graph.h"
#include "utilitaire.h"
#include <time.h>


int main()
{
    int graph =1;
    int t1 = clock();
    int rest_evolution = 2000;
    bool pause = false;
bool fin = true;

    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du r�pertoire o� se trouvent les images � charger
    grman::set_pictures_path("pics");


    Graph testFichier;
    fin = testFichier.MenuPrincipal(); //on demande � l'utilisateur quel graph il souhaite ouvrir
    testFichier.ordredebase(); //enregistrement de l'ordre du graph
//    testFichier.remplir_tab_adj();



//    testFichier.toutesLesComposantesFortementConnexes();
//    testFichier.affichageTableauForteConnexite();

//    testFichier.affichageForteConnexiteInterface();




    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement � des frameworks plus avanc�s )

   do
    {


        testFichier.evolution(&pause,&rest_evolution,&t1); // �volution dynamique des populations
        fin = testFichier.boutons(); //wrapper de gestion des boutons


                if(key[KEY_K])
        {
            testFichier.ordredebase();
            testFichier.getAllCombin(testFichier.get_serie());
            testFichier.affichage_tab_combin();
            testFichier.initialise_tab_combine();
            while (key[KEY_K]){}

        }


        /// Il faut appeler les m�thodes d'update des objets qui comportent des widgets
        testFichier.update();
        /// Mise � jour g�n�rale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();



        }while ( !fin );

    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


















