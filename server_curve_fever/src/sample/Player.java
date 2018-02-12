package sample;

import javafx.scene.paint.Color;

import java.util.ArrayList;
import java.util.List;

import static java.lang.Math.cos;
import static java.lang.Math.sin;

//////////////// CALA KLASA COPY PASTE ///////////////////////////


public class Player {
    private final static double LINEAR_SPEED = 2.8;
    private final static double CIRCULAR_SPEED = 0.12;
    private final List <Point> visited = new ArrayList<Point>(); // wektor typu point
    private final Color color;
    private double currentX;
    private double currentY;
    private double angle;
    private int turn;
    private boolean draw;
    private boolean nowPlaying;

    public Player(double startX, double startY, double angle, Color color) {
        this.currentX = startX;
        this.currentY = startY;
        this.angle = angle;
        this.draw = false;
        this.color = color;
        this.nowPlaying = true;
        if (draw) {
            visited.add(new Point(currentX, currentY));
        }
    }

    public void generateNextLine() {
        if (turn == -1) {
            angle -= CIRCULAR_SPEED;
        }
        else if (turn == 1) {
            angle += CIRCULAR_SPEED;
        }
        currentX += LINEAR_SPEED * cos(angle);
        currentY += LINEAR_SPEED * sin(angle);
        if (draw && nowPlaying) {
            visited.add(new Point(currentX, currentY));
        }
    }

    public void markGap() {
        if (nowPlaying) {
            final int size = visited.size();
            final Point last = visited.get(size-1);
            visited.set(size-1, new Point(last.getX(), last.getY(), true));
        }
    }

    public double getCurrentX() {
        return currentX;
    }

    public double getCurrentY() {
        return currentY;
    }

    public void setTurn(int turn) {
        this.turn = turn;
    }

    public void setDraw(boolean draw) {
        this.draw = draw;
    }

    public List<Point> getVisited() {
        return visited;
    }

    public Color getColor() {
        return color;
    }

    public boolean isNowPlaying() {
        return nowPlaying;
    }

    public void setNowPlaying(boolean nowPlaying) {
        this.nowPlaying = nowPlaying;
    }
}
