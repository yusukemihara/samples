/*
 *  perform HTTPS GET on SSL client verification using Java PKCS#11
 * 
 *  requirement:
 *   * PKCS#11 enable security token
 *   * the token has stored cert and key for ssl client verification,and ca cert
 * PKCS#11 library(.dll or .so) for the token
 * 
 *  usage:
 *   java -cp . -D java.security.debug=sunpkcs11 TestPKCS11 https://www.example.com/
 */

import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.URL;
import java.security.GeneralSecurityException;
import java.security.KeyStore;
import java.security.Provider;
import java.security.Security;
import javax.net.ssl.HttpsURLConnection;
import javax.net.ssl.KeyManager;
import javax.net.ssl.KeyManagerFactory;
import javax.net.ssl.KeyStoreBuilderParameters;
import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLSocketFactory;
import javax.net.ssl.TrustManager;
import javax.net.ssl.TrustManagerFactory;
import javax.security.auth.callback.Callback;
import javax.security.auth.callback.CallbackHandler;
import javax.security.auth.callback.PasswordCallback;
import javax.security.auth.callback.UnsupportedCallbackException;
import javax.swing.JOptionPane;
import javax.swing.JPasswordField;

public class TestPKCS11 {

    private final SSLSocketFactory factory;

    public TestPKCS11() throws IOException, GeneralSecurityException {
        KeyStore.Builder builder = createPKCS11KeyStoreBuilder();
        final SSLContext ctx;
        ctx = SSLContext.getInstance("TLS");
        ctx.init(createPKCS11KeyManagers(builder), createPKCS11TrustManagers(builder), null);
        factory = ctx.getSocketFactory();
        SSLContext.setDefault(ctx);
    }

    public void sendHttpsRequest(String urlString) throws IOException {
        HttpURLConnection con;
        try (ByteArrayOutputStream out = new ByteArrayOutputStream()) {
            URL url = new URL(urlString);
            con = (HttpURLConnection) url.openConnection();
            String protocol = url.getProtocol();
            if (protocol.equals("https")) {
                ((HttpsURLConnection) con).setSSLSocketFactory(factory);
            } else if (protocol.equals("http")) {
                // do nothing 
            } else {
                throw new IOException("bad protocol");
            }
            con.setInstanceFollowRedirects(false);
            con.setRequestMethod("GET");
            BufferedInputStream bis = new BufferedInputStream(con.getInputStream());
            int length;
            while ((length = bis.read()) != -1) {
                out.write(length);
            }
            out.close();
            System.out.println(out);
        }
        con.disconnect();
        System.out.println(con.getResponseCode());
    }

    private KeyStore.Builder createPKCS11KeyStoreBuilder() throws IOException, GeneralSecurityException {
        String propPath = "pkcs11.cfg";
        /*
         *   ---- pkcs11.cfg
         *   name=test
         *   library=C:\FULLPATH\yourpkcs11.dll
         *   slot=1
         *   ----
         *
         *   see docs.oracle.com/javase/7/docs/technotes/guides/security/p11guide.html
         */

        Provider p = new sun.security.pkcs11.SunPKCS11(propPath);
        Security.removeProvider("IAIK");
        Security.addProvider(p);
        KeyStore.Builder builder = KeyStore.Builder.newInstance("PKCS11", p, new KeyStore.CallbackHandlerProtection(new MyCallbackHandler()));
        return builder;
    }

    private KeyManager[] createPKCS11KeyManagers(KeyStore.Builder builder) throws IOException, GeneralSecurityException {
        KeyManagerFactory kmf = KeyManagerFactory.getInstance("NewSunX509");
        kmf.init(new KeyStoreBuilderParameters(builder));
        return kmf.getKeyManagers();
    }

    private TrustManager[] createPKCS11TrustManagers(KeyStore.Builder builder) throws GeneralSecurityException, FileNotFoundException, IOException {
        KeyStore keystore = builder.getKeyStore();
        TrustManagerFactory trustManagerFactory = TrustManagerFactory.getInstance(TrustManagerFactory.getDefaultAlgorithm());
        trustManagerFactory.init(keystore);
        return trustManagerFactory.getTrustManagers();
    }

    private class MyCallbackHandler implements CallbackHandler {

        @Override
        public void handle(Callback[] callbacks) throws IOException, UnsupportedCallbackException {
            for (Callback cb : callbacks) {
                if (cb instanceof PasswordCallback) {
                    PasswordCallback pcb = (PasswordCallback) cb;
                    JPasswordField pf = new JPasswordField();
                    Object[] message = {"pin", pf};
                    int resp = JOptionPane.showConfirmDialog(null, message, "input pin", JOptionPane.OK_CANCEL_OPTION, JOptionPane.QUESTION_MESSAGE);
                    if (resp == JOptionPane.OK_OPTION) {
                        pcb.setPassword(pf.getPassword());
                    }
                } else {
                    throw new UnsupportedCallbackException(callbacks[0]);
                }
            }
        }
    }

    public static void main(String[] args) throws Exception {
        TestPKCS11 p11 = new TestPKCS11();
        p11.sendHttpsRequest(args[0]);
    }

}
