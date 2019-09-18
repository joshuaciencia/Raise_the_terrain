#ifndef _HOLBERTON
#define _HOLBERTON

#include <SDL2/SDL.h>

#define T_W 112
#define T_H 56
#define TILES 7
#define WIDTH 800
#define HEIGHT 600

typedef struct SDL_Instance
{
	SDL_Window *window;
	SDL_Renderer *renderer;
} SDL_Instance;

int init_instance(SDL_Instance *);
void draw_stuff(SDL_Instance *, int **);
int **parse_terrain(const char*);
#endif
