public class MainParallel {
    public static void main(String[] args) {
        int rows = 100;  // Default grid size
        int cols = 100;
        int steps = 100;
        long seed = 12345;
        int numThreads = Runtime.getRuntime().availableProcessors();
        String input_filename = "";
        String filename = "weather_simulation_parallel.csv";

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
            seed = Long.parseLong(args[3]);
        }
        if (args.length > 4) {
            numThreads = Integer.parseInt(args[4]);
        }
        if (args.length > 5) {
            input_filename = args[4];
        }

        Grid grid = new Grid(rows,cols,seed);
        if (input_filename.isEmpty()) {
            grid.initializeCells();
        } else {
            grid.loadGridFromCSV(input_filename);
        }

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
