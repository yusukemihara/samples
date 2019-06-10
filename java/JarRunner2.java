
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.InvocationTargetException;
import java.net.JarURLConnection;
import java.net.URL;
import java.net.URLClassLoader;
import java.security.cert.Certificate;
import java.security.KeyStore;
import java.security.cert.X509Certificate;
import java.util.jar.Attributes;
import javax.net.ssl.TrustManager;
import javax.net.ssl.TrustManagerFactory;
import javax.net.ssl.X509TrustManager;

public class JarRunner2 {

    public static void main(String[] args)
            throws IOException, ClassNotFoundException,
            NoSuchMethodException, InvocationTargetException {

        System.err.println("start");        

        URL url = new URL(args[0]);

        // アプリケーション jar ファイルに対するクラスローダを生成
        JarClassLoader cl = new JarClassLoader(url);

        // アプリケーションのメインクラス名を取得
        String name = cl.getMainClassName();

        // アプリケーションの引数を取得
        String[] newArgs = new String[args.length - 1];
        System.arraycopy(args, 1, newArgs, 0, newArgs.length);

        // アプリケーションのメインクラスを呼び出す
        try {
        cl.invokeClass(name, newArgs);
        } catch(Exception ex) {
ex.printStackTrace();            
        };
        System.err.println("end");
    }
}

final class JarClassLoader extends URLClassLoader {

    private URL url;

    public JarClassLoader(URL url) {
        super(new URL[]{url});
        this.url = url;
    }

    public String getMainClassName() throws IOException {
        URL u = new URL("jar", "", url + "!/");
        JarURLConnection uc = (JarURLConnection) u.openConnection();
        Attributes attr = uc.getMainAttributes();
        return attr != null
                ? attr.getValue(Attributes.Name.MAIN_CLASS) : null;
    }

    public void invokeClass(String name, String[] args)
            throws Exception {
        Class c = loadClass(name);
        Certificate[] certs =
                c.getProtectionDomain().getCodeSource().getCertificates();
        if (certs == null) {
            // 署名されてなければ実行せずにリターン
            System.out.println("No signature!");
            return;
        }
        
System.out.println(certs.length);        
        
        KeyStore ks = KeyStore.getInstance("JKS"); //$NON-NLS-1$
        File trustStorePath = getTrustStorePath();
        InputStream is = new FileInputStream(trustStorePath);
        ks.load(is, null);
        final TrustManagerFactory tmf = TrustManagerFactory.getInstance("SunX509"); //$NON-NLS-1$
        tmf.init(ks);
        final TrustManager[] tms = tmf.getTrustManagers();
        System.out.println("tms.length:" + tms.length);
        for (int i = 0; i < tms.length; i++) {
            if (tms[i] instanceof X509TrustManager) {
                X509TrustManager x509tm = (X509TrustManager)tms[i];
                X509Certificate[] x509certs = new X509Certificate[certs.length];
                for(int j = 0; j < certs.length; j++) {
                    x509certs[i] = (X509Certificate)certs[i];
                }
                
                x509tm.checkClientTrusted(x509certs, "RSA");
System.out.println("verify OK");                
            } else {
                System.out.println("other:" + tms[i]);
            }
        }        
        
        
        /*
         * KeyStore ks = KeyStore.getInstance("JKS"); ks.load(new
         * FileInputStream(System.getProperty( "user.home"+ File.separator +
         * "keystore.jks")), "loadkeystorepassword".toCharArray()); // "user"
         * は証明書を表す別名 Certificate pubCert = ks.getCertificate("user"); //
         * 信頼する公開鍵で検証する。検証に失敗したら例外をスロー certs[0].verify(pubCert.getPublicKey());
         */
    }

    public static boolean isMacOSX() {
        return System.getProperty("os.name").toLowerCase().startsWith("mac os x"); //$NON-NLS-1$ //$NON-NLS-2$
    }

    public static boolean isWindows() {
        String osName = System.getProperty("os.name").toLowerCase(); //$NON-NLS-1$
        return osName.startsWith("windows"); //$NON-NLS-1$
    }

    public static File createFilePath(String[] elements) {
        File path = new File(elements[0]);
        for (int i = 1; i < elements.length; i++) {
            path = new File(path, elements[i]);
        }
        return path;
    }

    public static File getTrustStorePath() {
        String home = System.getProperty("user.home");
        String osName = System.getProperty("os.name").toLowerCase();
        if (osName.startsWith("windows vista")
                || osName.startsWith("windows 7")
                || osName.startsWith("windows 8")
                || osName.startsWith("windows 9")
                || osName.startsWith("windows 10")) {
            return new File(
                    createFilePath(new String[]{home, "AppData", "LocalLow", "Sun", "Java", "Deployment", "security"}), "trusted.jssecacerts");
        } else if (isWindows()) {
            return new File(
                    createFilePath(new String[]{home, "Application Data", "Sun", "Java", "Deployment", "security"}), "trusted.jssecacerts");

        } else if (isMacOSX()) {
            return new File(
                    createFilePath(new String[]{home, "Library", "Keychains"}),
                    "login.keychain");
            /*
             * return new File( createFilePath(new String[]{home, "Library",
             * "keychains"}), "login.keychain");
             */
        } else {
            return new File(
                    createFilePath(new String[]{home, ".icedtea", "security",}),"trusted.jssecacerts");
/*            
            return new File(
                    createFilePath(new String[]{home, ".java", "deployment", "security"}),
                    "trusted.jssecacerts");*/
        }
    }
}
