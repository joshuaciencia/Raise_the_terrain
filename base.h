#ifndef _BASE
#define _BASE

#include <SDL2/SDL.h>
#include <math.h>

#define T_S 100
#define INCLINATION 0.7
#define TILES 7
#define WIDTH 1000
#define HEIGHT 800
#define ELEVATE 0
#define INTER_TIME 5000
#define DELTA 0.0001
/**
 * struct SDL_Instance - sdl instance
 * @window: sdl window
 * @renderer: sdl renderer
 */
typedef struct SDL_Instancie
{
	SDL_Window *window;
	SDL_Renderer *renderer;
} SDL_Instance;

int init_instance(SDL_Instance *);
void draw_stuff(SDL_Instance *, double [8][8][3], double *);
int **parse_terrain(const char *);
void draw_line(SDL_Instance *, SDL_Point*, SDL_Point*);
SDL_Point *world_to_screen(double, double, double);
void rotate_point(double *, double *, double);
#endif
