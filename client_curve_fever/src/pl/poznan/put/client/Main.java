package pl.poznan.put.client;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

public class Main extends Application {
    private static final String TITLE = "Curve fever!";
    private static final int WIDTH = 630;
    private static final int HEIGHT = 270;

    @Override
    public void start(Stage primaryStage) throws Exception{
        Parent root = FXMLLoader.load(getClass().getResource("main.fxml"));
        primaryStage.setTitle(TITLE);
        primaryStage.setScene(new Scene(root, WIDTH, HEIGHT));

        primaryStage.show();
    }

    public static void main(String[] args) {
        launch(args);
        UdpClient client = null;
        /*try {
            client = new UdpClient();
            client.send("heheszki");
            for (int i=0; i<10; i++) {
                client.receive();
            }
            client.close();
        } catch (Exception e) {
            e.printStackTrace();
        }*/
        System.out.println("Koniec");
    }

}


/*
private static final double KEYFRAME_DURATION_TIME = 0.026; // seconds
    //private static final int NUMBER_OF_PLAYERS = 2;
    private static final int WIDTH = 900;
    private static final int HEIGHT = 700;
    private static final long START_DRAWING_DELAY = 1500; // milliseconds
    //private static final long MIN_TIME_OF_DRAWING = 4000; // milliseconds
    //private static final long MAX_TIME_OF_DRAWING = 6000; // milliseconds
    //private static final long MIN_TIME_OF_DELAY = 200; // milliseconds
    //private static final long MAX_TIME_OF_DELAY = 400; // milliseconds
    private static final double END_CIRCLE_RADIUS = 1.0;
    private static final double LINE_WIDTH = 2.0;
    private static final double BOUNDS_WIDTH = 4.0;
    private static final String TITLE = "Curve fever!";
    private static final Color BOUNDS_COLOR = Color.BLACK;
    //private static int currentNumberOfPlayers = NUMBER_OF_PLAYERS;
    //private final Timer timer = new Timer();
    //private final Timeline timeline = new Timeline();
    private Group root = new Group();
    private Scene scene = new Scene(root, WIDTH, HEIGHT);
    private Canvas canvas = new Canvas(WIDTH, HEIGHT);
    private GraphicsContext gc = canvas.getGraphicsContext2D();
    //private Player [] player;
 */