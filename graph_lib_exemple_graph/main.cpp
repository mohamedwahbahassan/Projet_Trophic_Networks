#include "grman/grman.h"
#include "graph.h"
#include "utilitaire.h"
#include <time.h>



int MenuPrincipal()
{
    BITMAP* buffer = create_bitmap(SCREEN_W,SCREEN_H);
    BITMAP* images[NB_IMAGES_MENU];
    BITMAP* menu = charger_image("pics/menu/ecran_d'accueil.bmp");
    BITMAP* menu_coli = charger_image("pics/menu/menu_carte_coli.bmp");
    chargement_images_menu(images);


    bool fin = false;

    blit(menu,buffer,0,0,0,0,SCREEN_W,SCREEN_H);

    while(!fin)
    {
        blit(menu,buffer,0,0,0,0,SCREEN_W,SCREEN_H);

        if (getpixel(menu_coli,mouse_x,mouse_y) == DESERT)
            blit(images[0],buffer,0,0,0,0,SCREEN_W,SCREEN_H);
        if (getpixel(menu_coli,mouse_x,mouse_y) == SAVANE)
            blit(images[1],buffer,0,0,0,0,SCREEN_W,SCREEN_H);
        if (getpixel(menu_coli,mouse_x,mouse_y) == BANQUISE)
            blit(images[2],buffer,0,0,0,0,SCREEN_W,SCREEN_H);
        if (getpixel(menu_coli,mouse_x,mouse_y) == QUIT)
            blit(images[3],buffer,0,0,0,0,SCREEN_W,SCREEN_H);

            if (mouse_b&1)
            {
        if (getpixel(menu_coli,mouse_x,mouse_y) == DESERT)
            return 2;
        if (getpixel(menu_coli,mouse_x,mouse_y) == SAVANE)
            return 3;
        if (getpixel(menu_coli,mouse_x,mouse_y) == BANQUISE)
            return 1;
        if (getpixel(menu_coli,mouse_x,mouse_y) == QUIT)
        return 0;
            }


        blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
        //fin = true;
    }

}



int main()
{


    int t1 = clock();
    int rest_evolution = 2000;
    bool pause = false;

    bool fin = false;

    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    int graph = MenuPrincipal(); //on demande à l'utilisateur quel graph il souhaite ouvrir
    if (graph == 0) return 0;//cas ou l'utilisateur veut quitter


    Graph testFichier;
    testFichier.chargerFichier(graph);
    testFichier.ordredebase();
    testFichier.remplir_tab_adj();

    testFichier.toutesLesComposantesFortementConnexes();
    //testFichier.affichageTableauForteConnexite();
    //testFichier.affichageForteConnexiteInterface();


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
            //std::cout << "\n rest = " << rest_evolution;
        }
        if(key[KEY_D]) ///ralentir
        {
            rest_evolution = rest_evolution + 10;
            //std::cout << "\n rest = " << rest_evolution;
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


















