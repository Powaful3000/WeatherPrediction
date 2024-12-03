#include "Grid.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

// Structure for thread arguments
typedef struct {
    Grid* grid;
    WeatherCell*** new_cells;
    int start_row;
    int end_row;
} ThreadArgs;

// Constructor
Grid* Grid_new(int rows, int cols) {
    Grid* grid = (Grid*)malloc(sizeof(Grid));
    if (grid == NULL) return NULL;

    grid->rows = rows;
    grid->cols = cols;

    // Allocate 2D array of WeatherCell pointers
    grid->cells = (WeatherCell***)malloc(rows * sizeof(WeatherCell**));
    if (grid->cells == NULL) {
        free(grid);
        return NULL;
    }

    for (int i = 0; i < rows; i++) {
        grid->cells[i] = (WeatherCell**)malloc(cols * sizeof(WeatherCell*));
        if (grid->cells[i] == NULL) {
            Grid_destroy(grid);
            return NULL;
        }
    }

    return grid;
}

// Initialize cells
void Grid_initialize_cells(Grid* grid) {
    for (int i = 0; i < grid->rows; i++) {
        for (int j = 0; j < grid->cols; j++) {
            grid->cells[i][j] = WeatherCell_new();
        }
    }
}

// Cell access methods
WeatherCell* Grid_get_cell(Grid* grid, int row, int col) {
    if (Grid_is_valid_position(grid, row, col)) {
        return grid->cells[row][col];
    }
    return NULL;
}

void Grid_update_cell(Grid* grid, int row, int col, WeatherCell* new_state) {
    if (Grid_is_valid_position(grid, row, col)) {
        WeatherCell_destroy(grid->cells[row][col]);
        grid->cells[row][col] = new_state;
    }
}

// Getter methods
int Grid_get_rows(Grid* grid) {
    return grid->rows;
}

int Grid_get_cols(Grid* grid) {
    return grid->cols;
}

void Grid_set_cells(Grid* grid, WeatherCell*** new_cells) {
    grid->cells = new_cells;
}

// Get neighbors
WeatherCell** Grid_get_neighbors(Grid* grid, int row, int col) {
    WeatherCell** neighbors = (WeatherCell**)malloc(4 * sizeof(WeatherCell*));
    
    // Initialize all to NULL
    for (int i = 0; i < 4; i++) {
        neighbors[i] = NULL;
    }

    // Up
    if (Grid_is_valid_position(grid, row - 1, col)) {
        neighbors[0] = grid->cells[row - 1][col];
    }
    // Down
    if (Grid_is_valid_position(grid, row + 1, col)) {
        neighbors[1] = grid->cells[row + 1][col];
    }
    // Left
    if (Grid_is_valid_position(grid, row, col - 1)) {
        neighbors[2] = grid->cells[row][col - 1];
    }
    // Right
    if (Grid_is_valid_position(grid, row, col + 1)) {
        neighbors[3] = grid->cells[row][col + 1];
    }

    return neighbors;
}

// Thread function for parallel update
static void* update_thread_function(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    
    for (int i = args->start_row; i < args->end_row; i++) {
        for (int j = 0; j < args->grid->cols; j++) {
            WeatherCell* current = Grid_get_cell(args->grid, i, j);
            WeatherCell** neighbors = Grid_get_neighbors(args->grid, i, j);
            args->new_cells[i][j] = WeatherCell_calculate_new_state(current, neighbors);
            free(neighbors);
        }
    }
    
    return NULL;
}

// Serial update
void Grid_update(Grid* grid) {
    WeatherCell*** new_cells = (WeatherCell***)malloc(grid->rows * sizeof(WeatherCell**));
    for (int i = 0; i < grid->rows; i++) {
        new_cells[i] = (WeatherCell**)malloc(grid->cols * sizeof(WeatherCell*));
    }

    for (int i = 0; i < grid->rows; i++) {
        for (int j = 0; j < grid->cols; j++) {
            WeatherCell* current = Grid_get_cell(grid, i, j);
            WeatherCell** neighbors = Grid_get_neighbors(grid, i, j);
            new_cells[i][j] = WeatherCell_calculate_new_state(current, neighbors);
            free(neighbors);
        }
    }

    // Free old cells and update grid
    for (int i = 0; i < grid->rows; i++) {
        for (int j = 0; j < grid->cols; j++) {
            WeatherCell_destroy(grid->cells[i][j]);
        }
        free(grid->cells[i]);
    }
    free(grid->cells);
    grid->cells = new_cells;
}

// Parallel update
void Grid_update_parallel(Grid* grid, int num_threads) {
    pthread_t* threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
    ThreadArgs* thread_args = (ThreadArgs*)malloc(num_threads * sizeof(ThreadArgs));
    
    // Allocate new cells array
    WeatherCell*** new_cells = (WeatherCell***)malloc(grid->rows * sizeof(WeatherCell**));
    for (int i = 0; i < grid->rows; i++) {
        new_cells[i] = (WeatherCell**)malloc(grid->cols * sizeof(WeatherCell*));
    }

    // Calculate rows per thread
    int rows_per_thread = grid->rows / num_threads;
    int remaining_rows = grid->rows % num_threads;
    int current_row = 0;

    // Create threads
    for (int i = 0; i < num_threads; i++) {
        thread_args[i].grid = grid;
        thread_args[i].new_cells = new_cells;
        thread_args[i].start_row = current_row;
        thread_args[i].end_row = current_row + rows_per_thread + (i < remaining_rows ? 1 : 0);
        current_row = thread_args[i].end_row;

        pthread_create(&threads[i], NULL, update_thread_function, &thread_args[i]);
    }

    // Wait for threads to complete
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Free old cells and update grid
    for (int i = 0; i < grid->rows; i++) {
        for (int j = 0; j < grid->cols; j++) {
            WeatherCell_destroy(grid->cells[i][j]);
        }
        free(grid->cells[i]);
    }
    free(grid->cells);
    grid->cells = new_cells;

    // Cleanup
    free(threads);
    free(thread_args);
}

// Position validation
int Grid_is_valid_position(Grid* grid, int row, int col) {
    return (row >= 0 && row < grid->rows && col >= 0 && col < grid->cols);
}

// Display grid
void Grid_display_grid(Grid* grid) {
    for (int i = 0; i < grid->rows; i++) {
        for (int j = 0; j < grid->cols; j++) {
            printf("%.2f ", grid->cells[i][j]->temperature - 273.15);
        }
        printf("\n");
    }
}

// Save to CSV
void Grid_save_grid_to_csv(Grid* grid, const char* filename, int step) {
    FILE* file = fopen(filename, "a");
    if (file == NULL) {
        fprintf(stderr, "Unable to open file %s\n", filename);
        return;
    }

    fprintf(file, "Step %d\n", step);
    for (int i = 0; i < grid->rows; i++) {
        for (int j = 0; j < grid->cols; j++) {
            WeatherCell* cell = grid->cells[i][j];
            fprintf(file, "%.2f,%.2f,%.2f", 
                cell->temperature, cell->pressure, cell->humidity);
            if (j < grid->cols - 1) {
                fprintf(file, ",");
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

// Memory management
void Grid_destroy(Grid* grid) {
    if (grid == NULL) return;

    if (grid->cells != NULL) {
        for (int i = 0; i < grid->rows; i++) {
            if (grid->cells[i] != NULL) {
                for (int j = 0; j < grid->cols; j++) {
                    WeatherCell_destroy(grid->cells[i][j]);
                }
                free(grid->cells[i]);
            }
        }
        free(grid->cells);
    }
    free(grid);
}
