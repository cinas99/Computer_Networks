package pl.poznan.put.game;
import javafx.scene.paint.Color;

import java.util.ArrayList;
import java.util.List;

import static java.lang.Math.cos;
import static java.lang.Math.sin;

public class Player {
    private final List <PointWrapper> visited = new ArrayList<PointWrapper>();
    private final Color color;
    private double currentX;
    private double currentY;
    private boolean nowPlaying;

    public Player(Color color) {
        this.color = color;
        this.nowPlaying = true;
    }

    public void addPoint(PointWrapper pointWrapper) {
        currentX = pointWrapper.getPoint().getX();
        currentY = pointWrapper.getPoint().getY();
        visited.add(pointWrapper);
    }

    public double getCurrentX() {
        return currentX;
    }

    public double getCurrentY() {
        return currentY;
    }

    public List<PointWrapper> getVisited() {
        return visited;
    }

    public Color getColor() {
        return color;
    }

    public boolean isNowPlaying() {
        return nowPlaying;
    }

}
