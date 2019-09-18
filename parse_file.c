#include "holberton.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int ** parse_terrain(const char *file)
{
	int i = 0, j = 0;
	FILE *fp;
	char *buff = 0;
	size_t bytes = 1024;
	int **terrain;
	char *token;
	char *del = " \n";

	fp = fopen(file, "r");
	if (!fp)
		return (0);

	terrain = malloc(sizeof(int *) * bytes);
	if(!terrain)
	{
		fclose(fp);
		return (0);
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
			return (0);
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
