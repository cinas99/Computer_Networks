package pl.poznan.put.client;

import javafx.application.Platform;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.TextField;
import pl.poznan.put.game.Board;

import java.io.IOException;
import java.util.List;

public class Controller {
    @FXML private Button join;
    @FXML private Button start;
    @FXML private TextField nick;
    @FXML private TextField room1;
    @FXML private TextField room2;
    @FXML private TextField room3;
    @FXML private TextField room4;
    private TcpClient tcpClient;
    //private UdpClient client;
    private boolean inside;

    public void initialize(){
        try {
            tcpClient = new TcpClient(this);
            tcpClient.turnOnSend();
        } catch (Exception e) {
            e.printStackTrace();
        }

        nick.textProperty().addListener(new ChangeListener<String>() {

            @Override
            public void changed(ObservableValue<? extends String> observable, String oldValue, String newValue) {
                final boolean isEmpty = newValue.isEmpty();
                join.setDisable(isEmpty);
            }
        });

        Platform.runLater(new Runnable() {
            public void run() {
                nick.requestFocus();
            }
        });
    }

    public void shutdown() {
        try {
            tcpClient.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @FXML protected void handleJoinButton(ActionEvent event) {
        try {
            if (inside) {
                join.setText("Dołącz do pokoju");
                start.setDisable(true);
                nick.setDisable(false);
                tcpClient.leaveSend();
            }
            else {
                join.setText("Opuść pokój");
                start.setDisable(false);
                nick.setDisable(true);
                tcpClient.joinSend(nick.getText());
            }
            inside = !inside;
        }
        catch (IOException e) {
            e.printStackTrace();
        }
    }

    @FXML protected void handleStartButton(ActionEvent event) {
        try {
            tcpClient.readySend();
            Board board = new Board();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    protected void refreshRoom(List<String> playerNicks) {
        int size = playerNicks.size();
        room1.setText(size < 1 ? "" : playerNicks.get(0));
        //room1.setStyle("-fx-text-fill: green;");
        room2.setText(size < 2 ? "" : playerNicks.get(1));
        room3.setText(size < 3 ? "" : playerNicks.get(2));
        room4.setText(size < 4 ? "" : playerNicks.get(3));
    }
}