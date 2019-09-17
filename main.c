#include "holberton.h"

int main(int argc, char* argv[])
{
	if (argc != 2)
		return (1);

	SDL_Instance instance;
	SDL_bool running = SDL_TRUE;

	if (init_instance(&instance) != 0)
		return (1);

	/* open file */

	int s = read_textfile(argv[1], 1000);


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
					if (key.keysym.scancode == 0x29)
						running = SDL_FALSE;
					break;
			}
		}
		/* render graphics */
		draw_stuff(&instance);
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
		       	SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);

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

void draw_stuff(SDL_Instance *instance)
{
	/*draw grid*/
	int start_x = WIDTH / 2;
	int start_y = HEIGHT / 2 - T_H * TILES / 2;
	int y, x;
	/* draw the lines with white color */
	SDL_SetRenderDrawColor(instance->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	/* draw lines */
	for(y = 0; y < TILES; y++)
	{
		for (x = 0; x < TILES; x++)
		{
			int x_offset = ((x - y) * T_W / 2);
			int y_offset = ((x + y) * T_H / 2);

			int tl_x = start_x + x_offset;
			int tl_y = start_y - T_H / 2 + y_offset;
			int tr_x = start_x + T_W / 2 + x_offset;
			int tr_y = start_y + y_offset;
			int bl_x = start_x - T_W / 2 + x_offset;
			int bl_y = start_y + y_offset;
			int br_x = start_x + x_offset;
			int br_y = start_y + T_H / 2 + y_offset;
			/* draw up side */
			SDL_RenderDrawLine(instance->renderer, tl_x, tl_y, tr_x, tr_y);
			/* draw left side */
			SDL_RenderDrawLine(instance->renderer, tl_x, tl_y, bl_x, bl_y);
			/* draw right side */
			SDL_RenderDrawLine(instance->renderer, tr_x, tr_y, br_x, br_y);
			/* draw down side */
			SDL_RenderDrawLine(instance->renderer, bl_x, bl_y, br_x, br_y);
		}
	}
}
