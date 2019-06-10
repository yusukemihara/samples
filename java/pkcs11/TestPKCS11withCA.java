
import java.io.BufferedInputStream;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.URL;
import java.security.GeneralSecurityException;
import java.security.KeyStore;
import java.security.Provider;
import java.security.Security;
import java.security.cert.CertificateException;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;
import java.util.Properties;
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
import javax.xml.bind.DatatypeConverter;

public class TestPKCS11withCA {

    private final SSLSocketFactory factory;

    public TestPKCS11withCA(String caCertPath) throws IOException, GeneralSecurityException {
        final SSLContext ctx;
        ctx = SSLContext.getInstance("TLS");
        ctx.init(createKeyManagers(), createCAFileTrustManagers(caCertPath), null);
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

    private KeyManager[] createKeyManagers() throws IOException, GeneralSecurityException {
        String propPath = "pkcs11.cfg";
        /*
         Properties tmp = new Properties();
         try {
         tmp.setProperty("name", "test");
         tmp.setProperty("library", library);
         tmp.setProperty("slotListIndex", "0");

         tmp.store(new FileOutputStream(propPath), "monsiaj setting");
         } catch (IOException ex) {
         System.out.println(ex);
         System.exit(0);
         }*/

        Provider p = new sun.security.pkcs11.SunPKCS11(propPath);
        Security.removeProvider("IAIK");
        Security.addProvider(p);
        KeyStore.Builder builder = KeyStore.Builder.newInstance("PKCS11", p, new KeyStore.CallbackHandlerProtection(new MyCallbackHandler()));
        KeyManagerFactory kmf = KeyManagerFactory.getInstance("NewSunX509");
        kmf.init(new KeyStoreBuilderParameters(builder));
        return kmf.getKeyManagers();
    }

    protected static byte[] parseDERFromPEM(byte[] pem, String beginDelimiter, String endDelimiter) {
        String data = new String(pem);
        if (data.contains(beginDelimiter) && data.contains(endDelimiter)) {
            String[] tokens = data.split(beginDelimiter);
            tokens = tokens[1].split(endDelimiter);
            return DatatypeConverter.parseBase64Binary(tokens[0]);
        } else {
            return pem;
        }
    }

    protected static X509Certificate generateCertificateFromDER(byte[] certBytes) throws CertificateException {
        CertificateFactory factory = CertificateFactory.getInstance("X.509");

        return (X509Certificate) factory.generateCertificate(new ByteArrayInputStream(certBytes));
    }

    private byte[] fileToBytes(File file) throws IOException {
        byte[] bytes = new byte[(int) file.length()];
        try (FileInputStream inputStream = new FileInputStream(file)) {
            inputStream.read(bytes);
        }
        return bytes;
    }

    private TrustManager[] createCAFileTrustManagers(String caCertPath) throws GeneralSecurityException, FileNotFoundException, IOException {
        byte[] certPem = fileToBytes(new File(caCertPath));
        byte[] certBytes = parseDERFromPEM(certPem, "-----BEGIN CERTIFICATE-----", "-----END CERTIFICATE-----");
        X509Certificate cert = generateCertificateFromDER(certBytes);

        KeyStore keystore = KeyStore.getInstance("JKS");
        keystore.load(null);
        keystore.setCertificateEntry("cert-alias", cert);
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
        TestPKCS11withCA p11 = new TestPKCS11withCA(args[0]);
        for (int i = 0; i < 100; i++) {
            p11.sendHttpsRequest(args[1]);
            System.out.println(i);
        }
    }

}
