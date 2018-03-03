package pl.poznan.put.game;

public class PointWrapper {
    private int number;
    private Point point;

    public PointWrapper(int number, Point point) {
        this.number = number;
        this.point = point;
    }

    public int getNumber() {
        return number;
    }

    public void setNumber(int number) {
        this.number = number;
    }

    public Point getPoint() {
        return point;
    }

    public void setPoint(Point point) {
        this.point = point;
    }
}
