#include "struct.h"

// Prototypy wszystkich funkcji
void negatyw(t_obraz *, w_opcje *);
void progowanie(t_obraz *, w_opcje *);
void p_czern(t_obraz *, w_opcje *);
void p_biel(t_obraz *, w_opcje *);
void kontur(t_obraz *, w_opcje *);
int czytaj(t_obraz *, w_opcje *);
int pisz(t_obraz *, w_opcje *);
void wyswietl(char *);
int przetwarzaj_opcje(int, char **, w_opcje *);
void wyzeruj_opcje(w_opcje *);
void przetw_kolor(t_obraz *, w_opcje *);
void drugi_pierwszy(t_obraz *);
