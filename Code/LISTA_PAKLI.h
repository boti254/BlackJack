#ifndef LISTA_PAKLI_H_INCLUDED
#define LISTA_PAKLI_H_INCLUDED

#include <stdbool.h>

typedef struct ListaKartya{
    int adat;
    struct ListaKartya *kov;
}ListaKartya;

typedef struct Jatekallas{
    int Lapokszama;
    int Osztolapok;
    bool megall;
    int Egyenleg;
    int Tet;
}Jatekallas;

int lista_elem(ListaKartya *eleje,int szam);
ListaKartya* listafeltolt(void);
ListaKartya *lista_holvanbenne(ListaKartya *eleje,int szam);
ListaKartya *listaelemcserel(ListaKartya *eleje,ListaKartya *ezzel);
void shuffle(ListaKartya *eleje);
void Lista_felszabadit(ListaKartya *eleje);
int erteke(int szam,int Pontsz);


#endif // LISTA_PAKLI_H_INCLUDED
