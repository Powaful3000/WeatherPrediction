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
    // Calculate pressure from temperature
    cell->pressure = cell->temperature * MOLES * GAS_CONSTANT;
    cell->rHumidity = random_range(0, 100);             // 0-100%
    // Specific humidity is calculated based off relative humidity
    double celsius = cell->temperature - 273.15;
    cell->sHumidity = ((cell->rHumidity/100) * 0.61121 * exp((18.678-(celsius/234.5))*(celsius/(257.14 + celsius)))) / (461.5 * cell->temperature * 100) * 1000; // in kg/m^3
    //Calculate humid heat capacity
    cell->hCapacity = SPECIFIC_HEAT_CAPACITY + (1.82 * (cell->sHumidity/(AIR_DENSITY - cell->sHumidity)));
    
    return cell;
}

// Constructor with specific values
WeatherCell* WeatherCell_new_with_values(double temperature, double pressure, double humidity) {
    WeatherCell* cell = (WeatherCell*)malloc(sizeof(WeatherCell));
    if (cell == NULL) return NULL;
    
    cell->temperature = temperature;
    double celsius = temperature - 273.15;
    cell->pressure = pressure;
    cell->rHumidity = humidity > 100 ? 100 : (humidity < 0 ? 0 : humidity);
    cell->sHumidity = ((cell->rHumidity/100) * 0.61121 * exp((18.678-(celsius/234.5))*(celsius/(257.14 + celsius)))) / (461.5 * cell->temperature * 100) * 1000; // in kg/m^3
    cell->hCapacity = SPECIFIC_HEAT_CAPACITY + (1.82 * (cell->sHumidity/(AIR_DENSITY - cell->sHumidity)));
    
    return cell;
}

// Calculate new state based on neighbors
WeatherCell* WeatherCell_calculate_new_state(WeatherCell* cell, WeatherCell* neighbors[4]) {
    if (cell == NULL) return NULL;
    
    // Create new cell for next state
    WeatherCell* new_cell = WeatherCell_new_with_values(
        cell->temperature,
        cell->pressure,
        cell->rHumidity
    );
    if (new_cell == NULL) return NULL;

    // Count valid neighbors
    int valid_neighbors = 0;
    double heatChange = 0;
    double tempChange = 0;
    double pressureChange = 0;
    double sHumidityChange = 0;
    double newTemperature = cell->temperature;
    double newPressure = cell->pressure;
    double newRHumidity = cell->rHumidity;

    // Calculate averages including current cell
    valid_neighbors = 0;  // Count self

    // Add neighbor values
    for (int i = 0; i < 4; i++) {
        if (neighbors[i] != NULL) {
            // Temperature exchange based on heat transfer
                double htc = ((neighbors[i]->temperature - cell->temperature)/1.444) + 10;
                heatChange += htc * (neighbors[i]->temperature - cell->temperature); // watt/m^2
                // Pressure change based on fluid dynamics (simplified)
                pressureChange += neighbors[i]->pressure;
                // Simplified Humidity diffusion
                sHumidityChange += neighbors[i]->sHumidity;
                valid_neighbors++;
        }
    }

    // Calculate final averages
    if (valid_neighbors > 0) {
        tempChange = heatChange/(AIR_DENSITY * cell->hCapacity * 1000);             //simplified temperature exchange through convection
        newTemperature = cell->temperature + tempChange;
        double celsius = newTemperature - 273.15;
        newPressure = (cell->pressure + pressureChange)/(valid_neighbors + 1);       //simplified pressure exchange
        double tempsHumidity = (cell->sHumidity + sHumidityChange)/(valid_neighbors + 1);    //VERY simplified humidity exchange
        newRHumidity = (tempsHumidity * 461.5 * newTemperature * 100)/(0.61121 * exp((18.678-(celsius/234.5))*(celsius/(257.14 + celsius))))/10;          
    }

    
    // Update new cell state with smoothed values and adjustments
    new_cell->temperature = newTemperature > 300 ? 300 : (newTemperature < 200 ? 200 : newTemperature);
    new_cell->pressure = newPressure > 120000 ? 120000 : (newPressure < 80000 ? 80000 : newPressure);
    new_cell->rHumidity = newRHumidity > 100 ? 100 : (newRHumidity < 0 ? 0 : newRHumidity);

    return new_cell;
}

// Memory management
void WeatherCell_destroy(WeatherCell* cell) {
    free(cell);
}
