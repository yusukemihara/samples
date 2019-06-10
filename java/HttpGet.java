import java.io.*;
import java.net.HttpURLConnection;
import java.net.URL;

public class HttpGet {

    public static void main(String[] args) throws Exception {
        URL url = new URL(args[0]);
        HttpURLConnection con = (HttpURLConnection) url.openConnection();
        con.setInstanceFollowRedirects(false);
        con.setRequestMethod("GET");
        con.connect();
        if (con.getResponseCode() != HttpURLConnection.HTTP_OK) {
            con.disconnect();
            throw new IOException("" + con.getResponseCode());
        }
        File tmp = new File("/tmp/out");
        BufferedInputStream in = new BufferedInputStream(con.getInputStream());
        int length;
        try (BufferedOutputStream out = new BufferedOutputStream(new FileOutputStream(tmp))) {
            while ((length = in.read()) != -1) {
                out.write(length);
            }
        }
        con.disconnect();
    }
}
