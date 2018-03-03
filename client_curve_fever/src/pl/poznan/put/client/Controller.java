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
    @FXML private TextField roomPlace1;
    @FXML private TextField roomPlace2;
    @FXML private TextField roomPlace3;
    @FXML private TextField roomPlace4;
    private TcpClient tcpClient;
    private UdpClient udpClient;
    private Board board;
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
            tcpClient.disconnectSend();
            //tcpClient.close();
            //udpClient.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @FXML protected void handleJoinButton(ActionEvent event) {
        try {
            if (inside) {
                tcpClient.leaveSend();
                join.setText("Dołącz do pokoju");
                start.setDisable(true);
                nick.setDisable(false);
                if (board != null) {
                    board.close();
                }
                inside = false;
            }
            else {
                tcpClient.joinSend(nick.getText());
                nick.setDisable(true);
            }
        }
        catch (IOException e) {
            e.printStackTrace();
        }
    }

    @FXML protected void handleStartButton(ActionEvent event) {
        try {
            tcpClient.readySend();
            udpClient = new UdpClient();
            udpClient.startReceiving();
            if (board == null || !board.isVisible()) {
                board = new Board(tcpClient, udpClient);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    protected void handleUnjoinRoom() {
        Platform.runLater(new Runnable() {
            @Override
            public void run() {
                join.setText("Dołącz do pokoju");
                start.setDisable(true);
                nick.setDisable(false);
                inside = false;
            }
        });
    }

    protected void handleJoinRoom() {
        Platform.runLater(new Runnable() {
            @Override
            public void run() {
                join.setText("Opuść pokój");
                start.setDisable(false);
                //nick.setDisable(true);
                inside = true;
            }
        });
    }

    protected void startGame() {
        Platform.runLater(new Runnable() {
            @Override
            public void run() {
                if (board != null) {
                    board.start();
                }
            }
        });
    }

    protected void refreshRoom(List<NickField> nickFields) {
        int size = nickFields.size();
        refreshTextField(roomPlace1, nickFields,0, size);
        refreshTextField(roomPlace2, nickFields,1, size);
        refreshTextField(roomPlace3, nickFields,2, size);
        refreshTextField(roomPlace4, nickFields,3, size);
    }

    private void refreshTextField(TextField textField, List<NickField> nickFields, int num, int size) {
        if (num < size) {
            final NickField nickField = nickFields.get(num);
            textField.setText(nickField.getNick());
            if (nickField.isReady())
                textField.setStyle("-fx-text-fill: green;");
            else
                textField.setStyle("-fx-text-fill: black;");
        }
        else {
            textField.setText("");
        }
    }

    /*@FXML protected void handleButton() {
        nick.setDisable(false);
        String msg = nick.getText();
        try {
            udpClient.send(msg);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }*/
}