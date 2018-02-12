package sample;

public class Point {
    private boolean gap;
    private double x;
    private double y;

    public Point(double x, double y) {
        this.x = x;
        this.y = y;
    }

    public Point(double x, double y, boolean gap) {
        this.x = x;
        this.y = y;
        this.gap = gap;
    }

    public boolean isGap() {
        return gap;
    }

    public double getX() {
        return x;
    }

    public double getY() {
        return y;
    }
}
