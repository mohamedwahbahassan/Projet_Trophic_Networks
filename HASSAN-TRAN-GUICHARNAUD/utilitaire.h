#ifndef UTILITAIRE_H_INCLUDED
#define UTILITAIRE_H_INCLUDED

#include <fstream>
#include <iostream>
#include <time.h>

#define DESERT makecol(136,0,21)
#define SAVANE makecol (255,242,0)
#define BANQUISE makecol (34,177,76)
#define QUIT makecol (181,230,29)

#define ENREGISTREMENT 100 ///enregistrement tout les 100 ms

#define NB_IMAGES_MENU 4

BITMAP* charger_image(char nom_fic[50]);

void chargement_images_menu(BITMAP*images[NB_IMAGES_MENU] );

#endif // UTILITAIRE_H_INCLUDED
