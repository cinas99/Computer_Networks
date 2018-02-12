package sample;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

public class Main extends Application {

    @Override
    public void start(Stage primaryStage) throws Exception{
        Parent root = FXMLLoader.load(getClass().getResource("sample.fxml"));
        primaryStage.setTitle("Hello World");
        primaryStage.setScene(new Scene(root, 300, 275));
        primaryStage.show();
    }


    public static void main(String[] args) {
        //launch(args);
        UdpClient client = null;
        try {
            client = new UdpClient();
            client.send("heheszki");
            for (int i=0; i<10; i++) {
                client.receive();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        client.close();
        System.out.println("Koniec");
    }
}
