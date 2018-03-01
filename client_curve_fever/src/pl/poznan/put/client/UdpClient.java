package pl.poznan.put.client;

import java.io.*;
import java.net.*;
import java.util.Properties;

public class UdpClient {
    private static final int BUF_SIZE = 1024;
    private DatagramSocket sock;
    private InetSocketAddress addr;
    private int port;
    private ReceivingUdpThread receivingUdpThread;

    class ReceivingUdpThread extends Thread {

        @Override
        public void run() {
            System.out.println("Udp receive: is running!");
            while (true) {
                try {
                    String msg = receive();
                    System.out.println("Udp receive: (message) " + msg);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public UdpClient() throws IOException {
        Properties props = new Properties();
        BufferedReader reader = new BufferedReader(new InputStreamReader(UdpClient.class.getResourceAsStream("config.properties")));
        props.load(reader);

        sock = new DatagramSocket();
        port = Integer.parseInt(props.getProperty("udp_port"));
        addr = new InetSocketAddress(props.getProperty("ip"), port);
    }

    public void startReceiving() {
        receivingUdpThread = new ReceivingUdpThread();
        receivingUdpThread.setDaemon(true);
        receivingUdpThread.start();
    }

    public void send(String msg) throws IOException {
        msg += '\0';
        byte[] buf = msg.getBytes();
        DatagramPacket out = new DatagramPacket(buf, buf.length, addr);
        sock.send(out);
    }

    public String receive() throws IOException {
        byte[] buf = new byte[BUF_SIZE];
        DatagramPacket in = new DatagramPacket(buf, BUF_SIZE);
        sock.receive(in);
        return new String(in.getData());
    }

    public void close() {
        sock.close();
    }
}