#ifndef WEATHER_CELL_H
#define WEATHER_CELL_H

// Physical constants (matching Java static final variables)
    #define AIR_DENSITY 1.225               // kg/m^3 at sea level
    #define SPECIFIC_HEAT_CAPACITY 1.005    // kJ/(kg·K) for air
    #define MOLES 42.29                     // # of moles in 1 m^3 of air at STP
    #define GAS_CONSTANT 8.314              // J / mol * K

// Structure definition matching Java class fields
typedef struct WeatherCell {
    double temperature;  // in Kelvin
    double hCapacity;    // humid heat capacity in Kelvin
    double pressure;     // in Pascals
    double sHumidity;    // Actual amount of water in air in g/m^3
    double rHumidity;     // relative amount of humidity as a percentage (0 to 100)
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