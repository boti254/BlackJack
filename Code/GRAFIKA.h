#ifndef GRAFIKA_H_INCLUDED
#define GRAFIKA_H_INCLUDED
    #include <SDL2/SDL.h>
    #include <SDL2/SDL2_gfxPrimitives.h>
    #include <math.h>
    #include <stdlib.h>
    #include <SDL2/SDL_ttf.h>
    #include <time.h>
    #include <stdio.h>
    #include <SDL2/SDL_image.h>
    #include "LISTA_PAKLI.h"

typedef struct Gomb{
    SDL_Rect Pozicio;
    int sorszam;
}Gomb;
void GOMBOK(Gomb* gombok, int gombokszama, int *Gombnyomott);

void sdl_init(int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer);

void egeszkepkirak(SDL_Renderer *renderer, SDL_Texture *kep, int x_hova, int y_hova, int hossz, int magas);
void Kartya_rajzol(SDL_Renderer *renderer, SDL_Texture *Kartyak,int szam, int x_hova, int y_hova);

void shadedfelirat( SDL_Renderer *renderer, TTF_Font *betutipus, SDL_Color betuszin, SDL_Color hatterszin, char* szoveg, int x_hova, int y_hova);
void shadedfeliratkozep( SDL_Renderer *renderer, TTF_Font *betutipus, SDL_Color betuszin, SDL_Color hatterszin, char* szoveg, int x_hova, int y_hova);
void simafelirat( SDL_Renderer *renderer, TTF_Font *betutipus, SDL_Color betuszin, char* szoveg, int x_hova, int y_hova);
void simafeliratkozep( SDL_Renderer *renderer, TTF_Font *betutipus, SDL_Color betuszin, char* szoveg, int x_hova, int y_hova);

int MENU_kirajzolasa(SDL_Renderer *renderer);
int SZABALYOK_kirajzolasa(SDL_Renderer *renderer);
int FOGADAS_kirajzolasa(SDL_Renderer *renderer, Jatekallas Jatekallas);
int JATEK_kirajzolasa(SDL_Renderer *renderer, Jatekallas Jatekallas, ListaKartya *pakli, char* Vegeredmeny);

#endif // GRAFIKA_H_INCLUDED
