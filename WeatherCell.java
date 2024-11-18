import java.util.Random;

public class WeatherCell {
    public double temperature;  // in Kelvin
    public double pressure;     // in Pascals
    public double humidity;     // as a percentage (0 to 100)

    // Physical constants
    public static final double AIR_DENSITY = 1.225;             // kg/m^3 at sea level
    public static final double SPECIFIC_HEAT_CAPACITY = 1005;   // J/(kg·K) for air

    // Random values for initialization
    public WeatherCell() {
        Random rand = new Random();
        // Random temperature between 0°C and 30°C (in Kelvin)
        this.temperature = 273.15 + rand.nextDouble() * 30.0;
        // Random pressure around 100 kPa +/- 2500 Pa
        this.pressure = 100000 + rand.nextDouble() * 5000 - 2500;
        // Random humidity between 0% and 100%
        this.humidity = rand.nextDouble() * 100.0;
    }

    // Constructor for specific values
    public WeatherCell(double temperature, double pressure, double humidity) {
        this.temperature = temperature;
        this.pressure = pressure;
        this.humidity = humidity;
    }

    // Update cell state based on simplified physics
    public WeatherCell calculateNewState(WeatherCell[] neighbors) {
        double tempChange = 0;
        double pressureChange = 0;
        double humidityChange = 0;
        int count = 0;

        for (WeatherCell neighbor : neighbors) {
            if (neighbor != null) {
                // Temperature exchange based on heat transfer
                tempChange += (neighbor.temperature - this.temperature);
                // Pressure change based on fluid dynamics (simplified)
                pressureChange += (neighbor.pressure - this.pressure);
                // Humidity diffusion
                humidityChange += (neighbor.humidity - this.humidity);
                count++;
            }
        }

        if (count > 0) {
            // Simple averaging for demonstration purposes
            tempChange /= count;
            pressureChange /= count;
            humidityChange /= count;
        }

        // Update temperature using a simplified heat equation
        double newTemperature = this.temperature + (tempChange * 0.1); // Coefficient represents thermal conductivity

        // Update pressure based on simplified fluid dynamics
        double newPressure = this.pressure + (pressureChange * 0.1); // Coefficient represents pressure diffusion

        // Update humidity
        double newHumidity = this.humidity + (humidityChange * 0.1); // Coefficient represents humidity diffusion

        // Ensure values stay within realistic bounds
        newTemperature = Math.max(200.0, Math.min(newTemperature, 330.0));     // Limit temperature between -73°C and 57°C
        newPressure = Math.max(80000.0, Math.min(newPressure, 120000.0));      // Limit pressure between 80 kPa and 120 kPa
        newHumidity = Math.max(0.0, Math.min(newHumidity, 100.0));             // Limit humidity between 0% and 100%

        return new WeatherCell(newTemperature, newPressure, newHumidity);
    }
}
