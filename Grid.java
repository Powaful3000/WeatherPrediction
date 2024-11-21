import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Random;

public class Grid {
    private WeatherCell[][] cells;
    private int rows;
    private int cols;
    private Random random;

    public Grid(int rows, int cols) {
        this(rows, cols, System.currentTimeMillis());
    }

    public Grid(int rows, int cols, long seed) {
        this.random = new Random(seed);
        this.rows = rows;
        this.cols = cols;
        cells = new WeatherCell[rows][cols];
    }

    public void initializeCells() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cells[i][j] = new WeatherCell(this.random);
            }
        }
    }

    // Method to get a specific cell
    public WeatherCell getCell(int row, int col) {
        if (isValidPosition(row, col)) {
            return cells[row][col];
        }
        return null;
    }

    // Method to update a specific cell
    public void updateCell(int row, int col, WeatherCell newState) {
        if (isValidPosition(row, col)) {
            cells[row][col] = newState;
        }
    }

    // Getter methods
    public int getRows() {
        return rows;
    }

    public int getCols() {
        return cols;
    }

    public void setCells(WeatherCell[][] newCells) {
        cells = newCells;
    }

    // Serial update method
    public void update() {
        WeatherCell[][] newCells = new WeatherCell[rows][cols];

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                WeatherCell currentCell = getCell(i, j);
                WeatherCell[] neighbors = getNeighbors(i, j);
                newCells[i][j] = currentCell.calculateNewState(neighbors);
            }
        }

        cells = newCells;
    }

    // Parallel update method using multiple threads
    public void update(int numThreads) throws InterruptedException {
        final WeatherCell[][] newCells = new WeatherCell[rows][cols];
        final WeatherCell[][] oldCells = this.cells; // Reference to the original grid

        Thread[] threads = new Thread[numThreads];
        int rowsPerThread = rows / numThreads;
        int remainder = rows % numThreads;
        int startRow = 0;

        for (int i = 0; i < numThreads; i++) {
            int threadRows = rowsPerThread + (i < remainder ? 1 : 0);
            int endRow = startRow + threadRows;

            final int threadStartRow = startRow;
            final int threadEndRow = endRow;

            threads[i] = new Thread(() -> {
                for (int row = threadStartRow; row < threadEndRow; row++) {
                    for (int col = 0; col < cols; col++) {
                        WeatherCell currentCell = oldCells[row][col];
                        WeatherCell[] neighbors = getNeighbors(row, col, oldCells);
                        newCells[row][col] = currentCell.calculateNewState(neighbors);
                    }
                }
            });

            threads[i].start();
            startRow = endRow;
        }

        // Wait for all threads to finish
        for (Thread thread : threads) {
            thread.join();
        }

        // Update the grid cells
        this.cells = newCells;
    }

    // Neighbor retrieval for serial update
    public WeatherCell[] getNeighbors(int row, int col) {
        WeatherCell[] neighbors = new WeatherCell[4]; // Up, Down, Left, Right

        if (isValidPosition(row - 1, col)) {
            neighbors[0] = cells[row - 1][col]; // Up
        }
        if (isValidPosition(row + 1, col)) {
            neighbors[1] = cells[row + 1][col]; // Down
        }
        if (isValidPosition(row, col - 1)) {
            neighbors[2] = cells[row][col - 1]; // Left
        }
        if (isValidPosition(row, col + 1)) {
            neighbors[3] = cells[row][col + 1]; // Right
        }

        return neighbors;
    }

    // Neighbor retrieval for parallel update
    public WeatherCell[] getNeighbors(int row, int col, WeatherCell[][] cellsArray) {
        WeatherCell[] neighbors = new WeatherCell[4]; // Up, Down, Left, Right

        if (isValidPosition(row - 1, col)) {
            neighbors[0] = cellsArray[row - 1][col]; // Up
        }
        if (isValidPosition(row + 1, col)) {
            neighbors[1] = cellsArray[row + 1][col]; // Down
        }
        if (isValidPosition(row, col - 1)) {
            neighbors[2] = cellsArray[row][col - 1]; // Left
        }
        if (isValidPosition(row, col + 1)) {
            neighbors[3] = cellsArray[row][col + 1]; // Right
        }

        return neighbors;
    }

    public boolean isValidPosition(int row, int col) {
        return (row >= 0 && row < rows && col >= 0 && col < cols);
    }

    // Display the grid (optional)
    public void displayGrid() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                System.out.print((cells[i][j].temperature - 273.15) + " "); // Convert Kelvin to Celsius
            }
            System.out.println();
        }
    }

    // Save the grid state to a CSV file
    public void saveGridToCSV(String filename, int step) {
        File file = new File(filename);

        // Ensure the file exists or can be created
        try {
            if (!file.exists()) {
                file.createNewFile(); // Create the file if it does not exist
            }
        } catch (IOException e) {
            System.err.println("Error creating file: " + filename);
            e.printStackTrace();
            return; // Exit the method as file creation failed
        }

        // Attempt to open and write to the file
        for (int attempt = 0; attempt < 3; attempt++) { // Retry logic
            try (FileWriter fileWriter = new FileWriter(file, true)) {
                fileWriter.write("Step " + step + "\n");
                for (int i = 0; i < rows; i++) {
                    for (int j = 0; j < cols; j++) {
                        WeatherCell cell = cells[i][j];
                        fileWriter.write(cell.temperature + "," + cell.pressure + "," + cell.humidity);
                        if (j < cols - 1) {
                            fileWriter.write(",");
                        }
                    }
                    fileWriter.write("\n");
                }
                // Successful write; exit the retry loop
                return;
            } catch (IOException e) {
                System.err.println("Attempt " + (attempt + 1) + ": Unable to open file " + filename);
                if (attempt == 2) {
                    System.err.println("Failed after multiple attempts. Aborting write operation.");
                    e.printStackTrace();
                }
            }
        }
    }

    // Read the initial grid state from a CSV file
    public void loadGridFromCSV(String filename) {
        try (BufferedReader br = new BufferedReader(new FileReader(filename))) {
            // Determine the dimensions of the grid
            String line;
            int rows = 0;
            int cols = 0;

            // Read the first line to determine the number of columns
            if ((line = br.readLine()) != null) {
                String[] values = line.split(",");
                cols = values.length / 3; // Each cell has temperature, pressure, and humidity
                rows++;
            }

            // Count the remaining rows
            while ((line = br.readLine()) != null) {
                rows++;
            }

            // Reinitialize the grid with the new dimensions
            this.rows = rows;
            this.cols = cols;
            this.cells = new WeatherCell[rows][cols];

            // Reset the BufferedReader to read the file again
            br.close();
            try (BufferedReader br2 = new BufferedReader(new FileReader(filename))) {
                int row = 0;
                while ((line = br2.readLine()) != null) {
                    String[] values = line.split(",");
                    for (int col = 0; col < cols; col++) {
                        double temperature = Double.parseDouble(values[col * 3]);
                        double pressure = Double.parseDouble(values[col * 3 + 1]);
                        double humidity = Double.parseDouble(values[col * 3 + 2]);
                        cells[row][col] = new WeatherCell(temperature, pressure, humidity);
                    }
                    row++;
                }
            }

        } catch (IOException e) {
            System.err.println("Error reading the grid from file: " + filename);
            e.printStackTrace();
        } catch (NumberFormatException e) {
            System.err.println("Error parsing numerical values in file: " + filename);
            e.printStackTrace();
        }
    }

    // Methods for boundary exchange (not directly applicable in Java
    // multithreading)
    public WeatherCell[] getRow(int rowIndex) {
        if (rowIndex >= 0 && rowIndex < rows) {
            return cells[rowIndex];
        } else {
            // Return an empty array or handle ghost rows if necessary
            return new WeatherCell[cols];
        }
    }

    public void setRow(int rowIndex, WeatherCell[] rowData) {
        if (rowIndex >= 0 && rowIndex < rows) {
            cells[rowIndex] = rowData;
        }
        // Handle ghost rows if necessary
    }
}
