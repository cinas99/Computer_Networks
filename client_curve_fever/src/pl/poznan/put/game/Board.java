package pl.poznan.put.game;

import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.scene.layout.StackPane;
import javafx.stage.Stage;
import javafx.stage.WindowEvent;
import pl.poznan.put.client.TcpClient;
import pl.poznan.put.client.UdpClient;

import java.io.IOException;

public class Board {
    private TcpClient tcpClient;
    private UdpClient udpClient;
    private Stage stage;
    private Label label;

    public Board(TcpClient tcpClient, UdpClient udpClient) throws IOException {
        this.tcpClient = tcpClient;
        this.udpClient = udpClient;

        label = new Label("Waiting for server...");

        StackPane layout = new StackPane();
        layout.getChildren().add(label);

        Scene scene = new Scene(layout, 230, 100);

        stage = new Stage();
        stage.setTitle("Stage");
        stage.setScene(scene);

        stage.show();

        stage.setOnCloseRequest((WindowEvent event) -> {
            try {
                tcpClient.unreadySend();
            } catch (IOException e) {
                e.printStackTrace();
            }
        });

        /*Parent root = FXMLLoader.load(getClass().getResource("board.fxml"));*/
    }

    public void start() {
        label.setText("Server confirmed! Let's start a game!");
        try {
            udpClient.send("udp test message");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public boolean isVisible() {
        return stage.isShowing();
    }

    public void close() {
        stage.close();
    }
}
