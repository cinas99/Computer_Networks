package sample;

import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.application.Application;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.control.Alert;
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyEvent;
import javafx.scene.paint.Color;
import javafx.stage.Stage;
import javafx.util.Duration;

import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

import static java.lang.Math.*;


public class Board extends Application {
    private static final double KEYFRAME_DURATION_TIME = 0.026; // seconds
    private static final int NUMBER_OF_PLAYERS = 2;
    private static final int WIDTH = 900;
    private static final int HEIGHT = 700;
    private static final long START_DRAWING_DELAY = 1500; // milliseconds
    private static final long MIN_TIME_OF_DRAWING = 4000; // milliseconds
    private static final long MAX_TIME_OF_DRAWING = 6000; // milliseconds
    private static final long MIN_TIME_OF_DELAY = 200; // milliseconds
    private static final long MAX_TIME_OF_DELAY = 400; // milliseconds
    //private static final double END_CIRCLE_RADIUS = 1.0;
    //private static final double LINE_WIDTH = 2.0;
    //private static final double BOUNDS_WIDTH = 4.0;
    //private static final String TITLE = "Curve fever!";
    //private static final Color BOUNDS_COLOR = Color.BLACK;
    private static int currentNumberOfPlayers = NUMBER_OF_PLAYERS;
    private final Timer timer = new Timer();
    private final Timeline timeline = new Timeline();
    //private Group root = new Group();
    //private Scene scene = new Scene(root, WIDTH, HEIGHT);
    //private Canvas canvas = new Canvas(WIDTH, HEIGHT);
    //private GraphicsContext gc = canvas.getGraphicsContext2D();
    private Player [] player;

    // WHOLE METHOD MINE COPY PASTE

    private void startDrawing() {
        for (Player p : player) {
            p.setDraw(true);
        }
        final long time = Math.round(MIN_TIME_OF_DRAWING + random() * (MAX_TIME_OF_DRAWING - MIN_TIME_OF_DRAWING));
        timer.schedule(new TimerTask() {

            @Override
            public void run() {
                stopDrawing();
            }
        }, time);
    }

    // WHOLE METHOD MINE COPY PASTE

    private void stopDrawing() {
        for (Player p : player) {
            p.markGap();
            p.setDraw(false);
        }
        final long time = Math.round(MIN_TIME_OF_DELAY + random() * (MAX_TIME_OF_DELAY - MIN_TIME_OF_DELAY));
        timer.schedule(new TimerTask() {

            @Override
            public void run() {
                startDrawing();
            }
        }, time);
    }

    // WHOLE METHOD MINE COPY PASTE

    private static boolean outOfBounds(Point p) {
        return (p.getX() > WIDTH || p.getX() < 0 || p.getY() > HEIGHT || p.getY() < 0);
    }

    // WHOLE METHOD MINE COPY PASTE//

    private static boolean areIntersecting(Point p1, Point p2, Point q1, Point q2) {
        final boolean result;
        // special case when player move out of map
        if (outOfBounds(q2)) {
            result = true;
        }
        // other cases when sections are intersecting
        else if (p1.getX() != p2.getX() && q1.getX() != q2.getX()) {
            // count a, b parameters for two lines
            double pa = (p1.getY() - p2.getY()) / (p1.getX() - p2.getX());
            double pb = p1.getY() - pa * p1.getX();
            double qa = (q1.getY() - q2.getY()) / (q1.getX() - q2.getX());
            double qb = q1.getY() - qa * q1.getX();
            // point of intersection
            double x = (qb - pb) / (pa - qa);
            double y = pa * x + pb;
            // check if the point belongs to the sections
            boolean pCondition = min(p1.getX(), p2.getX()) <= x && x <= max(p1.getX(), p2.getX())
                    && min(p1.getY(), p2.getY()) <= y && y <= max(p1.getY(), p2.getY());
            boolean qCondition = min(q1.getX(), q2.getX()) <= x && x <= max(q1.getX(), q2.getX())
                    && min(q1.getY(), q2.getY()) <= y && y <= max(q1.getY(), q2.getY());
            result = pCondition && qCondition;
        }
        else if (p1.getX() == p2.getX() && q1.getX() == q2.getX()) {
            result = p1.getX() == q1.getX() && (min(p1.getY(), p2.getY()) <= max(q1.getY(), q2.getY())
                    || min(q1.getY(), q2.getY()) <= max(p1.getY(), p2.getY()));
        }
        else if (p1.getX() == p2.getX()) {
            double qa = (q1.getY() - q2.getY()) / (q1.getX() - q2.getX());
            double qb = q1.getY() - qa * q1.getX();
            double x = p1.getX();
            double y = qa * x + qb;
            boolean pCondition = min(p1.getY(), p2.getY()) <= x && x <= max(p1.getY(), p2.getY());
            boolean qCondition = min(q1.getX(), q2.getX()) <= x && x <= max(q1.getX(), q2.getX())
                    && min(q1.getY(), q2.getY()) <= y && y <= max(q1.getY(), q2.getY());
            result = pCondition && qCondition;
        }
        else { // (q1.getX() == q2.getX())
            double pa = (p1.getY() - p2.getY()) / (p1.getX() - p2.getX());
            double pb = p1.getY() - pa * p1.getX();
            double x = q1.getX();
            double y = pa * q1.getX() + pb;
            boolean pCondition = min(p1.getX(), p2.getX()) <= x && x <= max(p1.getX(), p2.getX())
                    && min(p1.getY(), p2.getY()) <= y && y <= max(p1.getY(), p2.getY());
            boolean qCondition = min(q2.getX(), q2.getY()) <= x && x <= max(q2.getX(), q2.getY());
            result = pCondition && qCondition;
        }
        return result;
    }

    // ? w Serwerze //TODO LATER //

    private void showResults() {
        int winnerIndex = -1;
        for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
            if (player[i].isNowPlaying()) {
                winnerIndex = i;
            }
        }
        Alert alert = new Alert(Alert.AlertType.INFORMATION);
        alert.setHeaderText(null);
        alert.setTitle(TITLE);
        alert.setContentText("Player " + (winnerIndex+1) + " won!");
        timeline.stop();
        alert.show();
    }

    // WHOLE METHOD MINE //

    private void checkCollision() {
        for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
            if (!player[i].isNowPlaying()) continue;
            final List <Point> iVisited = player[i].getVisited(); // wektory !
            if (iVisited.size() > 1) {
                Point last = iVisited.get(iVisited.size() - 1);
                Point nextToLast = iVisited.get(iVisited.size() - 2);
                for (int j = 0; j < NUMBER_OF_PLAYERS; j++) {
                    final List <Point> jVisited = player[j].getVisited();
                    final int sizeToCheck = (i == j) ? jVisited.size() - 3 : jVisited.size() - 1;
                    for (int k = 0; k < sizeToCheck; k++) {
                        Point p1 = jVisited.get(k);
                        Point p2 = jVisited.get(k + 1);
                        if (!p1.isGap() && areIntersecting(p1, p2, nextToLast, last)) {
                            player[i].setNowPlaying(false);
                            if (--currentNumberOfPlayers <= 1) {
                                this.showResults();
                                return;
                            }
                        }
                    }
                }
            }
        }
        // to know number of pts to check
        int mySize = 0;
        for (int i=0; i<NUMBER_OF_PLAYERS; i++) {
            mySize += player[i].getVisited().size();
        }
        System.out.println(mySize);
    }

    /*
    private void drawLines(Player player) {
        // draw boundaries
        gc.setStroke(BOUNDS_COLOR);
        gc.setLineWidth(BOUNDS_WIDTH);
        gc.strokeLine(0, 0, 0, HEIGHT);
        gc.strokeLine(0, HEIGHT, WIDTH, HEIGHT);
        gc.strokeLine(WIDTH, HEIGHT, WIDTH, 0);
        gc.strokeLine(WIDTH, 0, 0, 0);
        List<Point> visited = player.getVisited();
        if (visited.size() > 1) {
            gc.setStroke(player.getColor());
            gc.setLineWidth(LINE_WIDTH);
            for (int i = 0; i < visited.size() - 1; i++) {
                if (!visited.get(i).isGap()) {
                    gc.strokeLine(visited.get(i).getX(), visited.get(i).getY(), visited.get(i + 1).getX(), visited.get(i + 1).getY());
                }
            }
        }
        if (player.isNowPlaying()) {
            double r = END_CIRCLE_RADIUS * LINE_WIDTH;
            gc.setFill(player.getColor());
            gc.fillOval(player.getCurrentX() - r, player.getCurrentY() - r, 2 * r, 2 * r);
        }
    }
        */

    // WHOLE METHOD MINE //

    private void initPlayers(final int maxNumberOfPlayers) {
        player = new Player[maxNumberOfPlayers];
        if (maxNumberOfPlayers == 1) {
            player[0] = new Player(WIDTH/2.0, HEIGHT/2.0, random()*2*PI, Color.RED);
        }
        else if (maxNumberOfPlayers == 2) {
            player[0] = new Player(WIDTH/3.0, HEIGHT/2.0, random()*2*PI, Color.RED);
            player[1] = new Player(2*WIDTH/3.0, HEIGHT/2.0, random()*2*PI, Color.BLUE);
        }
        else if (maxNumberOfPlayers == 3) {
            player[0] = new Player(WIDTH/3.0, HEIGHT/2.0, random()*2*PI, Color.RED);
            player[1] = new Player(2*WIDTH/3.0, HEIGHT/2.0, random()*2*PI, Color.BLUE);
            player[2] = new Player(WIDTH/2.0, 2*HEIGHT/3.0, random()*2*PI, Color.GREEN);
        }
        else if (maxNumberOfPlayers == 4) {
            player[0] = new Player(WIDTH/3.0, HEIGHT/3.0, random()*2*PI, Color.RED);
            player[1] = new Player(2*WIDTH/3.0, HEIGHT/3.0, random()*2*PI, Color.BLUE);
            player[2] = new Player(WIDTH/3.0, 2*HEIGHT/3.0, random()*2*PI, Color.GREEN);
            player[3] = new Player(2*WIDTH/3.0, 2*HEIGHT/3.0, random()*2*PI, Color.DARKTURQUOISE);
        }
    }

    @Override
    public void start(Stage primaryStage) throws Exception {
        //root.getChildren().add(canvas);
        initPlayers(NUMBER_OF_PLAYERS);
        timer.schedule(new TimerTask() {

            @Override
            public void run() {
                startDrawing();
            }
        }, START_DRAWING_DELAY);
        //for (int i=0; i < NUMBER_OF_PLAYERS; i++) {
        //    this.drawLines(player[i]);
        //}
        timeline.getKeyFrames().add(new KeyFrame(Duration.seconds(KEYFRAME_DURATION_TIME), new EventHandler<ActionEvent>() {
            @Override
            public void handle(ActionEvent event) {
                for (int i=0; i < NUMBER_OF_PLAYERS; i++) {
                    player[i].generateNextLine();
                }
                checkCollision();
               // gc.clearRect(0, 0, WIDTH, HEIGHT);
               // for (int i=0; i < NUMBER_OF_PLAYERS; i++) {
                //    drawLines(player[i]);
               // }
            }
        }));
        timeline.setCycleCount(Timeline.INDEFINITE);

        /*scene.setOnKeyPressed(new EventHandler<KeyEvent>() {
            @Override
            public void handle(KeyEvent event) {
                if (event.getCode().ordinal() == KeyCode.LEFT.ordinal() && NUMBER_OF_PLAYERS > 0) {
                    player[0].setTurn(-1);
                }
                else if (event.getCode().ordinal() == KeyCode.RIGHT.ordinal() && NUMBER_OF_PLAYERS > 0) {
                    player[0].setTurn(1);
                }
                if (event.getCode().ordinal() == KeyCode.A.ordinal() && NUMBER_OF_PLAYERS > 1) {
                    player[1].setTurn(-1);
                }
                else if (event.getCode().ordinal() == KeyCode.D.ordinal() && NUMBER_OF_PLAYERS > 1) {
                    player[1].setTurn(1);
                }
                if (event.getCode().ordinal() == KeyCode.N.ordinal() && NUMBER_OF_PLAYERS > 2) {
                    player[2].setTurn(-1);
                }
                else if (event.getCode().ordinal() == KeyCode.M.ordinal() && NUMBER_OF_PLAYERS > 2) {
                    player[2].setTurn(1);
                }
                if (event.getCode().ordinal() == KeyCode.NUMPAD8.ordinal() && NUMBER_OF_PLAYERS > 3) {
                    player[3].setTurn(-1);
                }
                else if (event.getCode().ordinal() == KeyCode.NUMPAD9.ordinal() && NUMBER_OF_PLAYERS > 3) {
                    player[3].setTurn(1);
                }
                event.consume();
            }
        });

        scene.setOnKeyReleased(new EventHandler<KeyEvent>() {
            @Override
            public void handle(KeyEvent event) {
                if(event.getCode().ordinal() == KeyCode.LEFT.ordinal() && NUMBER_OF_PLAYERS > 0) {
                    player[0].setTurn(0);
                }
                else if (event.getCode().ordinal() == KeyCode.RIGHT.ordinal() && NUMBER_OF_PLAYERS > 0) {
                    player[0].setTurn(0);
                }
                if(event.getCode().ordinal() == KeyCode.A.ordinal() && NUMBER_OF_PLAYERS > 1) {
                    player[1].setTurn(0);
                }
                else if (event.getCode().ordinal() == KeyCode.D.ordinal() && NUMBER_OF_PLAYERS > 1) {
                    player[1].setTurn(0);
                }
                if(event.getCode().ordinal() == KeyCode.N.ordinal() && NUMBER_OF_PLAYERS > 2) {
                    player[2].setTurn(0);
                }
                else if (event.getCode().ordinal() == KeyCode.M.ordinal() && NUMBER_OF_PLAYERS > 2) {
                    player[2].setTurn(0);
                }
                if(event.getCode().ordinal() == KeyCode.NUMPAD8.ordinal() && NUMBER_OF_PLAYERS > 3) {
                    player[3].setTurn(0);
                }
                else if (event.getCode().ordinal() == KeyCode.NUMPAD9.ordinal() && NUMBER_OF_PLAYERS > 3) {
                    player[3].setTurn(0);
                }
                event.consume();
            }
        });
*/
      //  primaryStage.setTitle(TITLE);
       // primaryStage.setScene(scene);
       // primaryStage.show();


        timeline.play();


    }
}