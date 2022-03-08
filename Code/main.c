#include "debugmalloc.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdlib.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>
#include <stdio.h>
#include "GRAFIKA.h"
#include "LISTA_PAKLI.h"
#include <stdbool.h>
#include "MENTES.h"

int main(int argc, char *argv[]) {

    ///////////////////////////////////////////////////////////
    //SDL INICIALIZÁLÁSOK [GRAFIKÁHOZ]
    ///////////////////////////////////////////////////////////

    SDL_Window *window;
    SDL_Renderer *renderer;
    /*1280x720 ablak*/
    sdl_init(1280, 720, &window, &renderer);
    /*Fehér Háttér*/
    boxRGBA(renderer, 0, 0, 1279, 719, 0xFF, 0xFF, 0xFF, 0xFF);

    /*MINDIG MÁS KÁRTYÁK LEGYENEK UJRAINDITÁSKOR*/
    srand(time(NULL));

    ///////////////////////////////////////////////////////////
    //KIRAJZOLÁS, PAKLI
    ///////////////////////////////////////////////////////////

    int Gombnyomott = 0;

    ListaKartya *pakli = NULL;
    pakli = listafeltolt();

    Jatekallas Jatekallas;
    Jatekallas.Lapokszama = 4;
    Jatekallas.megall = false;
    char Vegeredmeny = 'X';

    Gombnyomott = MENU_kirajzolasa(renderer);
    SDL_Event event;
    while (SDL_WaitEvent(&event) && event.type != SDL_QUIT) {
        switch(Gombnyomott){
            case 0:
                SDL_RenderClear(renderer);
                Gombnyomott = MENU_kirajzolasa(renderer);
                break;
            case 1:
                Jatekallas.Egyenleg = 0;
                Jatekallas.Tet = 0;
                Jatekallas.Lapokszama = 4;
                Jatekallas.Osztolapok = 0;
                Jatekallas.megall = false;
                Vegeredmeny = 'X';
                shuffle(pakli);

                SDL_RenderClear(renderer);
                Gombnyomott = FOGADAS_kirajzolasa(renderer,Jatekallas);
                break;

            case 2:
                if(Vegeredmeny == 'N'){
                    Jatekallas.Egyenleg += Jatekallas.Tet;
                }
                else if(Vegeredmeny == 'V'){
                    Jatekallas.Egyenleg -= Jatekallas.Tet;
                }
                Jatekallas.Tet = 0;
                Jatekallas.Lapokszama = 4;
                Jatekallas.Osztolapok = 0;
                Jatekallas.megall = false;
                Vegeredmeny = 'X';
                shuffle(pakli);

                SDL_RenderClear(renderer);
                Gombnyomott = FOGADAS_kirajzolasa(renderer,Jatekallas);
                break;

            case 3:
                SDL_RenderClear(renderer);
                Gombnyomott = SZABALYOK_kirajzolasa(renderer);
                break;

            case 4:
                SDL_RenderClear(renderer);
                Gombnyomott = JATEK_kirajzolasa(renderer,Jatekallas,pakli,&Vegeredmeny);
                break;

            case 5:
                SDL_RenderClear(renderer);
                Jatekallas.Tet += 10000;
                if(Jatekallas.Tet > 200000)
                    Jatekallas.Tet = 200000;
                Gombnyomott = FOGADAS_kirajzolasa(renderer,Jatekallas);
                break;

            case 6:
                SDL_RenderClear(renderer);
                Jatekallas.Tet += 5000;
                if(Jatekallas.Tet > 200000)
                    Jatekallas.Tet = 200000;
                Gombnyomott = FOGADAS_kirajzolasa(renderer,Jatekallas);
                break;

            case 7:
                SDL_RenderClear(renderer);
                Jatekallas.Tet += 2000;
                if(Jatekallas.Tet > 200000)
                    Jatekallas.Tet = 200000;
                Gombnyomott = FOGADAS_kirajzolasa(renderer,Jatekallas);
                break;

            case 14:
                SDL_RenderClear(renderer);
                Jatekallas.Tet = 200000;
                Gombnyomott = FOGADAS_kirajzolasa(renderer,Jatekallas);
                break;

            case 15:
                SDL_RenderClear(renderer);
                Jatekallas.Tet = 0;
                Gombnyomott = FOGADAS_kirajzolasa(renderer,Jatekallas);
                break;

            case 8:
                SDL_RenderClear(renderer);
                Jatekallas.Tet += 1000;
                if(Jatekallas.Tet > 200000)
                    Jatekallas.Tet = 200000;
                Gombnyomott = FOGADAS_kirajzolasa(renderer,Jatekallas);
                break;

            case 9:
                SDL_RenderClear(renderer);
                if(Vegeredmeny == 'N'){
                    Jatekallas.Egyenleg += Jatekallas.Tet;
                }
                else if(Vegeredmeny == 'V'){
                    Jatekallas.Egyenleg -= Jatekallas.Tet;
                }
                BEIRAS(Jatekallas.Egyenleg);
                Jatekallas.Tet = 0;
                Jatekallas.Lapokszama = 4;
                Jatekallas.Osztolapok = 0;
                Jatekallas.megall = false;
                Vegeredmeny = 'X';
                shuffle(pakli);

                Gombnyomott = FOGADAS_kirajzolasa(renderer,Jatekallas);
                break;

            case 10:
                if(!Jatekallas.megall){
                    SDL_RenderClear(renderer);
                    Jatekallas.Lapokszama += 1;
                    Gombnyomott = JATEK_kirajzolasa(renderer,Jatekallas,pakli,&Vegeredmeny);
                }else{
                    SDL_RenderClear(renderer);
                    Gombnyomott = JATEK_kirajzolasa(renderer,Jatekallas,pakli,&Vegeredmeny);
                }
               break;

            case 11:
                if(!Jatekallas.megall){
                    Jatekallas.Osztolapok += 1;
                    Jatekallas.megall = true;
                    SDL_RenderClear(renderer);
                    Gombnyomott = JATEK_kirajzolasa(renderer,Jatekallas,pakli,&Vegeredmeny);
                }else{
                    SDL_RenderClear(renderer);
                    Gombnyomott = JATEK_kirajzolasa(renderer,Jatekallas,pakli,&Vegeredmeny);
                }
                break;

            case 12:
                SDL_Quit();
                break;

            case 13:
                SDL_RenderClear(renderer);
                Jatekallas.Egyenleg = MENTES_KIOLVASAS();
                Jatekallas.Tet = 0;
                Jatekallas.Lapokszama = 4;
                Jatekallas.Osztolapok = 0;
                Jatekallas.megall = false;
                Vegeredmeny = 'X';
                shuffle(pakli);
                Gombnyomott = FOGADAS_kirajzolasa(renderer,Jatekallas);
                break;
        }
    }

    ///////////////////////////////////////////////////////////
    //BEZÁRÁS
    ///////////////////////////////////////////////////////////

    SDL_Quit();
    Lista_felszabadit(pakli);

    return 0;
}
