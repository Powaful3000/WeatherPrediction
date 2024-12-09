#ifndef GRID_H
#define GRID_H

// #include <pthread.h>  // Comment this out temporarily if not using threads yet
#include "WeatherCell.h"

// Structure definition matching Java class fields
typedef struct Grid {
    WeatherCell*** cells;  // 2D array of WeatherCell pointers (matching private WeatherCell[][] cells)
    int rows;             // matching private int rows
    int cols;             // matching private int cols
} Grid;

// Constructor (matching Java constructor)
Grid* Grid_new(int rows, int cols);

// Cell initialization (matching Java method)
void Grid_initialize_cells(Grid* grid);

// Cell access methods (matching Java methods)
WeatherCell* Grid_get_cell(Grid* grid, int row, int col);
void Grid_update_cell(Grid* grid, int row, int col, WeatherCell* new_state);

// Getter methods (matching Java getters)
int Grid_get_rows(Grid* grid);
int Grid_get_cols(Grid* grid);

// Cell array setter (matching Java method)
void Grid_set_cells(Grid* grid, WeatherCell*** new_cells);

// Update methods (matching Java methods)
void Grid_update(Grid* grid);  // Serial update
void Grid_update_parallel(Grid* grid, int num_threads);  // Parallel update

// Neighbor retrieval methods (matching Java methods)
WeatherCell** Grid_get_neighbors(Grid* grid, int row, int col);
WeatherCell** Grid_get_neighbors_parallel(Grid* grid, int row, int col, WeatherCell*** cells_array);

// Position validation (matching Java method)
int Grid_is_valid_position(Grid* grid, int row, int col);

// Display method (matching Java method)
void Grid_display_grid(Grid* grid);

// File output method (matching Java method)
void Grid_save_grid_to_csv(Grid* grid, const char* filename, int step);

// Boundary exchange methods (matching Java methods)
WeatherCell** Grid_get_row(Grid* grid, int row_index);
void Grid_set_row(Grid* grid, int row_index, WeatherCell** row_data);

// Memory management (required for C)
void Grid_destroy(Grid* grid);

#endif // GRID_H 