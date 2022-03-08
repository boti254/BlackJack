#include "debugmalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Lista_Pakli.h"

/* Feltölti a listát 1-52 ig*/
ListaKartya* listafeltolt(void){
    ListaKartya *eleje = NULL;
    eleje = (ListaKartya*) malloc(sizeof(ListaKartya));
    eleje->adat = 52;
    eleje->kov = NULL;
    for(int i = 52; i > 0; i--){
        ListaKartya *uj = NULL;
        uj = (ListaKartya*) malloc(sizeof(ListaKartya));
        uj->kov = eleje;
        uj->adat = i;
        eleje = uj;
    }
    return eleje;
}

/* Megkeres egy szamot, hol van a Listaban visszatér egy arra mutató pointerrel */
ListaKartya *lista_holvanbenne(ListaKartya *eleje,int szam){
    ListaKartya *mozgo;
    for (mozgo = eleje; mozgo != NULL; mozgo = mozgo->kov){
        if (mozgo->adat == szam)
            return mozgo;
    }
    return NULL;
}

/*Megkeresi a listában a megadott sorszamon milyen ertek van*/
int lista_elem(ListaKartya *eleje,int szam){
    ListaKartya *mozgo;
    int i = 1;
    for (mozgo = eleje; i < szam; mozgo = mozgo->kov){
        i++;
    }
    return mozgo->adat;
}

/* Kicsereli az egyik elemmel az elsőt */
ListaKartya *listaelemcserel(ListaKartya *eleje,ListaKartya *ezzel){
    ListaKartya *uj = NULL;
    uj = (ListaKartya*) malloc(sizeof(ListaKartya));
    uj->adat = eleje->adat;
    eleje->adat = ezzel->adat;
    ezzel->adat = uj->adat;
    free(uj);
    return eleje;
}

/* Megkeveri a listát */
void shuffle(ListaKartya *eleje){
    for (int i = 0; i < 1000; i++)
        eleje = listaelemcserel(eleje,lista_holvanbenne(eleje,rand()%52+1));
}

/* Felszabaditja a Lista elemeit */
void Lista_felszabadit(ListaKartya *eleje){
    ListaKartya *iter;
    while (eleje != NULL){
        iter = eleje;
        eleje = eleje->kov;
        free(iter);
    }
}

/* ELdönti egy 1-52 közötti számról hogy mennyi az értéke*/
int erteke(int szam,int Pontsz){
    /*szín nem fontos ezért csinalhatom */
    szam = szam % 13;
    switch (szam) {
        case 1:
            return Pontsz + 11 <= 21 ? 11 : 1;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            return szam;
        case 11:
        case 12:
        case 0:
            return 10;
        default:
            return -1;
        }
}
