public class MainSequential {
    public static void main(String[] args) {
        int rows = 100;  // Default grid size
        int cols = 100;
        int steps = 100;
        long seed = 12345;
        String input_filename = "";
        String output_file = "weather_simulation.csv";


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
            input_filename = args[4];
        }

        Grid grid = new Grid(rows, cols, seed);

        if (input_filename.isEmpty()) {
            grid.initializeCells();
        } else {
            grid.loadGridFromCSV(input_filename);
        }


        for (int i = 0; i < steps; i++) {
            System.out.println("Step " + (i + 1));
            grid.update();  // Serial update
            // grid.displayGrid(); // Optional depending on if we want to do this or not.
            grid.saveGridToCSV(output_file, i + 1);
        }
    }
}
