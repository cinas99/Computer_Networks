package pl.poznan.put.game;

import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.scene.layout.StackPane;
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
import java.util.Timer;

import static java.lang.Math.*;

import pl.poznan.put.client.Message;
import pl.poznan.put.client.TcpClient;
import pl.poznan.put.client.UdpClient;

import java.io.IOException;

public class Board {
    //private static final double KEYFRAME_DURATION_TIME = 0.026; // seconds
    //private static final int NUMBER_OF_PLAYERS = 2;
    private static final int WIDTH = 900;
    private static final int HEIGHT = 700;
    //private static final long START_DRAWING_DELAY = 1500; // milliseconds
    //private static final long MIN_TIME_OF_DRAWING = 4000; // milliseconds
    //private static final long MAX_TIME_OF_DRAWING = 6000; // milliseconds
    //private static final long MIN_TIME_OF_DELAY = 200; // milliseconds
    //private static final long MAX_TIME_OF_DELAY = 400; // milliseconds
    private static final double END_CIRCLE_RADIUS = 1.0;
    private static final double LINE_WIDTH = 2.0;
    private static final double BOUNDS_WIDTH = 4.0;
    private static final String TITLE = "Curve fever!";
    private static final Color BOUNDS_COLOR = Color.BLACK;
    private static int currentNumberOfPlayers;// = NUMBER_OF_PLAYERS;
    //private final Timer timer = new Timer();
    //private final Timeline timeline = new Timeline();
    private Group root = new Group();
    private Scene scene = new Scene(root, WIDTH, HEIGHT);
    private Canvas canvas = new Canvas(WIDTH, HEIGHT);
    private GraphicsContext gc = canvas.getGraphicsContext2D();
    private Player [] player;

    private static final int WAIT_FOR_UDP_CONFIRM = 200; // milliseconds
    private static boolean connectionEstablished = false;
    private TcpClient tcpClient;
    private UdpClient udpClient;
    private Stage primaryStage = new Stage();
    private Label label = new Label("Waiting for server...");

    public Board(TcpClient tcpClient, UdpClient udpClient) throws IOException {
        this.tcpClient = tcpClient;
        this.udpClient = udpClient;

        root.getChildren().add(canvas);
        root.getChildren().add(label);
        primaryStage.setTitle(TITLE);
        primaryStage.setScene(scene);
        primaryStage.show();

        //StackPane layout = new StackPane();
        //layout.getChildren().add(label);

        primaryStage.setOnCloseRequest((WindowEvent event) -> {
            try {
                tcpClient.unreadySend();
            } catch (IOException e) {
                e.printStackTrace();
            }
        });

        /*Parent root = FXMLLoader.load(getClass().getResource("board.fxml"));*/
    }

    public void start() {
        while(!connectionEstablished) {
            try {
                udpClient.sendInt(Message.FIRST_UDP_MESSAGE.ordinal());
                Thread.sleep(WAIT_FOR_UDP_CONFIRM);
            } catch (IOException e) {
                e.printStackTrace();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        label.setText("Server confirmed! Let's start a game!");
        /*try {
            udpClient.send("udp test message");
        } catch (IOException e) {
            e.printStackTrace();
        }*/

        /*root.getChildren().add(canvas);
        // current number of players have to be send
        initPlayers(currentNumberOfPlayers);
        for (int i=0; i < currentNumberOfPlayers; i++) {
            this.drawLines(player[i]);
        }
        // when msg is received
        gc.clearRect(0, 0, WIDTH, HEIGHT);
        for (int i=0; i < currentNumberOfPlayers; i++) {
            drawLines(player[i]);
        }

        scene.setOnKeyPressed(new EventHandler<KeyEvent>() {
            @Override
            public void handle(KeyEvent event) {
                if (event.getCode().ordinal() == KeyCode.LEFT.ordinal()) {
                    // send msg
                    //player[0].setTurn(-1);
                }
                else if (event.getCode().ordinal() == KeyCode.RIGHT.ordinal()) {
                    // send msg
                    //player[0].setTurn(1);
                }
                event.consume();
            }
        });

        scene.setOnKeyReleased(new EventHandler<KeyEvent>() {
            @Override
            public void handle(KeyEvent event) {
                if(event.getCode().ordinal() == KeyCode.LEFT.ordinal()) {
                    // send msg
                    //player[0].setTurn(0);
                }
                else if (event.getCode().ordinal() == KeyCode.RIGHT.ordinal()) {
                    // send msg
                    //player[0].setTurn(0);
                }
                event.consume();
            }
        });

        primaryStage.setTitle(TITLE);
        primaryStage.setScene(scene);
        primaryStage.show();*/
    }

    public static void setConnectionEstablished(boolean isEstablished) {
        connectionEstablished = isEstablished;
    }

    private void showResults() {
        int winnerIndex = -1;
        for (int i = 0; i < currentNumberOfPlayers; i++) {
            if (player[i].isNowPlaying()) {
                winnerIndex = i;
            }
        }
        Alert alert = new Alert(Alert.AlertType.INFORMATION);
        alert.setHeaderText(null);
        alert.setTitle(TITLE);
        alert.setContentText("Player " + (winnerIndex+1) + " won!");
        //timeline.stop();
        alert.show();
    }

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

    public boolean isVisible() {
        return primaryStage.isShowing();
    }

    public void close() {
        primaryStage.close();
    }
}
