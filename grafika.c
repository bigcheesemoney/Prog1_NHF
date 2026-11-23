#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "sakk.h"
#include "debugmalloc.h"

int mezo_meret = 64;
int tabla_szel_x = 224;
int tabla_szel_y = 48;

/*SDL-t inicializáló függvény:
Inicializálja a könyvtárat, létrehoz egy ablakot a megadott méretek szerint, és létrehozza a renderert*/
void sdl_init(int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow("Sakk", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szeles, magas, 0);
    if (window == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }

    *pwindow = window;
    *prenderer = renderer;
}
/*Szöveg megjelenítõ függvény:
A paraméterként kapott rendererrel, megjeleníti az adott színû, betûtipusú szöveget a kívánt koordinátákon.
Az SDL TTF alkönyvtárát használja.*/
void ttf_write(SDL_Renderer *renderer, TTF_Font *font, char *szoveg, int r, int g, int b, int x, int y){

    SDL_Surface *felirat;
    SDL_Texture *felirat_t;
    SDL_Rect hova = {0};
    SDL_Color szin = {r, g, b};

    felirat = TTF_RenderUTF8_Blended(font, szoveg, szin);
    felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
    hova.x = x;
    hova.y = y;
    hova.w = felirat->w;
    hova.h = felirat->h;
    SDL_RenderCopy(renderer, felirat_t, NULL, &hova);
    SDL_FreeSurface(felirat);
    SDL_DestroyTexture(felirat_t);
}
/*Sakktáblá alaphelyzetbe állító függvény:
Alaphelyzetbe állítja a táblát a kapott rendererrel.*/
void draw_board(SDL_Renderer *renderer){


    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    for (int sor = 0; sor < 8; sor++) {
        for (int oszlop = 0; oszlop < 8; oszlop++) {

            int x1 = tabla_szel_x + (oszlop * mezo_meret);
            int y1 = tabla_szel_y + (sor * mezo_meret);
            int x2 = x1 + mezo_meret - 1;
            int y2 = y1 + mezo_meret - 1;

            if ((sor + oszlop) % 2 == 0) {
                /* Világos mezo */
                boxRGBA(renderer, x1, y1, x2, y2, 240, 217, 181, 255);
            } else {
                /* Sötét mezo */
                boxRGBA(renderer, x1, y1, x2, y2, 181, 136, 99, 255);
            }
        }
    }

}
/*Megállapitja, melyik mezore kattintott a játékos*/
Mezo melyik_mezo(int x, int y, Tabla tabla){
    Mezo valasztott;
    int oszlop = (x - tabla_szel_x)/64;
    int sor = (y - tabla_szel_y)/64;

    valasztott.SOR = sor;
    valasztott.OSZLOP = oszlop;

    valasztott.BABU = tabla.mezok[sor][oszlop];

    return valasztott;
}

void pozicio_frissit(Tabla tabla, SDL_Renderer *renderer, TTF_Font *font){
    draw_board(renderer);
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            switch(tabla.mezok[i][j]){
                case 1:
                    ttf_write(renderer, font, "♟", 255, 255, 255, tabla_szel_x + j * mezo_meret, tabla_szel_y + i * mezo_meret);
                    break;
                case 2:
                    ttf_write(renderer, font, "♝", 255, 255, 255, tabla_szel_x + j * mezo_meret, tabla_szel_y + i * mezo_meret);
                    break;
                case 3:
                    ttf_write(renderer, font, "♞", 255, 255, 255, tabla_szel_x + j * mezo_meret, tabla_szel_y + i * mezo_meret);
                    break;
                case 4:
                    ttf_write(renderer, font, "♜", 255, 255, 255, tabla_szel_x + j * mezo_meret, tabla_szel_y + i * mezo_meret);
                    break;
                case 5:
                    ttf_write(renderer, font, "♛", 255, 255, 255, tabla_szel_x + j * mezo_meret, tabla_szel_y + i * mezo_meret);
                    break;
                case 6:
                    ttf_write(renderer, font, "♚", 255, 255, 255, tabla_szel_x + j * mezo_meret, tabla_szel_y + i * mezo_meret);
                    break;
                case 7:
                    ttf_write(renderer, font, "♟", 0, 0, 0, tabla_szel_x + j * mezo_meret, tabla_szel_y + i * mezo_meret);
                    break;
                case 8:
                    ttf_write(renderer, font, "♝", 0, 0, 0, tabla_szel_x + j * mezo_meret, tabla_szel_y + i * mezo_meret);
                    break;
                case 9:
                    ttf_write(renderer, font, "♞", 0, 0, 0, tabla_szel_x + j * mezo_meret, tabla_szel_y + i * mezo_meret);
                    break;
                case 10:
                    ttf_write(renderer, font, "♜", 0, 0, 0, tabla_szel_x + j * mezo_meret, tabla_szel_y + i * mezo_meret);
                    break;
                case 11:
                    ttf_write(renderer, font, "♛", 0, 0, 0, tabla_szel_x + j * mezo_meret, tabla_szel_y + i * mezo_meret);
                    break;
                case 12:
                    ttf_write(renderer, font, "♚", 0, 0, 0, tabla_szel_x + j * mezo_meret, tabla_szel_y + i * mezo_meret);
                    break;
                default:
                    continue;
            }
        }
    }
}

void lepes_kezel(Tabla *tabla, Lepes lepes){
    tabla->mezok[lepes.HOVA.SOR][lepes.HOVA.OSZLOP] = tabla->mezok[lepes.HONNAN.SOR][lepes.HONNAN.OSZLOP];
    tabla->mezok[lepes.HONNAN.SOR][lepes.HONNAN.OSZLOP] = 0;
}
