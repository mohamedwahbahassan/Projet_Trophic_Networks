#ifndef UTILITAIRE_H_INCLUDED
#define UTILITAIRE_H_INCLUDED

#include <fstream>
#include <iostream>


#define DESERT makecol(136,0,21)
#define SAVANE makecol (255,242,0)
#define BANQUISE makecol (34,177,76)

#define NB_IMAGES_MENU 3


BITMAP* charger_image(char nom_fic[50]);

void chargement_images_menu(BITMAP*images[NB_IMAGES_MENU] );




#endif // UTILITAIRE_H_INCLUDED
