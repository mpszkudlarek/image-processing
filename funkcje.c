#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "header.h"

#define DL_LINII 1024
#define MAX 512
#define W_OK 0                /* wartosc oznaczajaca brak bledow */
#define B_NIEPOPRAWNAOPCJA -1 /* kody bledow rozpoznawania opcji */
#define B_BRAKNAZWY -2
#define B_BRAKWARTOSCI -3
#define B_BRAKPLIKU -4

/********************************************************************/
/*                                                                  */
/* ALTERNATYWNA DO PRZEDSTAWIONEJ NA WYKLADZIE WERSJA OPRACOWANIA   */
/* PARAMETROW WYWOLANIA PROGRAMU UWZGLEDNIAJACA OPCJE Z PARAMETRAMI */
/* Z ODPOWIEDNIO ZAPROPONOWANYMI STRUKTURAMI DANYCH PRZEKAZUJACYMI  */
/* WCZYTANE USTAWIENIA                                              */
/*                                    COPYRIGHT (c) 2007-2020 KCiR  */
/*                                                                  */
/* Autorzy udzielaja kazdemu prawa do kopiowania tego programu      */
/* w calosci lub czesci i wykorzystania go w dowolnym celu, pod     */
/* warunkiem zacytowania zrodla                                     */
/*                                                                  */
/********************************************************************/

void wyzeruj_opcje(w_opcje *wybor)
{
    wybor->plik_we = NULL;
    wybor->plik_wy = NULL;
    wybor->negatyw = 0;
    wybor->konturowanie = 0;
    wybor->progowanie = 0;
    wybor->wyswietlenie = 0;
    wybor->prog_biel = 0;
    wybor->prog_czern = 0;
    wybor->w_progu = 0;
    wybor->czynnik = 0;
}
int przetwarzaj_opcje(int argc, char **argv, w_opcje *wybor)
{
    int i;
    char *nazwa_pliku_we;
    wyzeruj_opcje(wybor);
    wybor->plik_wy = stdout; /* w wypadku braku podania opcji : "-o", ustawienie standardowego strumienia wyjscia stdout*/

    for (i = 1; i < argc; i++)
    {
        if (argv[i][0] != '-') /*  komunikat o bledzie, brak znaku "-", a co za tym idzie niepoprawnie wybrana opcja*/
            return B_NIEPOPRAWNAOPCJA;
        switch (argv[i][1])
        {
        case 'i':
        { /* opcja z nazwa pliku wejsciowego */
            if (++i < argc)
            { /* wczytanie kolejnego argumenty jako nazwy pliku */
                nazwa_pliku_we = argv[i];
                if (strcmp(nazwa_pliku_we, "-") == 0) /* gdy nazwa jest "-" */
                    wybor->plik_we = stdin;           /* ustwiamy wejscie na standardowy strumien wejscia stdin */
                else                                  /* otwieramy wskazany plik */
                    wybor->plik_we = fopen(nazwa_pliku_we, "r");
            }
            else
                return B_BRAKNAZWY; /* blad: brak nazwy pliku */
            break;
        }
        case 'o':
        { /* opcja z nazwa pliku wyjsciowego */
            if (++i < argc)
            { /* wczytanie kolejnego argumenty jako nazwy pliku */
                wybor->nazwa_pliku_wy = argv[i];
                if (strcmp(wybor->nazwa_pliku_wy, "-") == 0) /* gdy nazwa jest "-" */
                    wybor->plik_wy = stdout;                 /* ustwiamy wyjscie na standardowy strumien wyjscia stdout */
                else                                         /* otwieramy wskazany plik */
                    wybor->plik_wy = fopen(wybor->nazwa_pliku_wy, "w");
            }
            else
                return B_BRAKNAZWY; /* blad: brak nazwy pliku */
            break;
        }
        case 'p':
        { /* funkcja progowania : do wyboru przez uzytkownika :  progowanei o zadanej wartosci -p <prog>, progowanie bieli -pb <prog> lub progowanie czerni -pc <prog> */
            int prog;
            if (argv[i][2] == 'b')
            {
                wybor->prog_biel = 1;
            }
            else if (argv[i][2] == 'c')
            {
                wybor->prog_czern = 1;
            }
            else if (!argv[i][2])
                wybor->progowanie = 1;
            else
                return B_NIEPOPRAWNAOPCJA;
            if (++i < argc)
            { /* wczytujemy kolejny argument jako wartosc progu */
                if (sscanf(argv[i], "%d", &prog) == 1)
                {
                    wybor->w_progu = prog;
                }
                else
                    return B_BRAKWARTOSCI; /* komunikat o bledzie: niepoprawna wartosc progu */
            }
            else
                return B_BRAKWARTOSCI; /* komunikat o bledzie: brak wartosci progu */
        }
        break;
        case 'm':
        { /* wybor koloru do przetwarzania, do wyboru : r – czerwony, g – zielony, b – niebieski, s – konwersja do szarości */
            char jaki;
            if (++i < argc)
            { /* wczytujemy kolejny argument jako wartosc progu */
                if (sscanf(argv[i], "%s", &jaki) == 1)
                {
                    wybor->kolor = 1;
                    if (jaki == 'r')
                    {
                        wybor->jaki_kolor = 0;
                    }
                    else if (jaki == 'g')
                    {
                        wybor->jaki_kolor = 1;
                    }
                    else if (jaki == 'b')
                    {
                        wybor->jaki_kolor = 2;
                    }
                    else if (jaki == 's')
                    {
                        wybor->jaki_kolor = 4;
                    }
                    else
                        return B_NIEPOPRAWNAOPCJA;
                }
                else
                    return B_BRAKWARTOSCI; /* blad: niepoprawna wartosc progu */
            }
        }
        break;
        case 'n':
        { /* mamy wykonac negatyw */
            wybor->negatyw = 1;
        }
        break;
        case 'k':
        { /* mamy wykonac konturowanie */
            wybor->konturowanie = 1;
        }
        break;
        case 'd':
        { /* mamy wyswietlic obraz */
            wybor->wyswietlenie = 1;
        }
        break;
        default: /* nierozpoznana opcja */
            return B_NIEPOPRAWNAOPCJA;
        }                       /*koniec switch */
    }                           /* koniec for */
    if (wybor->plik_we != NULL) /* poprawnie zainicjowany wejsciowy strumien danych */
        return W_OK;
    else
        return B_BRAKPLIKU; /* komunikat o bledzie, nie udalo sie otworzyc pliku wejsciowego */
}
void przetw_kolor(t_obraz *temp, w_opcje *wybor)
{
    int(*pierwszy)[temp->wymx * 3];
    pierwszy = (int(*)[temp->wymx * 3]) temp->obraz_we;
    int(*drugi)[temp->wymx * 3];
    drugi = (int(*)[temp->wymx * 3]) temp->obraz_wy;
    if (wybor->jaki_kolor == 4)
    {
        for (int i = 0; i < temp->wymy; i++)
        {
            for (int j = 0; j < temp->wymx * 3; j++)
            {
                drugi[i][j] = (pierwszy[i][(int)(j / 3) * 3] + pierwszy[i][(int)(j / 3) * 3 + 1] + pierwszy[i][(int)(j / 3) * 3 + 2]) / 3;
            }
        } // do szarosci
    }
}
void drugi_pierwszy(t_obraz *temp)
{
    if (temp->typ == 2)
    {
        int(*pierwszy)[temp->wymx];
        pierwszy = (int(*)[temp->wymx])temp->obraz_we;
        int(*drugi)[temp->wymx];
        drugi = (int(*)[temp->wymx])temp->obraz_wy;
        for (int i = 0; i < temp->wymy; i++)
        {
            for (int j = 0; j < temp->wymx; j++)
            {
                pierwszy[i][j] = drugi[i][j];
            }
        }
        pierwszy = NULL;
        drugi = NULL;
    }
    else if (temp->typ == 3)
    {
        int(*pierwszy)[temp->wymx * 3];
        pierwszy = (int(*)[temp->wymx * 3]) temp->obraz_we;
        int(*drugi)[temp->wymx * 3];
        drugi = (int(*)[temp->wymx * 3]) temp->obraz_wy;
        for (int i = 0; i < temp->wymy; i++)
        {
            for (int j = 0; j < temp->wymx * 3; j++)
            {
                pierwszy[i][j] = drugi[i][j];
            }
        }
        pierwszy = NULL;
        drugi = NULL;
    }
} // Przepisanie obrazu wejsciowego na wyjsciowy
void negatyw(t_obraz *temp, w_opcje *wybor)
{
    if (temp->typ == 2)
    {
        int(*pierwszy)[temp->wymx];
        pierwszy = (int(*)[temp->wymx])temp->obraz_we;
        int(*drugi)[temp->wymx];
        drugi = (int(*)[temp->wymx])temp->obraz_wy;
        for (int i = 0; i < temp->wymy; i++)
        {
            for (int j = 0; j < temp->wymx; j++)
            {
                drugi[i][j] = abs((temp->odcieni) - pierwszy[i][j]);
            }
        }
    }
    else if (temp->typ == 3)
    {
        int(*pierwszy)[temp->wymx * 3];
        pierwszy = (int(*)[temp->wymx * 3]) temp->obraz_we;
        int(*drugi)[temp->wymx * 3];
        drugi = (int(*)[temp->wymx * 3]) temp->obraz_wy;
        if (wybor->jaki_kolor != 4)
        {
            for (int i = 0; i < temp->wymy; i++)
            {
                for (int j = 0; j < temp->wymx * 3; j++)
                {
                    if (j % 3 == wybor->jaki_kolor)
                        drugi[i][j] = abs((temp->odcieni) - pierwszy[i][j]);
                }
            }
        }
        else
        {
            for (int i = 0; i < temp->wymy; i++)
            {
                for (int j = 0; j < temp->wymx * 3; j++)
                {
                    drugi[i][j] = abs((temp->odcieni) - pierwszy[i][j]);
                }
            }
        }
    }
    drugi_pierwszy(temp);
}
void progowanie(t_obraz *temp, w_opcje *wybor)
{
    if (temp->typ == 2)
    {
        int(*pierwszy)[temp->wymx];
        pierwszy = (int(*)[temp->wymx])temp->obraz_we;
        int(*drugi)[temp->wymx];
        drugi = (int(*)[temp->wymx])temp->obraz_wy;
        for (int i = 0; i < temp->wymy; i++)
        {
            for (int j = 0; j < temp->wymx; j++)
            {
                if (pierwszy[i][j] <= (wybor->w_progu))
                    drugi[i][j] = 0;
                else
                    drugi[i][j] = temp->odcieni;
            }
        }
    }
    else if (temp->typ == 3)
    {
        int(*pierwszy)[temp->wymx * 3];
        pierwszy = (int(*)[temp->wymx * 3]) temp->obraz_we;
        int(*drugi)[temp->wymx * 3];
        drugi = (int(*)[temp->wymx * 3]) temp->obraz_wy;
        if (wybor->jaki_kolor != 4)
        {
            for (int i = 0; i < temp->wymy; i++)
            {
                for (int j = 0; j < temp->wymx * 3; j++)
                {
                    if (j % 3 == wybor->jaki_kolor)
                    {
                        if (pierwszy[i][j] <= (wybor->w_progu))
                            drugi[i][j] = 0;
                        else
                            drugi[i][j] = temp->odcieni;
                    }
                }
            }
        }
        else
        {
            for (int i = 0; i < temp->wymy; i++)
            {
                for (int j = 0; j < temp->wymx * 3; j++)
                {
                    if (pierwszy[i][j] <= (wybor->w_progu))
                        drugi[i][j] = 0;
                    else
                        drugi[i][j] = temp->odcieni;
                }
            }
        }
    }
    drugi_pierwszy(temp);
}
void p_czern(t_obraz *temp, w_opcje *wybor)
{
    if (temp->typ == 2)
    {
        int(*pierwszy)[temp->wymx];
        pierwszy = (int(*)[temp->wymx])temp->obraz_we;
        int(*drugi)[temp->wymx];
        drugi = (int(*)[temp->wymx])temp->obraz_wy;
        for (int i = 0; i < temp->wymy; i++)
        {
            for (int j = 0; j < temp->wymx; j++)
            {
                if (pierwszy[i][j] <= (wybor->w_progu))
                    drugi[i][j] = 0;
            }
        }
    }
    else if (temp->typ == 3)
    {
        int(*pierwszy)[temp->wymx * 3];
        pierwszy = (int(*)[temp->wymx * 3]) temp->obraz_we;
        int(*drugi)[temp->wymx * 3];
        drugi = (int(*)[temp->wymx * 3]) temp->obraz_wy;
        if (wybor->jaki_kolor != 4)
        {
            for (int i = 0; i < temp->wymy; i++)
            {
                for (int j = 0; j < temp->wymx * 3; j++)
                {
                    if (pierwszy[i][j] <= (wybor->w_progu))
                        drugi[i][j] = 0;
                }
            }
        }
        else
        {
            for (int i = 0; i < temp->wymy; i++)
            {
                for (int j = 0; j < temp->wymx * 3; j++)
                {
                    if (j % 3 == wybor->jaki_kolor)
                    {
                        if (pierwszy[i][j] <= (wybor->w_progu))
                            drugi[i][j] = 0;
                    }
                }
            }
        }
    }
    drugi_pierwszy(temp);
}
void p_biel(t_obraz *temp, w_opcje *wybor)
{
    if (temp->typ == 2)
    {
        int(*pierwszy)[temp->wymx];
        pierwszy = (int(*)[temp->wymx])temp->obraz_we;
        int(*drugi)[temp->wymx];
        drugi = (int(*)[temp->wymx])temp->obraz_wy;
        for (int i = 0; i < temp->wymy; i++)
        {
            for (int j = 0; j < temp->wymx; j++)
            {
                if ((pierwszy[i][j]) >= (wybor->w_progu))
                    drugi[i][j] = temp->odcieni;
            }
        }
    }
    else if (temp->typ == 3)
    {
        int(*pierwszy)[temp->wymx * 3];
        pierwszy = (int(*)[temp->wymx * 3]) temp->obraz_we;
        int(*drugi)[temp->wymx * 3];
        drugi = (int(*)[temp->wymx * 3]) temp->obraz_wy;
        if (wybor->jaki_kolor != 4)
        {
            for (int i = 0; i < temp->wymy; i++)
            {
                for (int j = 0; j < temp->wymx * 3; j++)
                {
                    if (j % 3 == wybor->jaki_kolor)
                    {
                        if ((pierwszy[i][j]) >= (wybor->w_progu))
                            drugi[i][j] = temp->odcieni;
                    }
                }
            }
        }
        else
        {
            for (int i = 0; i < temp->wymy; i++)
            {
                for (int j = 0; j < temp->wymx * 3; j++)
                {
                    if ((pierwszy[i][j]) >= (wybor->w_progu))
                        drugi[i][j] = temp->odcieni;
                }
            }
        }
    }
    drugi_pierwszy(temp);
}
void kontur(t_obraz *temp, w_opcje *wybor)
{
    if (temp->typ == 2)
    {
        int(*pierwszy)[temp->wymx];
        pierwszy = (int(*)[temp->wymx])temp->obraz_we;
        int(*drugi)[temp->wymx];
        drugi = (int(*)[temp->wymx])temp->obraz_wy;
        for (int i = 0; i + 1 < temp->wymy; i++)
        {
            for (int j = 0; j + 1 < temp->wymx; j++)
            {
                drugi[i][j] = abs(pierwszy[i + 1][j] - pierwszy[i][j]) + abs(pierwszy[i][j + 1] - pierwszy[i][j]);
            }
        }
    }
    else if (temp->typ == 3)
    {
        int(*pierwszy)[temp->wymx * 3];
        pierwszy = (int(*)[temp->wymx * 3]) temp->obraz_we;
        int(*drugi)[temp->wymx * 3];
        drugi = (int(*)[temp->wymx * 3]) temp->obraz_wy;
        if (wybor->jaki_kolor != 4)
        {
            for (int i = 0; i < temp->wymy; i++)
            {
                for (int j = 0; j + 3 < temp->wymx * 3; j++)
                {
                    if (j % 3 == wybor->jaki_kolor)
                    {
                        drugi[i][j] = abs(pierwszy[i + 1][j] - pierwszy[i][j]) + abs(pierwszy[i][j + 3] - pierwszy[i][j]);
                    }
                }
            }
        }
        else
        {
            for (int i = 0; i < temp->wymy; i++)
            {
                for (int j = 0; j + 3 < temp->wymx * 3; j++)
                {
                    drugi[i][j] = abs(pierwszy[i + 1][j] - pierwszy[i][j]) + abs(pierwszy[i][j + 3] - pierwszy[i][j]);
                }
            }
        }
    }
    drugi_pierwszy(temp);
}
int pisz(t_obraz *temp, w_opcje *wybor)
{

    if (temp->typ == 2)
    {
        fprintf(wybor->plik_wy, "P2\n");
        fprintf(wybor->plik_wy, "%d %d\n%d\n", temp->wymx, temp->wymy, temp->odcieni);
        int(*drugi)[temp->wymx];
        drugi = (int(*)[temp->wymx])temp->obraz_wy;
        for (int i = 0; i < temp->wymy; i++)
        {
            for (int j = 0; j < temp->wymx; j++)
            {
                fprintf(wybor->plik_wy, "%d ", (drugi[i][j]));
            }
        }
    }
    else if (temp->typ == 3)
    {
        fprintf(wybor->plik_wy, "P3\n");
        fprintf(wybor->plik_wy, "%d %d\n%d\n", temp->wymx, temp->wymy, temp->odcieni);
        int(*drugi)[temp->wymx * 3];
        drugi = (int(*)[temp->wymx * 3]) temp->obraz_wy;
        for (int i = 0; i < temp->wymy; i++)
        {
            for (int j = 0; j < temp->wymx * 3; j++)
            {
                fprintf(wybor->plik_wy, "%d ", (drugi[i][j]));
            }
        }
    }
    else
        return B_BRAKWARTOSCI;
    return 0;
}
int czytaj(t_obraz *temp, w_opcje *wybor)
{
    char buf[DL_LINII]; /* bufor pomocniczy do czytania naglowka i komentarzy */
    int znak;           /* zmienna pomocnicza do czytania komentarzy */
    int koniec = 0;     /* czy napotkano koniec danych w pliku */
    int i, j;

    /*Sprawdzenie czy podano prawidlowy uchwyt pliku */
    if (wybor->plik_we == NULL)
    {
        fprintf(stderr, "Blad: Nie podano uchwytu do pliku.\n");
        return (0);
    }

    /* Sprawdzenie "numeru magicznego" - powinien byc P2/P3 */
    if (fgets(buf, DL_LINII, wybor->plik_we) == NULL) /* Wczytanie pierwszej linii pliku do bufora */
        koniec = 1;                                   /* Nie udalo sie? Koniec danych! */

    if ((buf[0] != 'P') || ((buf[1] != '2') && (buf[1] != '3')) || koniec)
    { /* Czy jest magiczne "P2" / "P3"? */
        fprintf(stderr, "Blad: To nie jest plik PGM ani PPM.\n");
        return (0);
    }
    else if (buf[1] == '2')
        temp->typ = 2;
    else if (buf[1] == '3')
        temp->typ = 3;
    /* Pominiecie komentarzy */
    do
    {
        if ((znak = fgetc(wybor->plik_we)) == '#')
        {                                                     /* Czy linia rozpoczyna sie od znaku '#'? */
            if (fgets(buf, DL_LINII, wybor->plik_we) == NULL) /* Przeczytaj ja do bufora                */
                koniec = 1;                                   /* Zapamietaj ewentualny koniec danych */
        }
        else
        {
            ungetc(znak, wybor->plik_we); /* Gdy przeczytany znak z poczatku linii */
        }                                 /* nie jest '#' zwroc go                 */
    } while (znak == '#' && !koniec);     /* Powtarzaj dopoki sa linie komentarza */
    /* i nie nastapil koniec danych  */

    /* Pobranie wymiarow obrazu i liczby odcieni szarosci */
    if (fscanf(wybor->plik_we, "%d %d %d", &temp->wymx, &temp->wymy, &temp->odcieni) != 3)
    {
        fprintf(stderr, "Blad: Brak wymiarow obrazu lub liczby stopni odcieni.\n");
        return (0);
    }
    if (temp->typ == 2) // dla pgm
    {
        temp->obraz_we = malloc(temp->wymx * temp->wymy * sizeof(int));
        temp->obraz_wy = malloc(temp->wymx * temp->wymy * sizeof(int));
        int(*pierwszy)[temp->wymx];
        pierwszy = (int(*)[temp->wymx])temp->obraz_we;
        int(*drugi)[temp->wymx];
        drugi = (int(*)[temp->wymx])temp->obraz_wy;
        for (i = 0; i < temp->wymy; i++)
        {
            for (j = 0; j < temp->wymx; j++)
            {
                if (fscanf(wybor->plik_we, "%d", &(pierwszy[i][j])) != 1)
                {
                    fprintf(stderr, "Blad: Niewlasciwe wymiary obrazu.\n");
                    return (0);
                }
                else
                {
                    drugi[i][j] = pierwszy[i][j];
                }
            }
        }
    }
    else // dla ppm
    {
        temp->obraz_we = malloc(temp->wymx * temp->wymy * sizeof(int) * 3);
        temp->obraz_wy = malloc(temp->wymx * temp->wymy * sizeof(int) * 3);
        int(*pierwszy)[temp->wymx * 3];
        pierwszy = (int(*)[temp->wymx * 3]) temp->obraz_we;
        int(*drugi)[temp->wymx * 3];
        drugi = (int(*)[temp->wymx * 3]) temp->obraz_wy;
        /* Pobranie obrazu i zapisanie w tablicy temp*/
        for (i = 0; i < temp->wymy; i++)
        {
            for (j = 0; j < temp->wymx * 3; j++)
            {
                if (fscanf(wybor->plik_we, "%d", &(pierwszy[i][j])) != 1)
                {
                    fprintf(stderr, "Blad: Niewlasciwe wymiary obrazu.\n");
                    return (0);
                }
                else
                {
                    drugi[i][j] = pierwszy[i][j];
                }
            }
        }
    }
    return 1;
}
void wyswietl(char *n_pliku)
{
    char polecenie[DL_LINII]; /* Pomocniczny bufor sluzacy do zestawiania polecen */

    strcpy(polecenie, "display "); /* konstrukcja polecenia postaci */
    strcat(polecenie, n_pliku);    /* display "nazwa_pliku" & */
    strcat(polecenie, " &");
    printf("%s\n", polecenie); /* kontrolny wydruk polecenia */
    system(polecenie);         /* wykonanie polecenia */
}
