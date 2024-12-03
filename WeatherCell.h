#ifndef WEATHER_CELL_H
#define WEATHER_CELL_H

// Physical constants (matching Java static final variables)
#define AIR_DENSITY 1.225             // kg/m^3 at sea level
#define SPECIFIC_HEAT_CAPACITY 1005   // J/(kg·K) for air

// Structure definition matching Java class fields
typedef struct WeatherCell {
    double temperature;  // in Kelvin
    double pressure;     // in Pascals
    double humidity;     // as a percentage (0 to 100)
} WeatherCell;

// Functions matching Java class methods
// Default constructor (with random initialization)
WeatherCell* WeatherCell_new(void);

// Constructor with specific values (matching Java constructor)
WeatherCell* WeatherCell_new_with_values(double temperature, double pressure, double humidity);

// Calculate new state (matching Java method)
WeatherCell* WeatherCell_calculate_new_state(WeatherCell* cell, WeatherCell* neighbors[4]);

// Memory management (required for C)
void WeatherCell_destroy(WeatherCell* cell);

#endif // WEATHER_CELL_H