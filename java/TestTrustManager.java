
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.security.KeyStore;
import java.util.Enumeration;

public class TestTrustManager {

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

    public static void main(String[] args) throws Exception {
        final KeyStore ks = KeyStore.getInstance("JKS"); //$NON-NLS-1$
        final File trustStorePath = getTrustStorePath();
        final InputStream is = new FileInputStream(trustStorePath);
        ks.load(is, null);
        is.close();
        for (Enumeration<String> e = ks.aliases(); e.hasMoreElements(); ) {
            String alias = e.nextElement();
            System.out.println(alias);
            System.out.println(ks.getCertificateChain(alias));
        }
/*        
        final TrustManagerFactory tmf = TrustManagerFactory.getInstance("SunX509"); //$NON-NLS-1$
        tmf.init(ks);
        final TrustManager[] tms = tmf.getTrustManagers();
        System.out.println("tms.length:" + tms.length);
        for (int i = 0; i < tms.length; i++) {
            if (tms[i] instanceof X509TrustManager) {
                X509TrustManager x509 = (X509TrustManager)tms[i];
                X509Certificate[] certs = x509.getAcceptedIssuers();
                System.out.println("certs.length:"+certs.length);
                for(int j=0;j<certs.length;j++) {
                    System.out.println(certs[i].getSubjectDN().getName());
                }
                System.out.println(x509.getAcceptedIssuers());
            } else {
                System.out.println("other:" + tms[i]);
            }
        }
        * 
        */
    }
}
