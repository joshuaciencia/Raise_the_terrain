#include "holberton.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

/**
 * parse_terrain - parses a square matrix from file
 * @file: file to be parsed
 * Return: terrain
 */
int **parse_terrain(const char *file)
{
	int i = 0, j = 0, **terrain;
	FILE *fp;
	size_t bytes = 1024;
	char *token, *del = " \n", *buff = 0;

	fp = fopen(file, "r");
	if (!fp)
	{
		fprintf(stderr, "Error: File not found %s\n", file);
		exit(EXIT_FAILURE);
	}
	terrain = malloc(sizeof(int *) * bytes);
	if (!terrain)
	{
		fclose(fp);
		fprintf(stderr, "Not enough memory to allocate\n");
		exit(EXIT_FAILURE);
	}
	while (getline(&buff, &bytes, fp) != EOF)
	{
		terrain[i] = malloc(sizeof(int) * bytes);
		if (!terrain[i])
		{
			int k;

			for (k = 0; k < i; k++)
				free(terrain[k]);
			free(terrain);
			fclose(fp);
			free(buff);
			fprintf(stderr, "Not enough memory to allocate\n");
			exit(EXIT_FAILURE);
		}
		token = strtok(buff, del);
		j = 0;
		while (token)
		{
			int val = atoi(token);

			terrain[i][j] = val;
			token = strtok(0, del);
			j++;
		}
		i++;
	}
	fclose(fp);
	free(buff);
	return (terrain);
}
