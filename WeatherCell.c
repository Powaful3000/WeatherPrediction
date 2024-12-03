#include "WeatherCell.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Helper function to get random value in range
static double random_range(double min, double max) {
    double scale = rand() / (double) RAND_MAX;
    return min + scale * (max - min);
}

// Default constructor with random initialization
WeatherCell* WeatherCell_new(void) {
    WeatherCell* cell = (WeatherCell*)malloc(sizeof(WeatherCell));
    if (cell == NULL) return NULL;
    
    // Initialize with random but realistic values
    cell->temperature = random_range(273.15, 313.15);  // 0°C to 40°C
    cell->pressure = random_range(95000, 105000);      // typical atmospheric pressure range
    cell->humidity = random_range(0, 100);             // 0-100%
    
    return cell;
}

// Constructor with specific values
WeatherCell* WeatherCell_new_with_values(double temperature, double pressure, double humidity) {
    WeatherCell* cell = (WeatherCell*)malloc(sizeof(WeatherCell));
    if (cell == NULL) return NULL;
    
    cell->temperature = temperature;
    cell->pressure = pressure;
    cell->humidity = humidity > 100 ? 100 : (humidity < 0 ? 0 : humidity);
    
    return cell;
}

// Calculate new state based on neighbors
WeatherCell* WeatherCell_calculate_new_state(WeatherCell* cell, WeatherCell* neighbors[4]) {
    if (cell == NULL) return NULL;
    
    // Create new cell for next state
    WeatherCell* new_cell = WeatherCell_new_with_values(
        cell->temperature,
        cell->pressure,
        cell->humidity
    );
    if (new_cell == NULL) return NULL;

    // Count valid neighbors
    int valid_neighbors = 0;
    double avg_temperature = 0;
    double avg_pressure = 0;
    double avg_humidity = 0;

    // Calculate averages including current cell
    avg_temperature = cell->temperature;
    avg_pressure = cell->pressure;
    avg_humidity = cell->humidity;
    valid_neighbors = 1;  // Count self

    // Add neighbor values
    for (int i = 0; i < 4; i++) {
        if (neighbors[i] != NULL) {
            avg_temperature += neighbors[i]->temperature;
            avg_pressure += neighbors[i]->pressure;
            avg_humidity += neighbors[i]->humidity;
            valid_neighbors++;
        }
    }

    // Calculate final averages
    if (valid_neighbors > 0) {
        avg_temperature /= valid_neighbors;
        avg_pressure /= valid_neighbors;
        avg_humidity /= valid_neighbors;
    }

    // Apply simple physics-based adjustments
    // Temperature affects pressure (ideal gas law simplified)
    double pressure_adjustment = (avg_temperature - cell->temperature) * 
                               (AIR_DENSITY * SPECIFIC_HEAT_CAPACITY) / 100;
    
    // Update new cell state with smoothed values and adjustments
    new_cell->temperature = avg_temperature;
    new_cell->pressure = avg_pressure + pressure_adjustment;
    new_cell->humidity = avg_humidity;

    return new_cell;
}

// Memory management
void WeatherCell_destroy(WeatherCell* cell) {
    free(cell);
}
