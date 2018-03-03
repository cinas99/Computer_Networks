package pl.poznan.put.game;

public final class Converter {
    private static final String DELIMITER = ",";
    private static int playerNumber;
    private static int pointNumber;
    private static double x;
    private static double y;
    private static boolean isGap;

    private Converter() {}

    public static void convert(String msg) {
        String [] components = msg.split(DELIMITER);
        playerNumber = Integer.parseInt(components[0]);
        pointNumber = Integer.parseInt(components[1]);
        x = Double.parseDouble(components[2]);
        y = Double.parseDouble(components[3]);
        isGap = components[4].equals("1");
        System.out.println("Converted msg: " + playerNumber + " " + pointNumber + " " + x + " " + y + " " + isGap + "\n");
    }
}
