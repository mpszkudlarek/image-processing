#include <stdio.h>
#include <stdlib.h>
#include "header.h"

int main(int argc, char **argv)
{
    int odczyt;    /*Sprawdzenie poprawnosci odczytu*/
    t_obraz temp;  /* tymczasowy obraz, ktory poddawany jest obrobce */
    w_opcje wybor; /* opcje */
    if ((odczyt = przetwarzaj_opcje(argc, argv, &wybor)) == 0)
    {
        if ((odczyt = czytaj(&temp, &wybor)) == 1)
        {
            if (wybor.konturowanie == 1)
            {
                kontur(&temp, &wybor);
            }
            if (wybor.negatyw == 1)
            {
                negatyw(&temp, &wybor);
            }
            if (wybor.progowanie == 1)
                progowanie(&temp, &wybor);
            if (wybor.prog_biel == 1)
                p_biel(&temp, &wybor);
            if (wybor.prog_czern == 1)
                p_czern(&temp, &wybor);
            if (wybor.kolor == 1)
                przetw_kolor(&temp, &wybor);
            if ((odczyt = pisz(&temp, &wybor) == 0))
            {
                if (wybor.wyswietlenie == 1)
                    wyswietl(wybor.nazwa_pliku_wy);
            }
            else
                printf("Blad funkcji pisz\n");
        }
    }
    else
        printf("%d", odczyt);
    free(temp.obraz_we); // zwolnienie pamieci
    temp.obraz_we = NULL;
    free(temp.obraz_wy);
    temp.obraz_wy = NULL;
    return 0;
}
