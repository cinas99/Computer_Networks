package pl.poznan.put.client;

import java.io.*;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;
import java.util.Properties;

public class TcpClient {
    private static final int BUF_SIZE = 1024;
    private Controller controller;
    private Socket sock;
    private DataInputStream dataInputStream;
    private DataOutputStream dataOutputStream;

    class ReceivingThread extends Thread {

        @Override
        public void run() {
            try {
                while (true) {
                    int msg = receiveInt();
                    System.out.println("ReceivingThread: (message) " + msg);
                    Message message = Message.values()[msg];
                    switch (message) {
                        case TURN_ON:
                            System.out.println("ReceivingThread: Connected to server!\n");
                            break;
                        case JOIN:
                            System.out.println("ReceivingThread: Joined to room!\n");
                            break;
                        case ROOM_EVENT:
                            System.out.println("ReceivingThread: Room event received!\n");
                            TcpClient.this.roomEventReceive();
                            break;
                    }
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public TcpClient(Controller controller) throws IOException {
        this.controller = controller;
        Properties props = new Properties();
        BufferedReader reader = new BufferedReader(new InputStreamReader(UdpClient.class.getResourceAsStream("config.properties")));
        props.load(reader);

        sock = new Socket();
        int port = Integer.parseInt(props.getProperty("port"));
        String ip = props.getProperty("ip");
        sock.connect(new InetSocketAddress(ip, port));
        dataInputStream = new DataInputStream(sock.getInputStream());
        dataOutputStream = new DataOutputStream(sock.getOutputStream());
    }

    private List <String> getPlayerNicks() throws IOException {
        int numberOfPlayers = receiveInt();
        List <String> playerNicks = new ArrayList<String>();
        System.out.println("SendingThread: (number of players) " + numberOfPlayers);
        for (int i=0; i<numberOfPlayers; i++) {
            String nick = receive();
            playerNicks.add(nick);
            System.out.println("SendingThread: (nr) " + (i+1) + " (player) " + nick);
        }
        return playerNicks;
    }

    public void turnOnSend() throws IOException {
        System.out.println("SendingThread: Turn on");
        Thread receivingThread = new ReceivingThread();
        receivingThread.setDaemon(true);
        receivingThread.start();
        sendInt(Message.TURN_ON.ordinal());
    }

    public void joinSend(String nick) throws IOException {
        sendInt(Message.JOIN.ordinal());
        send(nick);
    }

    public void leaveSend() throws IOException {
        sendInt(Message.LEAVE.ordinal());
    }

    public void readySend() throws IOException {
        sendInt(Message.READY.ordinal());
    }

    public void roomEventReceive() throws IOException {
        controller.refreshRoom(getPlayerNicks());
    }

    public void send(String msg) throws IOException {
        dataOutputStream.flush();
        msg += '\0';
        sendInt(msg.length());
        dataOutputStream.write(msg.getBytes());
    }

    public String receive() throws IOException {
        byte[] buf = new byte[BUF_SIZE];
        dataInputStream.read(buf);
        return new String(buf);
    }

    public void sendInt(int num) throws IOException {
        dataOutputStream.flush();
        dataOutputStream.writeInt(num);
    }

    public int receiveInt() throws IOException {
        return dataInputStream.readInt();
    }

    public void close() throws IOException {
        sock.close();
    }
}
