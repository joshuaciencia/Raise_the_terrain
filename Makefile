SRC = main.c parse_file.c
EXE = terrain
FILE = file
all :
	gcc $(SRC) -lm -g `sdl2-config --cflags --libs` -o $(EXE)
