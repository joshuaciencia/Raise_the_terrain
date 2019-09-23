#include "holberton.h"

/**
 * main - Raise the terrain by Joshua Hernandez
 * @argc: 2
 * @argv: argv square matrix file as the elevation coords
 * Return: void
 */
int main(int argc, char *argv[])
{
	SDL_Instance instance;
	SDL_bool running = SDL_TRUE;
	int **terrain;
	double ang = 0, time = 0, last = (double)SDL_GetTicks();

	if (init_instance(&instance) != 0)
		return (1);
	terrain = parse_terrain(argv[1]);
	while (running)
	{
		SDL_SetRenderDrawColor(instance.renderer, 0, 0, 0, 0);
		SDL_RenderClear(instance.renderer);
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
			}
		}
		time += ((double)SDL_GetTicks() - last) / INTER_TIME;
		last = (double)SDL_GetTicks();
		if (time >= 1.0)
			time = 0;
		draw_stuff(&instance, terrain, ang, &time);
		SDL_RenderPresent(instance.renderer);
	}
	SDL_DestroyRenderer(instance.renderer);
	SDL_DestroyWindow(instance.window);
	SDL_Quit();
	return (0);
}
/**
 * init_instance - initialize SDL
 * @instance: windows and renderer instance
 * Return: void
 */
int init_instance(SDL_Instance *instance)
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

	instance->renderer = SDL_CreateRenderer(instance->window, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (instance->renderer == NULL)
	{
		SDL_DestroyWindow(instance->window);
		fprintf(stderr, "Error %s\n", SDL_GetError());
		SDL_Quit();
		return (1);
	}
	return (0);
}
/**
 * draw_stuff - draws the terrain
 * @instance: sdl instance
 * @terrain: elevation terrain matrix
 * @angle: angle of rotation
 * @time: interpolation time
 * Return: void
 */
void draw_stuff(SDL_Instance *instance,
		int **terrain, double angle, double *time)
{
	int y, x, tiles = 0;
	int start_x = WIDTH / 2;
	int start_y = HEIGHT / 2;

	while (terrain[tiles])
		tiles++;
	start_y -= tiles * T_H / 2;
	SDL_SetRenderDrawColor(instance->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	for (y = 0; y < tiles - 1; y++)
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
				translate(&tile, 0, -lerp(*time, 0, terrain[y][x]));
				translate(&top_r, 0, -lerp(*time, 0, terrain[y][x + 1]));
				translate(&bottom_r, 0, -lerp(*time, 0, terrain[y + 1][x + 1]));
				translate(&bottom_l, 0, -lerp(*time, 0, terrain[y + 1][x]));
			}
			draw_line(instance, &tile, &top_r);
			draw_line(instance, &top_r, &bottom_r);
			draw_line(instance, &bottom_r, &bottom_l);
			draw_line(instance, &bottom_l, &tile);
		}
	}
}
/**
 * draw_line - draws a line from a to b
 * @instance: sdl instance
 * @a: line a
 * @b: line b
 * Return: void
 */
void draw_line(SDL_Instance *instance, SDL_Point *a, SDL_Point *b)
{
	SDL_RenderDrawLine(instance->renderer, a->x, a->y, b->x, b->y);
}
