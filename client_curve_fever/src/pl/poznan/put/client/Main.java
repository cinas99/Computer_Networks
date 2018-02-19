package pl.poznan.put.client;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

public class Main extends Application {
    private static final String TITLE = "Curve fever! - Main Menu";
    private static final int WIDTH = 630;
    private static final int HEIGHT = 270;
    private static FXMLLoader loader;

    @Override
    public void start(Stage primaryStage) throws Exception{
        loader = new FXMLLoader(getClass().getResource("main.fxml"));
        Parent root = loader.load(); //FXMLLoader.load(getClass().getResource("main.fxml"));
        primaryStage.setTitle(TITLE);
        primaryStage.setScene(new Scene(root, WIDTH, HEIGHT));

        primaryStage.show();
    }

    @Override
    public void stop() {
        Controller controller = loader.getController();
        controller.shutdown();
    }

    public static void main(String[] args) {
        launch(args);
        System.out.println("Finish");
    }

}