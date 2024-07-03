# Executable names:
EXE = project
SCC = view_scc
TEST = test

# Add all object files needed for compiling:
EXE_OBJ = main.o
OBJS = main.o FlightGraph.o HSLAPixel.o lodepng.o map_utils.o PNG.o RGB_HSL.o

SCC_OBJ = view_scc.o
SCC_OBJS = view_scc.o FlightGraph.o HSLAPixel.o lodepng.o map_utils.o PNG.o RGB_HSL.o


# Use the cs225 makefile template:
include make/cs225.mk
