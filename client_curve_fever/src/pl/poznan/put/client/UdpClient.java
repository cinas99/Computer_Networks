package pl.poznan.put.client;

import java.io.*;
import java.net.*;
import java.util.Properties;

public class UdpClient {
    private DatagramSocket sock;
    private InetSocketAddress addr;
    private int port;

    public UdpClient() throws IOException {
        Properties props = new Properties();
        BufferedReader reader = new BufferedReader(new InputStreamReader(UdpClient.class.getResourceAsStream("config.properties")));
        props.load(reader);

        sock = new DatagramSocket();
        port = Integer.parseInt(props.getProperty("port"));
        addr = new InetSocketAddress(props.getProperty("ip"), port);
    }

    public void receive() throws IOException {
        byte[] buf = new byte[1024];
        DatagramPacket in = new DatagramPacket(buf, buf.length);
        sock.receive(in);
        String modifiedSentence = new String(in.getData());
        System.out.println("FROM SERVER:" + modifiedSentence);
    }

    public void send(String msg) throws IOException {
        byte[] buf = msg.getBytes();
        DatagramPacket out = new DatagramPacket(buf, buf.length, addr);
        sock.send(out);
    }

    public void close() {
        sock.close();
    }
}