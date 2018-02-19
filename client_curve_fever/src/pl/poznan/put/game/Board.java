package pl.poznan.put.game;

import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.scene.layout.StackPane;
import javafx.stage.Stage;
import javafx.stage.WindowEvent;
import pl.poznan.put.client.TcpClient;

import java.io.IOException;

public class Board {
    private TcpClient tcpClient;
    private Stage stage;

    public Board(TcpClient tcpClient) throws IOException {
        Label secondLabel = new Label("I'm a Label on new Window");

        StackPane secondaryLayout = new StackPane();
        secondaryLayout.getChildren().add(secondLabel);

        Scene secondScene = new Scene(secondaryLayout, 230, 100);

        // New window (Stage)
        stage = new Stage();
        stage.setTitle("Second Stage");
        stage.setScene(secondScene);

        stage.show();

        stage.setOnCloseRequest((WindowEvent event) -> {
            try {
                tcpClient.unreadySend();
            } catch (IOException e) {
                e.printStackTrace();
            }
        });

        /*Parent root = FXMLLoader.load(getClass().getResource("board.fxml"));
        Scene scene = new Scene(root, 600, 400);
        Stage stage = new Stage();
        stage.setTitle("New Window");
        stage.setScene(scene);
        stage.show();*/
    }

    public void close() {
        stage.close();
    }

    public boolean isVisible() {
        return stage.isShowing();
    }
}
