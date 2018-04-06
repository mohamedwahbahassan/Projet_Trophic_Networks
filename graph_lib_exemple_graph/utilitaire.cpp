#include "utilitaire.h"







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
    else printf ("\n%s charge avec succes", nom_fic);
    return bmp;
}







