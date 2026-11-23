#ifndef GRAFIKA_H
#define GRAFIKA_H
#include "sakk.h"
#include <stdbool.h>


void sdl_init(int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer);

void ttf_write(SDL_Renderer *renderer, TTF_Font *font, char *szoveg, int r, int g, int b, int x, int y);

void draw_board(SDL_Renderer *renderer);

Mezo melyik_mezo(int x, int y, Tabla tabla);

void pozicio_frissit(Tabla tabla, SDL_Renderer *renderer, TTF_Font *font);

void lepes_kezel(Tabla *tabla, Lepes lepes);
#endif
