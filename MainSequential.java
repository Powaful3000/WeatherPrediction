public class MainSequential {
    public static void main(String[] args) {
        int rows = 100;  // Default grid size
        int cols = 100;
        int steps = 100;

        if (args.length > 0) {
            rows = Integer.parseInt(args[0]);
        }
        if (args.length > 1) {
            cols = Integer.parseInt(args[1]);
        }
        if (args.length > 2) {
            steps = Integer.parseInt(args[2]);
        }

        Grid grid = new Grid(rows, cols);
        grid.initializeCells();

        String filename = "weather_simulation.csv";

        for (int i = 0; i < steps; i++) {
            System.out.println("Step " + (i + 1));
            grid.update();  // Serial update
            // grid.displayGrid(); // Optional depending on if we want to do this or not.
            grid.saveGridToCSV(filename, i + 1);
        }
    }
}
