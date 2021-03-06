package pl.poznan.put.game;

import javafx.animation.KeyFrame;
import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.stage.Stage;
import javafx.stage.WindowEvent;

import javafx.animation.Timeline;
import javafx.event.EventHandler;
import javafx.scene.Group;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.control.Alert;
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyEvent;
import javafx.scene.paint.Color;

import java.util.List;

import javafx.util.Duration;
import pl.poznan.put.client.TcpClient;
import pl.poznan.put.client.UdpClient;

import java.io.IOException;

public class Board {
    private static final double KEYFRAME_DURATION_TIME = 0.015; // seconds
    private static final double KEY_SEND_TIME = 0.03; //0.01; // seconds
    private static final int WIDTH = 900;
    private static final int HEIGHT = 700;
    private static final double END_CIRCLE_RADIUS = 1.0;
    private static final double LINE_WIDTH = 2.0;
    private static final double BOUNDS_WIDTH = 4.0;
    private static final String TITLE = "Curve fever!";
    private static final Color BOUNDS_COLOR = Color.BLACK;
    private static final Timeline lineDrawer = new Timeline();
    private static final Timeline keySender = new Timeline();
    private Group root = new Group();
    private Scene scene = new Scene(root, WIDTH, HEIGHT);
    private Canvas canvas = new Canvas(WIDTH, HEIGHT);
    private GraphicsContext gc = canvas.getGraphicsContext2D();
    private static int numberOfPlayers;
    private static Player [] player;
    private int myTurn = 0;

    private static final int WAIT_FOR_UDP_CONFIRM = 100; // milliseconds
    private static boolean connectionEstablished = false;
    private TcpClient tcpClient;
    private UdpClient udpClient;
    private Stage primaryStage = new Stage();
    private Label label = new Label("Waiting for server...");

    private static final String LEFT = "L";
    private static final String RIGHT = "R";
    private static final String STRAIGHT = "S";
    private static final String FIRST_MESSAGE = "F";

    public Board(TcpClient tcpClient, UdpClient udpClient) throws IOException {
        this.tcpClient = tcpClient;
        this.udpClient = udpClient;

        root.getChildren().add(canvas);
        //root.getChildren().add(label); TODO LATER
        primaryStage.setTitle(TITLE);
        primaryStage.setScene(scene);
        primaryStage.show();

        primaryStage.setOnCloseRequest((WindowEvent event) -> {
            try {
                tcpClient.unreadySend();
            } catch (IOException e) {
                e.printStackTrace();
            }
        });
    }

    public void start() {
        while(!connectionEstablished) {
            try {
                udpClient.send(FIRST_MESSAGE);

                Thread.sleep(WAIT_FOR_UDP_CONFIRM);
            } catch (IOException e) {
                e.printStackTrace();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        label.setText("Server confirmed! Let's start a game!");

        scene.setOnKeyPressed(new EventHandler<KeyEvent>() {
            @Override
            public void handle(KeyEvent event) {
                if (event.getCode().ordinal() == KeyCode.LEFT.ordinal()) {
                    myTurn = -1;
                }
                else if (event.getCode().ordinal() == KeyCode.RIGHT.ordinal()) {
                    myTurn = 1;
                }
                event.consume();
            }
        });

        scene.setOnKeyReleased(new EventHandler<KeyEvent>() {
            @Override
            public void handle(KeyEvent event) {
                if(event.getCode().ordinal() == KeyCode.LEFT.ordinal()) {
                    myTurn = 0;
                }
                else if (event.getCode().ordinal() == KeyCode.RIGHT.ordinal()) {
                    myTurn = 0;
                }
                event.consume();
            }
        });

        lineDrawer.getKeyFrames().add(new KeyFrame(Duration.seconds(KEYFRAME_DURATION_TIME), new EventHandler<ActionEvent>() {
            @Override
            public void handle(ActionEvent event) {
                gc.clearRect(0, 0, WIDTH, HEIGHT);
                for (int i=0; i < numberOfPlayers; i++) {
                    drawLines(player[i]);
                }
            }
        }));
        lineDrawer.setCycleCount(Timeline.INDEFINITE);
        lineDrawer.play();

        keySender.getKeyFrames().add(new KeyFrame(Duration.seconds(KEY_SEND_TIME), new EventHandler<ActionEvent>() {
            @Override
            public void handle(ActionEvent event) {
                try {
                    if (myTurn == 0) {
                        udpClient.send(STRAIGHT);
                    }
                    else if (myTurn == 1) {
                        udpClient.send(RIGHT);
                    }
                    else if (myTurn == -1) {
                        udpClient.send(LEFT);
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }));
        keySender.setCycleCount(Timeline.INDEFINITE);
        keySender.play();
    }

    public static void setConnectionEstablished(boolean isEstablished) {
        connectionEstablished = isEstablished;
    }

    public static void addPoint(int playerNumber, int pointNumber, double x, double y, boolean isGap) {
        player[playerNumber].addPoint(new PointWrapper(pointNumber, new Point(x, y, isGap)));
    }

    public static void showResults(String winner) {
        Platform.runLater(new Runnable() {
            @Override
            public void run() {
                Alert alert = new Alert(Alert.AlertType.INFORMATION);
                alert.setHeaderText(null);
                alert.setTitle(TITLE);
                alert.setContentText("Player " + winner + " won!");
                alert.show();
            }
        });
        lineDrawer.stop();
        keySender.stop();
    }

    private void drawLines(Player player) {
        // draw boundaries
        gc.setStroke(BOUNDS_COLOR);
        gc.setLineWidth(BOUNDS_WIDTH);
        gc.strokeLine(0, 0, 0, HEIGHT);
        gc.strokeLine(0, HEIGHT, WIDTH, HEIGHT);
        gc.strokeLine(WIDTH, HEIGHT, WIDTH, 0);
        gc.strokeLine(WIDTH, 0, 0, 0);
        List<PointWrapper> visited = player.getVisited();
        if (visited.size() > 1) {
            gc.setStroke(player.getColor());
            gc.setLineWidth(LINE_WIDTH);
            for (int i = 0; i < visited.size() - 1; i++) {
                if (!visited.get(i).getPoint().isGap()) {
                    gc.strokeLine(visited.get(i).getPoint().getX(), visited.get(i).getPoint().getY(),
                            visited.get(i + 1).getPoint().getX(), visited.get(i + 1).getPoint().getY());
                }
            }
        }
        if (player.isNowPlaying()) {
            double r = END_CIRCLE_RADIUS * LINE_WIDTH;
            gc.setFill(player.getColor());
            gc.fillOval(player.getCurrentX() - r, player.getCurrentY() - r, 2 * r, 2 * r);
        }
    }

    public static void initPlayers(final int maxNumberOfPlayers) {
        numberOfPlayers = maxNumberOfPlayers;
        player = new Player[numberOfPlayers];
        if (numberOfPlayers == 1) {
            player[0] = new Player(Color.RED);
        }
        else if (numberOfPlayers == 2) {
            player[0] = new Player(Color.RED);
            player[1] = new Player(Color.BLUE);
        }
        else if (numberOfPlayers == 3) {
            player[0] = new Player(Color.RED);
            player[1] = new Player(Color.BLUE);
            player[2] = new Player(Color.GREEN);
        }
        else if (numberOfPlayers == 4) {
            player[0] = new Player(Color.RED);
            player[1] = new Player(Color.BLUE);
            player[2] = new Player(Color.GREEN);
            player[3] = new Player(Color.DARKTURQUOISE);
        }
    }

    public boolean isVisible() {
        return primaryStage.isShowing();
    }

    public void close() {
        primaryStage.close();
    }
}
