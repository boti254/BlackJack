#include "debugmalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include "MENTES.h"

/*txt alj�ra rakja a mentett egyenleget*/
void BEIRAS(int Egyenleg){
    FILE* Mentes;
    Mentes = fopen("mentes.txt", "a");
    if(Mentes != NULL){
        fprintf(Mentes,"%d\n",Egyenleg);
        fclose(Mentes);
    }
}
/*a txt alj�r�l kiolvassa az utols� elmentett intet*/
int MENTES_KIOLVASAS(){
    FILE* Mentesr;
    Mentesr = fopen("mentes.txt", "r");
    int elozo;
    int uj;
    if(Mentesr != NULL){
        while(fscanf(Mentesr,"%d",&uj)==1){
            elozo = uj;
        }
        fclose(Mentesr);
        return elozo;
    }else{
        return -1;
    }
}
/*Kiolvassa a legnagyobb mentett �rt�ket*/
int LEGNAGYOBB(){
    FILE* Mentesr;
    Mentesr = fopen("mentes.txt", "r");
    int elozo;
    int uj;
    int max;
    if(Mentesr != NULL){
        fscanf(Mentesr,"%d",&uj);
        if(uj < 0) max = 0;
        else max = uj;
        while(fscanf(Mentesr,"%d",&uj)==1){
            elozo = uj;
            if(elozo > max)
                max = elozo;
        }
        fclose(Mentesr);
        return max;
    }else{
        return 0;
    }
}
/*Kiolvassa a m�sodik legnagyobb mentett �rt�ket*/
int MASODIK(int max){
    FILE* Mentesr;
    Mentesr = fopen("mentes.txt", "r");
    int elozo;
    int uj;
    int max2 = 0;
    if(Mentesr != NULL){
        while(fscanf(Mentesr,"%d",&uj)==1){
            elozo = uj;
            if(elozo > max2 && elozo != max)
                max2 = elozo;
        }
        fclose(Mentesr);
        return max2;
    }else{
        return 0;
    }
}
/*Kiolvassa a harmadik legnagyobb mentett �rt�ket*/
int HARMADIK(int max,int max2){
    FILE* Mentesr;
    Mentesr = fopen("mentes.txt", "r");
    int elozo;
    int uj;
    int max3 = 0;
    if(Mentesr != NULL){
        while(fscanf(Mentesr,"%d",&uj)==1){
            elozo = uj;
            if((elozo > max3) && (elozo < max2) && (elozo < max))
                max3 = elozo;
        }
        fclose(Mentesr);
        return max3;
    }else{
        return 0;
    }
}
