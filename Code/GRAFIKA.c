#include "debugmalloc.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdlib.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include "GRAFIKA.h"
#include "LISTA_PAKLI.h"
#include "MENTES.h"

/* kulon fuggvenybe, hogy olvashatobb legyen (inicializalas)*/
void sdl_init(int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow("BLACKJACK", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szeles, magas, 0);
    if (window == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(renderer);

    *pwindow = window;
    *prenderer = renderer;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//KÉPEKHEZ
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* egesz képet kirak a kepernyõ megadott helyere, meg kell adni hogy hova rakod és a méretét a képnek */
void egeszkepkirak(SDL_Renderer *renderer, SDL_Texture *kep, int x_hova, int y_hova, int hossz, int magas){
    SDL_Rect dest = {x_hova,y_hova,hossz,magas};
    SDL_RenderCopy(renderer,kep,NULL,&dest);
}
/* Kartyakat rajzolja ki, a képbol vágja ki a kartya sorszamanak segitsegevel */
void Kartya_rajzol(SDL_Renderer *renderer, SDL_Texture *Kartyak,int szam, int x_hova, int y_hova) {
    /* a forras kepbol ezekrol a koordinatakrol, ilyen meretu reszletet masolunk. */
    SDL_Rect src = { ((szam-1) % 13) * (1027/13), ((szam-1) / 13) * (615/5), 1027/13, 615/5 };
    /* a cel kepre, ezekre a koordinatakra masoljuk */
    SDL_Rect dest = { x_hova, y_hova, 132, 183 };
    /* kepreszlet masolasa */
    SDL_RenderCopy(renderer, Kartyak, &src, &dest);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FELIRATOKHOZ
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* feliratot rak közepre igazitva megadott helyre aminek van szines hattere, meglehet adni mit irjon ki és milyen szinut */
void shadedfelirat( SDL_Renderer *renderer, TTF_Font *betutipus, SDL_Color betuszin, SDL_Color hatterszin, char* szoveg, int x_hova, int y_hova){
    SDL_Surface *felirat;
    SDL_Texture *felirat_t;
    SDL_Rect hova = { 0, 0, 0, 0 };
    felirat = TTF_RenderUTF8_Shaded(betutipus, szoveg, betuszin, hatterszin);
    felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
    hova.x = x_hova;
    hova.y = y_hova;
    hova.w = felirat->w;
    hova.h = felirat->h;
    SDL_RenderCopy(renderer, felirat_t, NULL, &hova);
}
/* feliratot rak közepre igazitva megadott helyre aminek van szines hattere, meglehet adni mit irjon ki és milyen szinut */
void shadedfeliratkozep( SDL_Renderer *renderer, TTF_Font *betutipus, SDL_Color betuszin, SDL_Color hatterszin, char* szoveg, int x_hova, int y_hova){
    SDL_Surface *felirat;
    SDL_Texture *felirat_t;
    SDL_Rect hova = { 0, 0, 0, 0 };
    felirat = TTF_RenderUTF8_Shaded(betutipus, szoveg, betuszin, hatterszin);
    felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
    hova.x = (x_hova - felirat->w) / 2;
    hova.y = y_hova;
    hova.w = felirat->w;
    hova.h = felirat->h;
    SDL_RenderCopy(renderer, felirat_t, NULL, &hova);
}
/* sima feliratot rak a megadott helyre szinnel betutipussal */
void simafelirat( SDL_Renderer *renderer, TTF_Font *betutipus, SDL_Color betuszin, char* szoveg, int x_hova, int y_hova){
    SDL_Surface *felirat;
    SDL_Texture *felirat_t;
    SDL_Rect hova = { 0, 0, 0, 0 };
    felirat = TTF_RenderUTF8_Solid(betutipus, szoveg, betuszin);
    felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
    hova.x = x_hova;
    hova.y = y_hova;
    hova.w = felirat->w;
    hova.h = felirat->h;
    SDL_RenderCopy(renderer, felirat_t, NULL, &hova);
}
/* sima feliratot rak a megadott helyre középre igazitva szinnel betutipussal */
void simafeliratkozep( SDL_Renderer *renderer, TTF_Font *betutipus, SDL_Color betuszin, char* szoveg, int x_hova, int y_hova){
    SDL_Surface *felirat;
    SDL_Texture *felirat_t;
    SDL_Rect hova = { 0, 0, 0, 0 };
    felirat = TTF_RenderUTF8_Solid(betutipus, szoveg, betuszin);
    felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
    hova.x = (x_hova - felirat->w) / 2;
    hova.y = y_hova;
    hova.w = felirat->w;
    hova.h = felirat->h;
    SDL_RenderCopy(renderer, felirat_t, NULL, &hova);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//GOMBOK
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Megmondja gombot nyomtunk-e meg, ha igen akkor az ahhoz tartozó értéket megváltoztatja*/
void GOMBOK(Gomb* gombok, int gombokszama, int *Gombnyomott){

    bool click = false;

    while(!click){
        SDL_Event event;
        SDL_WaitEvent(&event);
        switch(event.type){
            case SDL_MOUSEBUTTONDOWN:
                for(int i = 0; i < gombokszama; i++){
                    if(event.button.x > gombok[i].Pozicio.x &&
                       event.button.x < gombok[i].Pozicio.x + gombok[i].Pozicio.w &&
                       event.button.y > gombok[i].Pozicio.y &&
                       event.button.y < gombok[i].Pozicio.y + gombok[i].Pozicio.h){
                        *Gombnyomott = gombok[i].sorszam;
                        click = true;
                    }
                }
                break;
            case SDL_QUIT:
                *Gombnyomott = 12;
                click = true;
                SDL_Quit();
                break;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//MENÜ KIRAJZOLÁSA
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int MENU_kirajzolasa(SDL_Renderer *renderer){

    //////////////////////////////////////////////////////////////
    //BLACKJACK kép
    //////////////////////////////////////////////////////////////

        /* kep betoltese a blackjack logohoz */
        SDL_Texture *blackjack = IMG_LoadTexture(renderer, "bj.png");
        if (blackjack == NULL) {
            SDL_Log("Nem nyithato meg a kepfajl: %s", IMG_GetError());
            exit(1);
        }
        /* kep a helyere */
        egeszkepkirak(renderer,blackjack,(1280-813)/2,200,813,116);
        /* nincs mar ra szukseg: felszabaditjuk a memoriat */
        SDL_DestroyTexture(blackjack);

    //////////////////////////////////////////////////////////////
    //narancs zseton
    //////////////////////////////////////////////////////////////

        /* kep betoltese narancs zsetonhoz */
        SDL_Texture *narancs = IMG_LoadTexture(renderer, "chipORANGE.png");
        if (narancs == NULL) {
            SDL_Log("Nem nyithato meg a kepfajl: %s", IMG_GetError());
            exit(1);
        }
        egeszkepkirak(renderer,narancs,(1280-131) / 4.5,400,100,100);
        SDL_DestroyTexture(narancs);

    //////////////////////////////////////////////////////////////
    //lila zseton
    //////////////////////////////////////////////////////////////

        /* kep betoltese a lila zsetonhoz*/
        SDL_Texture *lila = IMG_LoadTexture(renderer, "chipPURPLE.png");
        if (lila == NULL) {
            SDL_Log("Nem nyithato meg a kepfajl: %s", IMG_GetError());
            exit(1);
        }
        egeszkepkirak(renderer,lila,(1280-131) / 4 * 3.5 - 100,400,100,100);
        SDL_DestroyTexture(lila);

    //////////////////////////////////////////////////////////////
    //SZÖVEGEK inicializások
    //////////////////////////////////////////////////////////////

        /* szinek megadasa */
        SDL_Color fekete = {45,41,38}, szurke = {217,217,214};

        /* betutipusok betoltese, 50,32,20 pont magassaggal */
        TTF_Init();

        TTF_Font *font = TTF_OpenFont("betu.TTF", 50);
        if (!font) {
            SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
            exit(1);
        }

        TTF_Font *font2 = TTF_OpenFont("betu2.TTF", 32);
        if (!font2) {
            SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
            exit(1);
        }

        TTF_Font *font3 = TTF_OpenFont("betu3.TTF", 20);
        if (!font3) {
            SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
            exit(1);
        }

        SDL_Surface *felirat;

        /* feliratok megrajzolasa, kulonfele verziokban */

    //////////////////////////////////////////////////////////////
    //UJ JATEK
    //////////////////////////////////////////////////////////////

        shadedfeliratkozep(renderer,font,fekete,szurke,"ÚJ JÁTÉK",1280,400);

    //////////////////////////////////////////////////////////////
    //JATEK folytatasa
    //////////////////////////////////////////////////////////////

        shadedfeliratkozep(renderer,font,fekete,szurke,"JÁTÉK FOLYTATÁSA",1280,480);

    //////////////////////////////////////////////////////////////
    //KILÉPÉS
    //////////////////////////////////////////////////////////////

        shadedfeliratkozep(renderer,font,fekete,szurke,"KILÉPÉS",1280,480 + 80);

    //////////////////////////////////////////////////////////////
    //SZABALYOK, Forrasok
    //////////////////////////////////////////////////////////////

        shadedfelirat(renderer,font2,fekete,szurke,"Szabályok, források",20,650);

    //////////////////////////////////////////////////////////////
    //KeveB
    //////////////////////////////////////////////////////////////

    felirat = TTF_RenderUTF8_Solid(font3, "Készítette: KeveB", fekete);
    simafelirat(renderer,font3,fekete,"Készítette: KeveB",1280 - 20 - felirat->w,650+felirat->h);
    SDL_FreeSurface(felirat);

    /*Eddigiek kirajzolása*/
    SDL_RenderPresent(renderer);

    //////////////////////////////////////////////////////////////
    //GOMBOK
    //////////////////////////////////////////////////////////////

    Gomb MENU_gombok[4];
    felirat = TTF_RenderUTF8_Shaded(font, "ÚJ JÁTÉK", fekete,szurke);
    MENU_gombok[0].Pozicio = (SDL_Rect) {(1280 - felirat->w)/2,400,felirat->w,felirat->h};
    SDL_FreeSurface(felirat);
    MENU_gombok[0].sorszam = 1;

    felirat = TTF_RenderUTF8_Shaded(font, "JÁTÉK FOLYTATÁSA", fekete,szurke);
    MENU_gombok[1].Pozicio = (SDL_Rect) {(1280 - felirat->w)/2,480,felirat->w,felirat->h};
    SDL_FreeSurface(felirat);
    MENU_gombok[1].sorszam = 13;

    felirat = TTF_RenderUTF8_Shaded(font2, "Szabályok, források", fekete,szurke);
    MENU_gombok[2].Pozicio = (SDL_Rect) {20,650,felirat->w,felirat->h};
    SDL_FreeSurface(felirat);
    MENU_gombok[2].sorszam = 3;

    felirat = TTF_RenderUTF8_Shaded(font, "KILÉPÉS", fekete,szurke);
    MENU_gombok[3].Pozicio = (SDL_Rect) {(1280 - felirat->w)/2,480 + 80,felirat->w,felirat->h};
    SDL_FreeSurface(felirat);
    MENU_gombok[3].sorszam = 12;

    int Gombnyomott;
    GOMBOK(MENU_gombok,4,&Gombnyomott);

    //////////////////////////////////////////////////////////////
    //Bezárások
    //////////////////////////////////////////////////////////////

         /* nem kell tobbe */
        TTF_CloseFont(font3);
         /* nem kell tobbe */
        TTF_CloseFont(font2);
         /* nem kell tobbe */
        TTF_CloseFont(font);

        return Gombnyomott;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SZABÁLYOK KIRAJZOLÁSA
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int SZABALYOK_kirajzolasa(SDL_Renderer *renderer){

    //////////////////////////////////////////////////////////////
    //SZÖVEGEK inicializások
    //////////////////////////////////////////////////////////////

    /* szinek megadasa */
    SDL_Color fekete = {45,41,38}, szurke = {217,217,214};

    TTF_Init();

    TTF_Font *font = TTF_OpenFont("betu.TTF", 50);
    if (!font) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }
    TTF_Font *font2 = TTF_OpenFont("arial.TTF",20);
    if (!font2) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    SDL_Surface *felirat;

    //////////////////////////////////////////////////////////////
    //VISSZA A MENUBE
    //////////////////////////////////////////////////////////////

    felirat = TTF_RenderUTF8_Shaded(font, "VISSZA A MENÜBE", fekete,szurke);
    shadedfelirat(renderer,font,fekete,szurke,"VISSZA A MENÜBE",1280 - 20 - felirat->w,650);
    SDL_FreeSurface(felirat);

    //////////////////////////////////////////////////////////////
    //asd
    //////////////////////////////////////////////////////////////

    felirat = TTF_RenderUTF8_Shaded(font2,"Házszabályok:", fekete,szurke);
    shadedfelirat(renderer,font2,fekete,szurke,"HázSzabályok:",20,20);
    shadedfelirat(renderer,font2,fekete,szurke,"Ez egy egyszemélyes blackjack játék.",20,20 + 2 * felirat->h);
    shadedfelirat(renderer,font2,fekete,szurke,"Téttel lehet játszani, de végtelen zsetonod van, bármennyit lehet veszteni, de egyszerre csak 200 000 értékű zsetonnal lehet fogadni.",20,20 + 3 * felirat->h);
    shadedfelirat(renderer,font2,fekete,szurke,"Az osztó nem húz 16 pont fölött.",20,20 + 4 * felirat->h);
    shadedfelirat(renderer,font2,fekete,szurke,"A paklit minden körnél megkeveri a gép.",20,20 + 5 * felirat->h);
    shadedfelirat(renderer,font2,fekete,szurke,"Duplázni, kettéosztani nem lehet.",20,20 + 6 * felirat->h);
    shadedfelirat(renderer,font2,fekete,szurke,"Ász esetén automatikusan kiválasztja a program a jobb értéket.",20,20 + 7 * felirat->h);
    shadedfelirat(renderer,font2,fekete,szurke,"Források:",20,20 + 9 * felirat->h);
    shadedfelirat(renderer,font2,fekete,szurke,"A blackjack szabályait itt olvashatod: https://www.pokerstarscasino-gg.eu/games/blackjack/rules/?no_redirect=1 ",20,20 + 11 * felirat->h);
    shadedfelirat(renderer,font2,fekete,szurke,"Zsetonok képei: https://opengameart.org/content/casino-pack",20,20 + 12 * felirat->h);
    shadedfelirat(renderer,font2,fekete,szurke,"További segítségeket az Infoc-ről ,Tankörtársaktól és számtalan SDL2- vel foglalkozó portál oldaláról szereztem.",20,20 + 13 * felirat->h);

    simafelirat(renderer,font2,fekete,"TOP 3 EGYENLEG",20,400);

    char elso[12];
    sprintf(elso, "%d", LEGNAGYOBB());
    simafelirat(renderer,font2,fekete,elso,25,400+40);

    char masodik[12];
    sprintf(masodik, "%d", MASODIK(LEGNAGYOBB()));
    simafelirat(renderer,font2,fekete,masodik,25,400+80);

    char harmadik[12];
    sprintf(harmadik, "%d", HARMADIK(LEGNAGYOBB(),MASODIK(LEGNAGYOBB())));
    simafelirat(renderer,font2,fekete,harmadik,25,400+120);

    SDL_FreeSurface(felirat);

    /*EDDIGIEK KIRAJZOLÁSA A KÉPERNYŐRE*/
    SDL_RenderPresent(renderer);

    Gomb SZABALYOK_gombok[1];
    felirat = TTF_RenderUTF8_Shaded(font, "VISSZA A MENÜBE", fekete,szurke);
    SZABALYOK_gombok[0].Pozicio = (SDL_Rect) {1280 - 20 - felirat->w,650,felirat->w,felirat->h};
    SDL_FreeSurface(felirat);
    SZABALYOK_gombok[0].sorszam = 0;

    int Gombnyomott;
    GOMBOK(SZABALYOK_gombok,1,&Gombnyomott);

    TTF_CloseFont(font);
    TTF_CloseFont(font2);

    return Gombnyomott;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FOGADÁS KIRAJZOLÁSA
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int FOGADAS_kirajzolasa(SDL_Renderer *renderer, Jatekallas Jatekallas){

    //////////////////////////////////////////////////////////////
    //BLACKJACK kép
    //////////////////////////////////////////////////////////////

    /* kep betoltese logohoz*/
    SDL_Texture *blackjack = IMG_LoadTexture(renderer, "bj.png");
    if (blackjack == NULL) {
        SDL_Log("Nem nyithato meg a kepfajl: %s", IMG_GetError());
        exit(1);
    }
    egeszkepkirak(renderer,blackjack,(1280-813/2)/2,20,813/2,116/2);
    SDL_DestroyTexture(blackjack);

    //////////////////////////////////////////////////////////////
    //narancs zseton
    //////////////////////////////////////////////////////////////

    /* kep betoltese */
    SDL_Texture *narancs = IMG_LoadTexture(renderer, "chipORANGE.png");
    if (narancs == NULL) {
        SDL_Log("Nem nyithato meg a kepfajl: %s", IMG_GetError());
        exit(1);
    }
    egeszkepkirak(renderer,narancs,1280 / 2 - 300,720/2,150,150);
    SDL_DestroyTexture(narancs);

    //////////////////////////////////////////////////////////////
    //kék zseton
    //////////////////////////////////////////////////////////////

    /* kep betoltese */
    SDL_Texture *kek = IMG_LoadTexture(renderer, "chipBLUE.png");
    if (kek == NULL) {
        SDL_Log("Nem nyithato meg a kepfajl: %s", IMG_GetError());
        exit(1);
    }
    egeszkepkirak(renderer,kek,1280 / 2 - 150,720/2,150,150);
    SDL_DestroyTexture(kek);

    //////////////////////////////////////////////////////////////
    //zöld zseton
    //////////////////////////////////////////////////////////////

    /* kep betoltese */
    SDL_Texture *zold = IMG_LoadTexture(renderer, "chipGREEN.png");
    if (zold == NULL) {
        SDL_Log("Nem nyithato meg a kepfajl: %s", IMG_GetError());
        exit(1);
    }
    egeszkepkirak(renderer,zold,1280 / 2 ,720/2,150,150);
    SDL_DestroyTexture(zold);

    //////////////////////////////////////////////////////////////
    //lila zseton
    //////////////////////////////////////////////////////////////

    /* kep betoltese */
    SDL_Texture *lila = IMG_LoadTexture(renderer, "chipPURPLE.png");
    if (lila == NULL) {
        SDL_Log("Nem nyithato meg a kepfajl: %s", IMG_GetError());
        exit(1);
    }
    egeszkepkirak(renderer,lila,1280 / 2 +150,720/2,150,150);
    SDL_DestroyTexture(lila);

    //////////////////////////////////////////////////////////////
    //SZÖVEGEK inicializások
    //////////////////////////////////////////////////////////////

    /* szinek megadasa */
    SDL_Color fekete = {45,41,38}, szurke = {217,217,214};

    /* betutipusok betoltese, 50,30,30 pont magassaggal */
    TTF_Init();

    TTF_Font *font = TTF_OpenFont("betu.TTF", 50);
    if (!font) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    TTF_Font *font2 = TTF_OpenFont("arial.TTF", 30);
    if (!font2) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }
    TTF_Font *font3 = TTF_OpenFont("szam.TTF", 30);
    if (!font) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    SDL_Surface *felirat;

    /* feliratok megrajzolasa, kulonfele verziokban */

    //////////////////////////////////////////////////////////////
    //VISSZA A MENUBE
    //////////////////////////////////////////////////////////////

    shadedfelirat(renderer,font,fekete,szurke,"VISSZA A MENÜBE",20,650);
//
    //////////////////////////////////////////////////////////////
    //ALL IN
    //////////////////////////////////////////////////////////////

    felirat = TTF_RenderUTF8_Shaded(font, "ALL IN", fekete,szurke);
    shadedfeliratkozep(renderer,font,fekete,szurke,"ALL IN",1280,710/4+(felirat->h)*2);
    SDL_FreeSurface(felirat);

    //////////////////////////////////////////////////////////////
    //Clear
    //////////////////////////////////////////////////////////////

    felirat = TTF_RenderUTF8_Shaded(font, "CLEAR", fekete,szurke);
    shadedfeliratkozep(renderer,font,fekete,szurke,"CLEAR",1280,650);
    SDL_FreeSurface(felirat);
//
    //////////////////////////////////////////////////////////////
    //JATEK INDITASA
    //////////////////////////////////////////////////////////////

    felirat = TTF_RenderUTF8_Shaded(font, "JÁTÉK INDÍTÁSA", fekete,szurke);
    shadedfelirat(renderer,font,fekete,szurke,"JÁTÉK INDÍTÁSA",1280 - 20 - felirat->w,650);
    SDL_FreeSurface(felirat);

    //////////////////////////////////////////////////////////////
    //FOGADJ
    //////////////////////////////////////////////////////////////

    char Tetossz[12];
    sprintf(Tetossz, "%d k", Jatekallas.Tet/1000);

    simafeliratkozep(renderer,font2,fekete,"Fogadj! Eddigi tét:",1280,720/4);

    felirat = TTF_RenderUTF8_Solid(font2, "Fogadj! Eddigi tét::", fekete);
    simafeliratkozep(renderer,font2,fekete,Tetossz,1280,720/4+felirat->h);
    SDL_FreeSurface(felirat);

    //////////////////////////////////////////////////////////////
    //Egyenleg
    //////////////////////////////////////////////////////////////

    char Egyenlegsz[12];
    sprintf(Egyenlegsz, "%d k", Jatekallas.Egyenleg/1000);

    simafeliratkozep(renderer,font2,fekete,"Egyenleged:",1280,550);

    felirat = TTF_RenderUTF8_Solid(font2, "Egyenleged:", fekete);
    simafeliratkozep(renderer,font2,fekete,Egyenlegsz,1280,550+felirat->h);
    SDL_FreeSurface(felirat);

    //////////////////////////////////////////////////////////////
    //szamok a zsetonokon a zsetonokhoz képest középre
    //////////////////////////////////////////////////////////////

    felirat = TTF_RenderUTF8_Solid(font3, "10 000", fekete);
    simafelirat(renderer,font3,fekete,"10 000",(1280 - felirat->w) /2 - (150*1.5) ,720/2 + 175/3);
    SDL_FreeSurface(felirat);

    felirat = TTF_RenderUTF8_Solid(font3, "5 000", fekete);
    simafelirat(renderer,font3,fekete,"5 000",(1280 - felirat->w) /2 - (150*0.5) ,720/2 + 175/3);
    SDL_FreeSurface(felirat);

    felirat = TTF_RenderUTF8_Solid(font3, "2 000", fekete);
    simafelirat(renderer,font3,fekete,"2 000",(1280 - felirat->w) /2 + (150*0.5) ,720/2 + 175/3);
    SDL_FreeSurface(felirat);

    felirat = TTF_RenderUTF8_Solid(font3, "1 000", fekete);
    simafelirat(renderer,font3,fekete,"1 000",(1280 - felirat->w) /2 + (150*1.5) ,720/2 + 175/3);
    SDL_FreeSurface(felirat);

    /*EDDIGIEK KIRAJZOLÁSA A KÉPERNYŐRE*/
    SDL_RenderPresent(renderer);

    //////////////////////////////////////////////////////////////
    //GOMBOK
    //////////////////////////////////////////////////////////////

    Gomb FOGADAS_gombok[8];

    felirat = TTF_RenderUTF8_Shaded(font, "VISSZA A MENÜBE", fekete,szurke);
    FOGADAS_gombok[0].Pozicio = (SDL_Rect) {20,650,felirat->w,felirat->h};
    SDL_FreeSurface(felirat);
    FOGADAS_gombok[0].sorszam = 0;

    felirat = TTF_RenderUTF8_Shaded(font, "JÁTÉK INDÍTÁSA", fekete,szurke);
    FOGADAS_gombok[1].Pozicio = (SDL_Rect) {1280 - 20 - felirat->w,650,felirat->w,felirat->h};
    SDL_FreeSurface(felirat);
    FOGADAS_gombok[1].sorszam = 4;
//
    felirat = TTF_RenderUTF8_Shaded(font, "ALL IN", fekete,szurke); //(x_hova - felirat->w) / 2
    FOGADAS_gombok[6].Pozicio = (SDL_Rect) {(1280 - felirat->w)/2,710/4+(felirat->h)*2,felirat->w,felirat->h};
    SDL_FreeSurface(felirat);
    FOGADAS_gombok[6].sorszam = 14;

    felirat = TTF_RenderUTF8_Shaded(font, "CLEAR", fekete,szurke); //(x_hova - felirat->w) / 2
    FOGADAS_gombok[7].Pozicio = (SDL_Rect) {(1280 - felirat->w)/2,650,felirat->w,felirat->h};
    SDL_FreeSurface(felirat);
    FOGADAS_gombok[7].sorszam = 15;
//
    FOGADAS_gombok[2].Pozicio = (SDL_Rect) {1280 / 2 - 300,720/2,150,150};
    FOGADAS_gombok[2].sorszam = 5;

    FOGADAS_gombok[3].Pozicio = (SDL_Rect) {1280 / 2 - 150,720/2,150,150};
    FOGADAS_gombok[3].sorszam = 6;

    FOGADAS_gombok[4].Pozicio = (SDL_Rect) {1280 / 2 ,720/2,150,150};
    FOGADAS_gombok[4].sorszam = 7;

    FOGADAS_gombok[5].Pozicio = (SDL_Rect) {1280 / 2 +150,720/2,150,150};
    FOGADAS_gombok[5].sorszam = 8;

    int Gombnyomott;
    GOMBOK(FOGADAS_gombok,8,&Gombnyomott);

    //////////////////////////////////////////////////////////////
    //Bezárások
    //////////////////////////////////////////////////////////////

     /* nem kell tobbe */
    TTF_CloseFont(font3);
     /* nem kell tobbe */
    TTF_CloseFont(font2);
     /* nem kell tobbe */
    TTF_CloseFont(font);

    return Gombnyomott;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//JÁTÉK KIRAJZOLÁSA
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int JATEK_kirajzolasa(SDL_Renderer *renderer, Jatekallas Jatekallas, ListaKartya *pakli, char* Vegeredmeny){

    /*Pakli megkeveréséhez*/
    srand(time(NULL));

    //////////////////////////////////////////////////////////////
    //Lapok
    //////////////////////////////////////////////////////////////

    /* kep betoltese a kártyákhoz*/
    SDL_Texture *kartyak = IMG_LoadTexture(renderer, "Kartyak.png");
    if (kartyak == NULL) {
        SDL_Log("Nem nyithato meg a kepfajl: %s", IMG_GetError());
        exit(1);
    }

    /*VÁLTOZOK A PONTSZÁMOKHOZ*/
    int Osztop = 0;
    int Pontsz = 0;

    /*OSZTO ELSO KETTŐ LAPJA*/
    Kartya_rajzol(renderer,kartyak,lista_elem(pakli,1),(1280-132)/3 - 132/4 - 50,100);
    Osztop += erteke(lista_elem(pakli,1),Osztop);

    /*HÁTLAP CSAK HA NEM ÁLLT MÉG MEG*/
    if(!Jatekallas.megall)
        Kartya_rajzol(renderer,kartyak,53,(1280-132)/3 - 50,100 - 3);

    /*JÁTÉKOS ELSO KETTŐ LAPJA*/
    Kartya_rajzol(renderer,kartyak,lista_elem(pakli,3),(1280-132)/3 - 132/4 - 50,720 - 150 - 180);
    Pontsz += erteke(lista_elem(pakli,3),Pontsz);
    Kartya_rajzol(renderer,kartyak,lista_elem(pakli,4),(1280-132)/3 - 50,720 - 150 - 183);
    Pontsz += erteke(lista_elem(pakli,4),Pontsz);

    /*JÁTÉKOS TÖBBI LAPJA*/
    if(Pontsz <= 21){
        for(int i = 5; i <= Jatekallas.Lapokszama; i++){
            Kartya_rajzol(renderer,kartyak,lista_elem(pakli,i),(1280-132)/3 - 50 + (i - 4)*(132/4),720 - 150 - 183 - (i - 4)*3);
            Pontsz += erteke(lista_elem(pakli,i),Pontsz);
            if(Pontsz >= 21){
                Jatekallas.megall = true;
                break;
            }
        }
    }

    /*OSZTO TOBBI LAPJA*/
    if(Jatekallas.megall == true){

        /*MÁSODIK OSZTO LAP MEGFORDITASA*/
        Kartya_rajzol(renderer,kartyak,lista_elem(pakli,2),(1280-132)/3 - 50,100 - 3);
        Osztop += erteke(lista_elem(pakli,2),Osztop);
        /*TÖBBI OSZTO LAP*/
        if(Pontsz <= 21  && Osztop < 17){
            int i = 1;
            do{
                Kartya_rajzol(renderer,kartyak,lista_elem(pakli,Jatekallas.Lapokszama + Jatekallas.Osztolapok),(1280-132)/3 - 50 + i* (132/4),100 - 3 - i*3);
                Osztop += erteke(lista_elem(pakli,Jatekallas.Lapokszama + Jatekallas.Osztolapok),Osztop);
                Jatekallas.Osztolapok++;
                i++;
            }while(Osztop < 17);
        }
        /*VEGEREDMENY ELDONTESE*/
        if (Pontsz > 21) *Vegeredmeny = 'V';                                               /*printf("Nyertel, az oszto besokallt\n");*/
        if (Osztop > 21) *Vegeredmeny = 'N';                                               /*printf("Nyertel, az oszto besokallt\n");*/
        else if (Osztop == Pontsz) *Vegeredmeny = 'D';                                      /*printf("Dontetlen ugyanannyi a pontotok\n");*/
        else if (Osztop <= 21 && Pontsz <= 21){
            if (21 - Osztop < 21 - Pontsz) *Vegeredmeny = 'V';                              /*printf("Vesztettel, kozelebb van az oszto a 21-hez\n");*/
            else if (21 - Osztop > 21 - Pontsz) *Vegeredmeny = 'N';                         /*printf("Nyertel, kozelebb vagy a 21-hez az osztonal\n");*/
        }
    }

    /*PAKLI*/
    for(int i = 0; i < 15;i++){
        Kartya_rajzol(renderer,kartyak,53,1280/2 + 100 + i*3,(720-183)/2);
    }

    /* nincs mar ra szukseg: felszabaditjuk a memoriat */
    SDL_DestroyTexture(kartyak);

    //////////////////////////////////////////////////////////////
    //SZÖVEGEK inicializások
    //////////////////////////////////////////////////////////////

    /* szinek megadasa */
    SDL_Color fekete = {45,41,38}, szurke = {217,217,214}, zold = {0,200,0}, piros = {240,0,0};

    /* betutipusok betoltese, 50,32,20 pont magassaggal */
    TTF_Init();

    TTF_Font *font = TTF_OpenFont("betu.TTF", 50);
    if (!font) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    TTF_Font *font2 = TTF_OpenFont("arial.TTF", 20);
    if (!font2) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    TTF_Font *font3 = TTF_OpenFont("betu.TTF", 150);
    if (!font3) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    SDL_Surface *felirat;

    /* feliratok megrajzolasa, kulonfele verziokban */

    //////////////////////////////////////////////////////////////
    //VISSZA A MENUBE
    //////////////////////////////////////////////////////////////

    felirat = TTF_RenderUTF8_Shaded(font, "VISSZA A MENÜBE", fekete,szurke);
    shadedfelirat(renderer,font,fekete,szurke,"VISSZA A MENÜBE",1280 - 20 - felirat->w,650);
    SDL_FreeSurface(felirat);

    //////////////////////////////////////////////////////////////
    //MENTÉS, ha vege a jateknak
    //////////////////////////////////////////////////////////////

    if(*Vegeredmeny == 'V' || *Vegeredmeny == 'D' || *Vegeredmeny == 'N'){
        felirat = TTF_RenderUTF8_Shaded(font, "MENTÉS", fekete,szurke);
        shadedfelirat(renderer,font,fekete,szurke,"MENTÉS",1280 - 20 - felirat->w,20);
        SDL_FreeSurface(felirat);

    //////////////////////////////////////////////////////////////
    //JATEK FOLYTATASA, ha vege a jateknak
    //////////////////////////////////////////////////////////////

        felirat = TTF_RenderUTF8_Shaded(font, "FOLYTATÁS", fekete,szurke);
        shadedfelirat(renderer,font,fekete,szurke,"FOLYTATÁS",1280 - 20 - felirat->w,40 + felirat->h);
        SDL_FreeSurface(felirat);
    }

    //////////////////////////////////////////////////////////////
    //Húzás
    //////////////////////////////////////////////////////////////

    felirat = TTF_RenderUTF8_Shaded(font, " HÚZÁS ", fekete,zold);
    shadedfelirat(renderer,font,fekete,zold," HÚZÁS ",200,650);
    SDL_FreeSurface(felirat);

    //////////////////////////////////////////////////////////////
    //Megállás
    //////////////////////////////////////////////////////////////

    shadedfelirat(renderer,font,fekete,piros,"MEGÁLLÁS",450,650);

    //////////////////////////////////////////////////////////////
    //Pontszámod:
    //////////////////////////////////////////////////////////////

    char Pontszam[12];
    sprintf(Pontszam, "%d", Pontsz);

    felirat = TTF_RenderUTF8_Solid(font2, "Pontszámod: ", fekete);
    simafelirat(renderer,font2,fekete,"Pontszámod: ",(1280-felirat->w)/3 - 50,(720-felirat->h)/2);

    simafelirat(renderer,font2,fekete,Pontszam,(1280-felirat->w)/3 + felirat->w - 50,(720-felirat->h)/2);
    SDL_FreeSurface(felirat);

    //////////////////////////////////////////////////////////////
    //Osztó pontszáma:
    //////////////////////////////////////////////////////////////

    char Osztopontszam[12];
    sprintf(Osztopontszam, "%d", Osztop);

    felirat = TTF_RenderUTF8_Solid(font2, "Osztó pontszáma: ", fekete);
    simafelirat(renderer,font2,fekete,"Osztó pontszáma: ",(1280-felirat->w)/3 - 50,50);

    simafelirat(renderer,font2,fekete,Osztopontszam,(1280-felirat->w)/3 + felirat->w - 50,50);
    SDL_FreeSurface(felirat);

    //////////////////////////////////////////////////////////////
    //Egyenleg
    //////////////////////////////////////////////////////////////

    char Egyenlegsz[12];
    sprintf(Egyenlegsz, "%d k", Jatekallas.Egyenleg/1000);

    felirat = TTF_RenderUTF8_Solid(font2, "Egyenleg:", fekete);
    simafelirat(renderer,font2,fekete,"Egyenleg:",1280 - 20 - felirat->w,(720-felirat->h)/2 + 2 * felirat->h);
    SDL_FreeSurface(felirat);

    felirat = TTF_RenderUTF8_Solid(font2, Egyenlegsz, fekete);
    simafelirat(renderer,font2,fekete,Egyenlegsz,1280 - 20 - felirat->w,(720-felirat->w)/2 + 4 * felirat->h);
    SDL_FreeSurface(felirat);

    //////////////////////////////////////////////////////////////
    //Tét
    //////////////////////////////////////////////////////////////


    char Tetossz[12];
    sprintf(Tetossz, "%d k",Jatekallas.Tet/1000);

    felirat = TTF_RenderUTF8_Solid(font2, "Tét:", fekete);
    simafelirat(renderer,font2,fekete,"Tét:",1280 - 20 - felirat->w,(720-felirat->h)/2 - 3 * felirat->h);
    SDL_FreeSurface(felirat);

    felirat = TTF_RenderUTF8_Solid(font2, Tetossz, fekete);
    simafelirat(renderer,font2,fekete,Tetossz,1280 - 20 - felirat->w ,(720-felirat->h)/2 - 2 * felirat->h);
    SDL_FreeSurface(felirat);

    //////////////////////////////////////////////////////////////
    //VEGEREDMENY
    //////////////////////////////////////////////////////////////

    if(*Vegeredmeny == 'N'){
        if(Jatekallas.Tet == 69000){
        felirat = TTF_RenderUTF8_Shaded(font3, "AZ NAGY GG", fekete,szurke);
        shadedfelirat(renderer,font,fekete,szurke,"AZ NAGY GG",1280/2,200);
        SDL_FreeSurface(felirat);
        }
        felirat = TTF_RenderUTF8_Shaded(font3, "NYERTÉL", fekete,zold);
        shadedfelirat(renderer,font3,fekete,zold,"NYERTÉL",1280/2,20);
        SDL_FreeSurface(felirat);
    }
    else if(*Vegeredmeny == 'V'){
        felirat = TTF_RenderUTF8_Shaded(font3, "VESZTETTÉL", fekete,piros);
        shadedfelirat(renderer,font3,fekete,piros,"VESZTETTÉL",1280/2,20);
        SDL_FreeSurface(felirat);
    }
    else if(*Vegeredmeny == 'D'){
        felirat = TTF_RenderUTF8_Shaded(font3, "DÖNTETLEN", fekete,szurke);
        shadedfelirat(renderer,font3,fekete,szurke,"DÖNTETLEN",1280/2,20);
        SDL_FreeSurface(felirat);
    }

    /*EDDIGIEK KIRAJZOLÁSA A KÉPERNYŐRE*/
    SDL_RenderPresent(renderer);

    //////////////////////////////////////////////////////////////
    //GOMBOK
    //////////////////////////////////////////////////////////////

    Gomb JATEK_gombok[5];

    felirat = TTF_RenderUTF8_Shaded(font, "VISSZA A MENÜBE", fekete,szurke);
    JATEK_gombok[0].Pozicio = (SDL_Rect) {1280 - 20 - felirat->w,650,felirat->w,felirat->h};
    SDL_FreeSurface(felirat);
    JATEK_gombok[0].sorszam = 0;

    if(*Vegeredmeny == 'V' || *Vegeredmeny == 'D' || *Vegeredmeny == 'N'){
        felirat = TTF_RenderUTF8_Shaded(font, "MENTÉS", fekete,szurke);
        JATEK_gombok[3].Pozicio = (SDL_Rect) {1280 - 20 - felirat->w,20,felirat->w,felirat->h};
        SDL_FreeSurface(felirat);
        JATEK_gombok[3].sorszam = 9;

        felirat = TTF_RenderUTF8_Shaded(font, "FOLYTATÁS", fekete,szurke);
        JATEK_gombok[4].Pozicio = (SDL_Rect) {1280 - 20 - felirat->w,40 + felirat->h,felirat->w,felirat->h};
        SDL_FreeSurface(felirat);
        JATEK_gombok[4].sorszam = 2;

    }else{
        JATEK_gombok[3].Pozicio = (SDL_Rect) {0,0,0,0};
        JATEK_gombok[4].Pozicio = (SDL_Rect) {0,0,0,0};
    }

    felirat = TTF_RenderUTF8_Shaded(font, " HÚZÁS ", fekete,szurke);
    JATEK_gombok[1].Pozicio = (SDL_Rect) {200,650,felirat->w,felirat->h};
    SDL_FreeSurface(felirat);
    JATEK_gombok[1].sorszam = 10;

    felirat = TTF_RenderUTF8_Shaded(font, "MEGÁLLÁS", fekete,szurke);
    JATEK_gombok[2].Pozicio = (SDL_Rect) {450,650,felirat->w,felirat->h};
    SDL_FreeSurface(felirat);
    JATEK_gombok[2].sorszam = 11;

    int Gombnyomott;

    GOMBOK(JATEK_gombok,5,&Gombnyomott);

    //////////////////////////////////////////////////////////////
    //Bezárások
    //////////////////////////////////////////////////////////////

     /* nem kell tobbe */
    TTF_CloseFont(font3);
    TTF_CloseFont(font2);
    TTF_CloseFont(font);

    return Gombnyomott;
}
