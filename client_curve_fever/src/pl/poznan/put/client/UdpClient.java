package pl.poznan.put.client;

import java.io.*;
import java.net.*;
import java.util.Properties;

public class UdpClient {
    private static final int BUF_SIZE = 1024;
    private DatagramSocket sock;
    private InetSocketAddress addr;
    private int port;

    public UdpClient() throws IOException {
        Properties props = new Properties();
        BufferedReader reader = new BufferedReader(new InputStreamReader(UdpClient.class.getResourceAsStream("config.properties")));
        props.load(reader);

        sock = new DatagramSocket();
        port = Integer.parseInt(props.getProperty("udp_port"));
        addr = new InetSocketAddress(props.getProperty("ip"), port);
    }

    public void send(String msg) throws IOException {
        byte[] buf = msg.getBytes();
        DatagramPacket out = new DatagramPacket(buf, buf.length, addr);
        sock.send(out);
    }

    public void close() {
        sock.close();
    }

    /*

    public void turnOn() throws IOException {
        int numberOfPlayers = 0;

        send("turn_on");
        if(receive().equals("number_of_players")) {
            numberOfPlayers = receiveInt();
            System.out.println("Number of players: " + numberOfPlayers);
        }

        if(receive().equals("players")) {
            for (int i=0; i<numberOfPlayers; i++) {
                System.out.println("Player " + (i+1) + ": " + receive());
            }
        }
        send("confirm");
    }

    public void join(String nick) throws IOException {
        send("joinSend");

        if(receive().equals("nick")) {
            send(nick);
        }

        if(receive().equals("confirm")) {
            System.out.println("Nick " + nick +  " was sent successfully!");
        }
    }

    public void ready() throws IOException {
        send("ready");

        if(receive().equals("confirm")) {
            System.out.println("Server received message 'ready'!");
        }
    }

    public void start() throws IOException {
        if(receive().equals("start")) {
            // open gameplay window
            send("confirm");
            System.out.println("Server confirmed start!");
        }
    }

    /*public void receive2() throws IOException {
        byte[] buf = new byte[BUFSIZE];
        DatagramPacket in = new DatagramPacket(buf, buf.length);
        sock.receive(in);
        String modifiedSentence = new String(in.getData());
        System.out.println("FROM SERVER:" + modifiedSentence);
    }*/

    /*final BigInteger bi = BigInteger.valueOf(1);
        byte[] buf = bi.toByteArray();
        DatagramPacket out = new DatagramPacket(buf, buf.length, addr);
        sock.send(out);

        buf = new byte[BUFSIZE];
        DatagramPacket in = new DatagramPacket(buf, buf.length);
        sock.receive(in);
        int numberOfPlayers = new BigInteger(in.getData()).intValue();
        System.out.println("Number of players: " + numberOfPlayers);*/

    /*final BigInteger bi = BigInteger.valueOf(2);
        byte[] buf = bi.toByteArray();
        DatagramPacket out = new DatagramPacket(buf, buf.length, addr);
        sock.send(out);

        buf = new byte[BUFSIZE];
        DatagramPacket in = new DatagramPacket(buf, buf.length);
        sock.receive(in);
        String confirm = new String(in.getData());
        System.out.println("Potwierdzenie: " + confirm);

        buf = nick.getBytes();
        out = new DatagramPacket(buf, buf.length, addr);
        sock.send(out);

        buf = new byte[BUFSIZE];
        in = new DatagramPacket(buf, buf.length);
        sock.receive(in);
        confirm = new String(in.getData());
        System.out.println("Potwierdzenie: " + confirm);*/

    /*public String receive() throws IOException {
        byte[] buf = new byte[BUF_SIZE];
        DatagramPacket in = new DatagramPacket(buf, buf.length);
        sock.receive(in);
        return new String(in.getData());
    }

    public int receiveInt() throws IOException {
        byte[] buf = new byte[BUF_SIZE];
        DatagramPacket in = new DatagramPacket(buf, buf.length);
        sock.receive(in);
        ByteBuffer wrapped = ByteBuffer.wrap(in.getData());
        return wrapped.getInt();
    }


    public void send(String msg) throws IOException {
        byte[] buf = msg.getBytes();
        DatagramPacket out = new DatagramPacket(buf, buf.length, addr);
        sock.send(out);
    } */
}