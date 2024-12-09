CC = gcc
CFLAGS = -Wall -Wextra -pthread -O2 -lm
DEPS = WeatherCell.h Grid.h
OBJ_SEQUENTIAL = WeatherCell.o Grid.o MainSequential.o
OBJ_PARALLEL = WeatherCell.o Grid.o MainParallel.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: sequential parallel

sequential: $(OBJ_SEQUENTIAL)
	$(CC) -o weather_sim_sequential $(OBJ_SEQUENTIAL) $(CFLAGS)

parallel: $(OBJ_PARALLEL)
	$(CC) -o weather_sim_parallel $(OBJ_PARALLEL) $(CFLAGS)

clean:
	rm -f *.o weather_sim_sequential weather_sim_parallel
	rm -f weather_simulation.csv weather_simulation_parallel.csv

.PHONY: all clean sequential parallel