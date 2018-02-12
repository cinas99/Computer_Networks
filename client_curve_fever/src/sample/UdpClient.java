package sample;

import java.io.IOException;
import java.io.OutputStream;
import java.net.*;

public class UdpClient {
    private DatagramSocket sock;
    private InetSocketAddress addr;
    private int port;

    public UdpClient() throws SocketException, UnknownHostException {
        sock = new DatagramSocket();
        port = 9020;
        addr = new InetSocketAddress("192.168.0.21", port);
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