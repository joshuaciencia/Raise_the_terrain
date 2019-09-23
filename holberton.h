#ifndef _HOLBERTON
#define _HOLBERTON

#include <SDL2/SDL.h>
#include <math.h>

#define T_W 128
#define T_H 64
#define TILES 7
#define WIDTH 1000
#define HEIGHT 800
#define ELEVATE 1
#define INTER_TIME 5000
#define DELTA 0.03
/**
 * struct SDL_Instance - sdl instance
 * @window: sdl window
 * @renderer: sdl renderer
 */
typedef struct SDL_Instance
{
	SDL_Window *window;
	SDL_Renderer *renderer;
} SDL_Instance;

int init_instance(SDL_Instance *);
void draw_stuff(SDL_Instance *, int **, double, double *);
int **parse_terrain(const char *);
void to_iso(SDL_Point *);
void translate(SDL_Point*, int, int);
double get_angle(int, int);
void rotate(SDL_Point*, double);
void draw_line(SDL_Instance *, SDL_Point*, SDL_Point*);
int lerp(double, int, int);
#endif
