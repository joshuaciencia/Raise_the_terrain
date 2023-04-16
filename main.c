#include "base.h"

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
	int **elevation, y, x;
	double ang = 0, time = 0, last = (double)SDL_GetTicks();
	double half_map = 3 * T_S + T_S / 2;
	double terrain[8][8][3];

	if (init_instance(&instance) != 0)
		return (1);

	if (argc != 2)
	{
		fprintf(stderr, "Usage: terrain file\n");
		exit(EXIT_FAILURE);
	}

	elevation = parse_terrain(argv[1]);

	for (y = 0; y < 8; y++)
	{
		for (x = 0; x < 8; x++)
		{
			double p_x = x * T_S - half_map;
			double p_y = y * T_S - half_map;
			double p_z = elevation[y][x];

			terrain[y][x][0] = p_x;
			terrain[y][x][1] = p_y;
			terrain[y][x][2] = p_z;
                }
        }

	while (running)
	{
		SDL_SetRenderDrawColor(instance.renderer, 0, 0, 0, 0);
		SDL_RenderClear(instance.renderer);
		SDL_Event event;
		SDL_KeyboardEvent key;

		while (SDL_PollEvent(&event))
		{
			ang = 0;
			switch (event.type)
			{
				case SDL_QUIT:
					running = SDL_FALSE;
				case SDL_KEYDOWN:
					key = event.key;
					if (key.keysym.scancode == SDL_SCANCODE_ESCAPE)
						running = SDL_FALSE;
					if (key.keysym.scancode == SDL_SCANCODE_LEFT)
						ang = -DELTA;
					if (key.keysym.scancode == SDL_SCANCODE_RIGHT)
						ang = DELTA;
			}
		}
		time += ((double)SDL_GetTicks() - last) / INTER_TIME;
		last = (double)SDL_GetTicks();
		if (time >= 1.0)
			time = 0;

        	for (y = 0; y < 8; y++)
        	{
                	for (x = 0; x < 8; x++)
                	{
				double p_x = terrain[y][x][0];
				double p_y = terrain[y][x][1];
				double p_z = terrain[y][x][2];
				
				rotate_point(&p_x, &p_y, ang);

                        	terrain[y][x][0] = p_x;
                        	terrain[y][x][1] = p_y;
                        	terrain[y][x][2] = p_z;
                	}
        	}
		draw_stuff(&instance, terrain, &time);
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
 * @terrain: terrain coords
 * @time: time since last iteration
 * Return: void
 */
void draw_stuff(SDL_Instance *instance,
		double terrain[8][8][3], double *time)
{
	int y, x;
	int center_x = WIDTH / 2;
	int center_y = HEIGHT / 2;

	SDL_SetRenderDrawColor(instance->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	for (y = 0; y < 7; y++)
	{
		for (x = 0; x < 7; x++)
		{

                        /* convert world coords to screen coords */

			SDL_Point *tl = world_to_screen(terrain[y][x][0],
				       	terrain[y][x][1], terrain[y][x][2]);
			
			SDL_Point *tr = world_to_screen(terrain[y][x + 1][0],
				       	terrain[y][x + 1][1], terrain[y][x + 1][2]);
			
			SDL_Point *bl = world_to_screen(terrain[y + 1][x][0],
				       	terrain[y + 1][x][1], terrain[y + 1][x][2]);
			
			SDL_Point *br = world_to_screen(terrain[y + 1][x + 1][0],
				       	terrain[y + 1][x + 1][1], terrain[y + 1][x + 1][2]);
                        
			/* translate to center */

			tl->x += center_x;
			tr->x += center_x;
			bl->x += center_x;
			br->x += center_x;

			tl->y += center_y;
			tr->y += center_y;
			bl->y += center_y;
			br->y += center_y;

			draw_line(instance, tl, tr);
			draw_line(instance, tr, br);
			draw_line(instance, br, bl);
			draw_line(instance, tl, bl);

			free(tl);
			free(tr);
			free(bl);
			free(br);
		}
	}
}

/**
 * rotate_point - rotates points in 3d space around z axis
 * @x: x coord
 * @y: y coord
 * @ang: angle of rotation
 * Return: void
 */
void rotate_point(double *x, double *y, double ang)
{
	/*      == rotation matrix ==
	 * [  cos(angle) | sin(angle) | 0 ]
	 * [ -sin(angle) | cos(angle) | 0 ]
	 * [           0 |          0 | 1 ]
	 */

	*x = *x * cos(ang) - *y * sin(ang);
	*y = *x * sin(ang) + *y * cos(ang);

}

/**
 * world_to_screen - maps 3d coords to screen coords
 * @x: x coord
 * @y: y coord
 * @z: z coord
 * Return: screen point
 */
SDL_Point *world_to_screen(double x, double y, double z)
{
	SDL_Point *p = malloc(sizeof(SDL_Point));

	p->x = (int)round(INCLINATION * x - INCLINATION * y);
	p->y = (int)round((1 - INCLINATION) * x + (1 - INCLINATION) * y - z);
	return (p);
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
