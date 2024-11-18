public class MainParallel {
    public static void main(String[] args) {
        int rows = 100;  // Default grid size
        int cols = 100;
        int steps = 100;
        int numThreads = Runtime.getRuntime().availableProcessors();

        if (args.length > 0) {
            rows = Integer.parseInt(args[0]);
        }
        if (args.length > 1) {
            cols = Integer.parseInt(args[1]);
        }
        if (args.length > 2) {
            steps = Integer.parseInt(args[2]);
        }
        if (args.length > 3) {
            numThreads = Integer.parseInt(args[3]);
        }

        Grid grid = new Grid(rows, cols);
        grid.initializeCells();

        String filename = "weather_simulation_parallel.csv";

        for (int i = 0; i < steps; i++) {
            System.out.println("Step " + (i + 1));
            try {
                grid.update(numThreads);  // Parallel update
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            // grid.displayGrid(); // Optional
            grid.saveGridToCSV(filename, i + 1);
        }
    }
}
