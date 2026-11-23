#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <stdbool.h>
#include "grafika.h"
#include "sakk.h"
#include "debugmalloc.h"

int main(int argc, char *argv[]){
    /*Váltotók deklarálása*/
    char *betutipus = "DejaVuSans.ttf";
    int betumeret = 64;
    int ablak_meret[2] = {960, 640};
    Tabla tabla = {0};
    init_tabla(&tabla);
    Mezo mezo = melyik_mezo(479, 559, tabla);

    /*SDL konfigurálása*/
    SDL_Window *window;
    SDL_Renderer *renderer;
    sdl_init(ablak_meret[0], ablak_meret[1], &window, &renderer);

    /*TTF konfigurálása*/
    TTF_Init();
    TTF_Font *font = TTF_OpenFont(betutipus, betumeret);
    if (!font) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }
    /*Teszt*/
    for(int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++)
            printf("%d ", tabla.mezok[i][j]);
        printf("\n");
    }
    printf("%d %d %d", mezo.SOR, mezo.OSZLOP, mezo.BABU);

    /*Game loop*/
    bool white_clicked = false;
    bool black_clicked = false;
    int turn = 1;
    Lepes lepes;
    SDL_Event event;
    while (SDL_WaitEvent(&event) && event.type != SDL_QUIT){
            switch (event.type){
                case SDL_MOUSEBUTTONDOWN:
                    if (white_clicked && !black_clicked){
                        lepes.HOVA = melyik_mezo(event.button.x, event.button.y, tabla);
                        if (szabalyose(lepes, tabla) && lepes.HONNAN.BABU < 7){
                            lepes_kezel(&tabla, lepes);
                            turn++;
                            white_clicked = false;
                        }
                        else{
                            white_clicked = false;
                        }
                    }
                    else if (turn % 2 != 0){
                        lepes.HONNAN = melyik_mezo(event.button.x, event.button.y, tabla);
                        white_clicked = true;
                    }
                    if (black_clicked && !white_clicked){
                        lepes.HOVA = melyik_mezo(event.button.x, event.button.y, tabla);
                        if (szabalyose(lepes, tabla) && lepes.HONNAN.BABU > 7){
                            lepes_kezel(&tabla, lepes);
                            turn++;
                            black_clicked = false;
                        }
                        else{
                            black_clicked = false;
                        }
                    }
                    else if (turn % 2 == 0){
                        lepes.HONNAN = melyik_mezo(event.button.x, event.button.y, tabla);
                        black_clicked = true;
                    }
            }
            pozicio_frissit(tabla, renderer, font);
            SDL_RenderPresent(renderer);
    }
    SDL_Quit();
    return 0;
}
