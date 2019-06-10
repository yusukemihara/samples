/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.net.HttpURLConnection;
import java.net.URL;

/**
 *
 * @author mihara
 */
public class HttpAccess {

    public static InputStream get(String strURL) throws IOException {
        URL url = new URL(strURL);
        String protocol = url.getProtocol();
        HttpURLConnection con = (HttpURLConnection) url.openConnection();
        if (protocol.equals("https")) {
        } else if (protocol.equals("http")) {
            // do nothing
        } else {
            throw new IOException("bad protocol");
        }
        con.setRequestMethod("GET");
        con.connect();
        if (con.getResponseCode() != HttpURLConnection.HTTP_OK) {
            throw new IOException("" + con.getResponseCode());
        }
        return con.getInputStream();
    }

	public static void main(String args[]) {
        System.out.println(HttpAccess.class.getResource("/").getFile());
		try {
            String url = "https://ftp.orca.med.or.jp/pub/java-client2/version";
            InputStream is = HttpAccess.get(url);
			BufferedReader reader = new BufferedReader(new InputStreamReader(is));
			System.out.println(reader.readLine());
		} catch (Exception ex) {
			ex.printStackTrace();
		}
	}
}

