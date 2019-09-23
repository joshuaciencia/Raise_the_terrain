#include "holberton.h"

/**
 * to_iso - converts tile coords to isometric coords
 * @point: point to convert
 * Return: void
 */
void to_iso(SDL_Point *point)
{
	int x = point->x;
	int y = point->y;

	point->y = (x + y) * T_H / 2;
	point->x = (x - y) * T_W / 2;
}
/**
 * translate - moves a point x, y pixels
 * @point: point to move
 * @x: x pixels
 * @y: y pixels
 * Return: void
 */
void translate(SDL_Point *point, int x, int y)
{
	point->x += x;
	point->y += y;
}
/**
 * rotate - rotate point to angle
 * @point: point to rotate
 * @angle: angle (in radians)
 * Return: void
 */
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
/**
 * get_angle - gets the direction of a vector
 * @x: x coord
 * @y: y coord
 * Return: void
 */
double get_angle(int x, int y)
{
	return (atan2(y, x));
}
/**
 * lerp - interpolates distance (linear)
 * @t: interpolation time (0, 1)
 * @start: start of the distance
 * @end: end of the distance
 * Return: void
 */
int lerp(double t, int start, int end)
{
	return ((int)(start + ((end - start) * t)));
}
