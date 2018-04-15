#include <allegro.h>
#include "utilitaire.h"

 //un wraper pour charger les images du menu
void chargement_images_menu(BITMAP*images[NB_IMAGES_MENU] )
{
    images[0] = charger_image("pics/menu/ecran_d'accueil_0.bmp");
    images[1] = charger_image("pics/menu/ecran_d'accueil_1.bmp");
    images[2] = charger_image("pics/menu/ecran_d'accueil_2.bmp");
    images[3] = charger_image("pics/menu/ecran_d'accueil_3.bmp");
}
/************************************************************************
Type : M�thode
Utilit� : charger une image proprement
Return : image charg�
Cr�ateur :L�o
Statut : fini
***********************************************************************/
BITMAP* charger_image(char nom_fic[50])
{
    BITMAP* bmp;
    bmp=load_bitmap(nom_fic ,NULL);

    if (!bmp)
    {
        allegro_message("failed to charge : %s ", nom_fic);
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    //else std::cout << "\n" << nom_fic << "charge avec succes" <<  nom_fic;
    return bmp;
}


