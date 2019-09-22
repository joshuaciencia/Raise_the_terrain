#include "holberton.h"
#include <math.h>

int main(int argc, char* argv[])
{
	SDL_Instance instance;
	SDL_bool running = SDL_TRUE;
	int **terrain;
	double ang = 0;

	if (init_instance(&instance) != 0)
		return (1);

	/* read terrain from file */

	terrain = parse_terrain(argv[1]);

	while (running)
	{
		SDL_SetRenderDrawColor(instance.renderer, 0, 0, 0, 0);
		SDL_RenderClear(instance.renderer);
		/* poll events */
		SDL_Event event;
		SDL_KeyboardEvent key;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					running = SDL_FALSE;
				case SDL_KEYDOWN:
					key = event.key;
					if (key.keysym.scancode == SDL_SCANCODE_ESCAPE)
						running = SDL_FALSE;
					if (key.keysym.scancode == SDL_SCANCODE_LEFT)
						ang -= DELTA;
					if (key.keysym.scancode == SDL_SCANCODE_RIGHT)
						ang += DELTA;
					break;
			}
			
		}
		/* render graphics */
		draw_stuff(&instance, terrain, ang);
		/* display graphics */
		SDL_RenderPresent(instance.renderer);
	}
	SDL_DestroyRenderer(instance.renderer);
	SDL_DestroyWindow(instance.window);
	SDL_Quit();
	return (0);
}

int init_instance (SDL_Instance *instance)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		fprintf(stderr, "Error %s\n", SDL_GetError());
		return (1);
	}
	instance->window = SDL_CreateWindow("Raise the terrain by Joshua Hernandez",
		       	SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);

	if (instance->window == NULL)
	{
		fprintf(stderr, "Error %s\n", SDL_GetError());
		SDL_Quit();
		return (1);
	}

	instance->renderer = SDL_CreateRenderer(instance->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (instance->renderer == NULL)
	{
		SDL_DestroyWindow(instance->window);
		fprintf(stderr, "Error %s\n", SDL_GetError());
		SDL_Quit();
		return (1);
	}
	return (0);
}
/* draw grid */
void draw_stuff(SDL_Instance *instance, int ** terrain, double angle)
{
	int start_x = WIDTH / 2;
	int y, x, tiles = 0;
	int start_y = HEIGHT / 2;

	/* calculate tiles based on the file */
	while (terrain[tiles])
	{
		tiles++;
	}
	start_y -= tiles * T_H / 2;
	/* draw the lines with white color */
	SDL_SetRenderDrawColor(instance->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	/* draw lines */
	for(y = 0; y < tiles - 1; y++)
	{
		for (x = 0; x < tiles - 1; x++)
		{
			SDL_Point tile = {x, y};

			SDL_Point top_r, bottom_l, bottom_r;

			to_iso(&tile);
			translate(&tile, start_x, start_y);

			top_r = (SDL_Point){tile.x, tile.y};
			bottom_r = (SDL_Point){tile.x, tile.y};
			bottom_l = (SDL_Point){tile.x, tile.y};

			translate(&top_r, T_W / 2, T_H / 2);
			translate(&bottom_r, 0, T_H);
			translate(&bottom_l, -T_W / 2, T_H / 2);

			rotate(&tile, angle);
			rotate(&top_r, angle);
			rotate(&bottom_r, angle);
			rotate(&bottom_l, angle);

			if (ELEVATE)
			{
				translate(&tile, 0, -terrain[y][x]);
				translate(&top_r, 0, -terrain[y][x + 1]);
				translate(&bottom_r, 0, -terrain[y + 1][x + 1]);
				translate(&bottom_l, 0, -terrain[y + 1][x]);
			}

			/* draw tile */
			draw_line(instance, &tile, &top_r);
			draw_line(instance, &top_r, &bottom_r);
			draw_line(instance, &bottom_r, &bottom_l);
			draw_line(instance, &bottom_l, &tile);
		}
	}
} 

void to_iso(SDL_Point *point)
{
	int x = point->x;
	int y = point->y;
	point->y = (x + y) * T_H / 2;
	point->x = (x - y) * T_W / 2;
}
void translate(SDL_Point * point, int x, int y)
{
	point->x += x;
	point->y += y;
}

void rotate(SDL_Point *point, double angle)
{
	int x = point->x;
	int y = point->y;
	int len;
	double ang;

	/* fist translate to origin */

	x -= WIDTH / 2;
	y -= HEIGHT / 2;

	ang = get_angle(x, y) + angle;
	len = sqrt(x * x + y * y);
	point->x = len * cos(ang);
	point->y = len * sin(ang);

	/* translate back to center */

	point->x += WIDTH / 2;
	point->y += HEIGHT / 2;
}

double get_angle(int x, int y)
{
	return (atan2(y, x));
}

void draw_line(SDL_Instance *instance, SDL_Point *a, SDL_Point *b)
{
	SDL_RenderDrawLine(instance->renderer, a->x, a->y, b->x, b->y);
}

