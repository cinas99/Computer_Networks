package pl.poznan.put.game;

public final class Converter {
    protected static final String DELIMITER = ",";
    private static int playerNumber;
    private static int pointNumber;
    private static double x;
    private static double y;
    private static boolean gap;

    private Converter() {}

    public static void convert(String msg) {
        String [] components = msg.split(DELIMITER);
        playerNumber = Integer.parseInt(components[0]);
        pointNumber = Integer.parseInt(components[1]);
        x = Double.parseDouble(components[2]);
        y = Double.parseDouble(components[3]);
        gap = components[4].equals("1");
        //System.out.println("Converted msg: " + getPlayerNumber() + " " + getPointNumber() + " " + getX() + " " + getY() + " " + isGap() + "\n");
    }

    public static int getPlayerNumber() {
        return playerNumber;
    }

    public static int getPointNumber() {
        return pointNumber;
    }

    public static double getX() {
        return x;
    }

    public static double getY() {
        return y;
    }

    public static boolean isGap() {
        return gap;
    }
}
