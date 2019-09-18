SRC = main.c parse_file.c
EXE = raise_terrain
FILE = file
all :
	gcc $(SRC) -g `sdl2-config --cflags --libs` -o $(EXE)
	./$(EXE) $(FILE)
	#gdb ./$(EXE)
