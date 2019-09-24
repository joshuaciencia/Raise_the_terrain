SRC = main.c parse_file.c core.c
EXE = terrain
FILE = file
all :
	gcc $(SRC) -lm -g `sdl2-config --cflags --libs` -o $(EXE)
	./$(EXE) $(FILE)
	#gdb ./$(EXE)
