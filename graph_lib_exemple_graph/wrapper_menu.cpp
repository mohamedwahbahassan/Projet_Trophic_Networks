#include "graph.h"
#include "utilitaire.h"
#include <fstream>
#include <iostream>



bool Graph::MenuPrincipal()
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
    {
        chargerFichier(2,false);
        set_current_graph(2);
        fin = true;
    }
            if (getpixel(menu_coli,mouse_x,mouse_y) == SAVANE)
    {
        chargerFichier(3,false);
        set_current_graph(3);
        fin = true;
    }
            if (getpixel(menu_coli,mouse_x,mouse_y) == BANQUISE)
    {
        chargerFichier(1,false);
        set_current_graph(1);
        fin = true;
    }
            if (getpixel(menu_coli,mouse_x,mouse_y) == QUIT)
                return true;



        }


        blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
        //fin = true;
    }
    return false;

}






bool Graph::boutons()
{
     if (get_quiter() == true)
        {
            std::cout << "\n quiter main";
            vider_graph();
            RAZ_quiter();
            return MenuPrincipal();//cas ou l'utilisateur veut quitter
        }


        if (get_restaurer_graph() == true)
        {
            std::cout << "\n restaurer main";
            vider_graph();
            chargerFichier(get_current_graph(),true);
            RAZ_restaurer_graph();
        }
        if (get_sauver() == true)
        {
            std::cout << "\n sauver main";
            sauverFichier(get_current_graph(),false);
            vider_graph();
            chargerFichier(get_current_graph(),false);
            RAZ_sauver();
        }







        return false;
}


