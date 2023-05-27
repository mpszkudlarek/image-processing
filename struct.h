/*Struktury wykorzystywane w programie */
typedef struct
{
    int wymx, wymy, odcieni; /* parametry przetwarzanego obrazu */
    int typ;                 /* Potrzebne do przetwarzania rozszerzen .ppm i .pgm, czyli do tak zwanych ,,magicznych numerów", kolejno 2 dla P2, czyli rozszerzenie .pgm i 3 dla P3, czyli rozszerzenie .ppm */
    void *obraz_we;          /* wskaznik na obraz wejsciowy */
    void *obraz_wy;          /* wskaznik na obraz wyjsciowy */
} t_obraz;                   /* nazwa struktury */

typedef struct
{
    FILE *plik_we, *plik_wy; /*uchwyt kolejno do pliku wejsciowego i wyjsciowego obrazu */
    int negatyw, progowanie, prog_czern, prog_biel, konturowanie, wyswietlenie; /*dostepne opcje */
    int w_progu, kolor;                                                         /* zmienne potrzebne do poszczegolnych funkcji */
    float czynnik;                                                              /*wartosc progu potrzeby w funkcji progowania */
    int jaki_kolor;                                                             /* wartosci potrzebne do edycji koloru, kolejno: 0-r(czerwony), 1-g(zielony), 2-b(niebieski), 4-do szarosci */
    char *nazwa_pliku_wy;                                                       /* wskaznik potrzebny do poprawnego nazwania obrazu po zakonczeniu jego edycji */
} w_opcje;
