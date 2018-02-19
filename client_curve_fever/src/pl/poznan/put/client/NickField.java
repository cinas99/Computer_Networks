package pl.poznan.put.client;

public class NickField {
    private String nick;
    private boolean isReady;

    public NickField(String nick, boolean isReady) {
        this.nick = nick;
        this.isReady = isReady;
    }

    public String getNick() {
        return nick;
    }

    public boolean isReady() {
        return isReady;
    }
}
