package pl.poznan.put.client;

import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.TextField;
import pl.poznan.put.game.Board;

import java.io.IOException;

public class Controller {
    @FXML private Button join;
    @FXML private Button start;
    @FXML private TextField nick;
    private boolean inside;

    public void initialize(){
        nick.textProperty().addListener(new ChangeListener<String>() {

            @Override
            public void changed(ObservableValue<? extends String> observable, String oldValue, String newValue) {
                final boolean isEmpty = newValue.isEmpty();
                join.setDisable(isEmpty);
            }
        });
    }

    @FXML protected void handleJoinButton(ActionEvent event) {
        if (inside) {
            join.setText("Dołącz do pokoju");
            start.setDisable(true);
        }
        else {
            join.setText("Opuść pokój");
            start.setDisable(false);
        }
        inside = !inside;
    }

    @FXML protected void handleStartButton(ActionEvent event) {
        try {
            Board board = new Board();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
