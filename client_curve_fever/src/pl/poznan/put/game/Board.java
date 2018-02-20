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
    private Label label;

    public Board(TcpClient tcpClient) throws IOException {
        label = new Label("Waiting for server...");

        StackPane secondaryLayout = new StackPane();
        secondaryLayout.getChildren().add(label);

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

    public void start() {
        label.setText("Server confirmed! Let's start a game!");
    }

    public boolean isVisible() {
        return stage.isShowing();
    }

    public void close() {
        stage.close();
    }
}
