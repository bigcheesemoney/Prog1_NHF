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
    int turn = 1;
    bool clicked = false;
    Lepes lepes;
    SDL_Event event;
    while (SDL_WaitEvent(&event) && event.type != SDL_QUIT){
        if (matte(tabla, V_KIRALY) || matte(tabla, S_KIRALY)){
            SDL_Quit();
            printf("\nMatt! A jateknak vege.");
        }
        else if(patte(tabla, V_KIRALY) || patte(tabla, S_KIRALY)){
            SDL_Quit();
            printf("\nPatt! A jateknak vege.");
        }
            switch (event.type){
                case SDL_MOUSEBUTTONDOWN:
                    if (clicked && turn % 2 == 0 && lepes.HONNAN.BABU > V_KIRALY){
                        lepes.HOVA = melyik_mezo(event.button.x, event.button.y, tabla);
                        Tabla masolat = tabla;
                        lepes_kezel(&masolat, lepes);
                        if (!sakke(masolat, S_KIRALY) && szabalyose(lepes, tabla)){
                            lepes_kezel(&tabla, lepes);
                            promocio(&tabla, lepes);
                            turn++;
                            clicked = false;
                        }
                        else{
                            clicked = false;
                        }
                    }
                    else if (clicked &&turn % 2 != 0 && lepes.HONNAN.BABU < S_GYALOG && lepes.HONNAN.BABU != URES){
                        lepes.HOVA = melyik_mezo(event.button.x, event.button.y, tabla);
                        Tabla masolat = tabla;
                        lepes_kezel(&masolat, lepes);
                        if (!sakke(masolat, V_KIRALY) && szabalyose(lepes, tabla)){
                            lepes_kezel(&tabla, lepes);
                            promocio(&tabla, lepes);
                            turn++;
                            clicked = false;
                        }
                        else{
                            clicked = false;
                        }
                    }
                    else{
                        lepes.HONNAN = melyik_mezo(event.button.x, event.button.y, tabla);
                        clicked = true;
                    }
                    break;
            }
            pozicio_frissit(tabla, renderer, font);
            SDL_RenderPresent(renderer);
    }
    SDL_Quit();
    return 0;
}
