#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Grid.h"

int main(int argc, char *argv[]) {
    // Default values matching Java implementation
    int rows = 100;
    int cols = 100;
    int steps = 100;

    // Parse command line arguments (matching Java's argument parsing)
    if (argc > 1) {
        rows = atoi(argv[1]);
    }
    if (argc > 2) {
        cols = atoi(argv[2]);
    }
    if (argc > 3) {
        steps = atoi(argv[3]);
    }

    // Initialize random number generator
    srand(time(NULL));

    // Create and initialize grid
    Grid* grid = Grid_new(rows, cols);
    if (grid == NULL) {
        fprintf(stderr, "Failed to create grid\n");
        return 1;
    }
    Grid_initialize_cells(grid);

    const char* filename = "weather_simulation.csv";
    FILE* file = fopen(filename, "w");
    fclose(file);

    // Main simulation loop
    for (int i = 0; i < steps; i++) {
        printf("Step %d\n", i + 1);
        Grid_update(grid);  // Serial update
        // Grid_display_grid(grid);  // Optional, commented out as in Java
        Grid_save_grid_to_csv(grid, filename, i + 1);
    }

    // Clean up
    Grid_destroy(grid);

    return 0;
} 