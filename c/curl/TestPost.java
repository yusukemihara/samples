/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.net.HttpURLConnection;
import java.net.URL;

/**
 *
 * @author mihara
 */
public class TestPost {

    static public void main(String argv[]) {
        try {
        URL url = new URL(argv[0]);
        HttpURLConnection con = (HttpURLConnection) url.openConnection();
        con.setInstanceFollowRedirects(false);
        con.setRequestMethod("POST");
        con.setDoOutput(true);
        con.connect();
        OutputStreamWriter osw = new OutputStreamWriter(con.getOutputStream());
        osw.write("{ \"moge\":\"goge\",   \n \"aaaaaaa   aaaaaaaaaaaaaaa\" : \"mu\"}");
        osw.flush();
        osw.close();

        BufferedInputStream bis = new BufferedInputStream(con.getInputStream());
        int data;
        while ((data = bis.read()) != -1) {
            System.out.write(data);
        }
        con.disconnect();
        } catch (Exception ex) {
        }
    }

}
