
import java.security.KeyStore;
import java.security.cert.X509Certificate;
import javax.net.ssl.TrustManager;
import javax.net.ssl.TrustManagerFactory;
import javax.net.ssl.X509TrustManager;

public class TestTrustManager2 {

    public static void main(String[] args) throws Exception {
        TrustManagerFactory trustManagerFactory = TrustManagerFactory.getInstance(TrustManagerFactory.getDefaultAlgorithm());
        trustManagerFactory.init((KeyStore) null);

        System.out.println("JVM Default Trust Managers:");
        for (TrustManager trustManager : trustManagerFactory.getTrustManagers()) {
            System.out.println(trustManager);

            if (trustManager instanceof X509TrustManager) {
                X509TrustManager x509TrustManager = (X509TrustManager) trustManager;
                X509Certificate[] certs = x509TrustManager.getAcceptedIssuers();
                System.out.println("certs.length:" + certs.length);
                for (int j = 0; j < certs.length; j++) {
                    System.out.println(certs[j].getSubjectDN().getName());
                }
                System.out.println("\tAccepted issuers count : " + x509TrustManager.getAcceptedIssuers().length);
            }
        }
        /*
         * final TrustManagerFactory tmf =
         * TrustManagerFactory.getInstance("SunX509"); //$NON-NLS-1$
         * tmf.init(ks); final TrustManager[] tms = tmf.getTrustManagers();
         * System.out.println("tms.length:" + tms.length); for (int i = 0; i <
         * tms.length; i++) { if (tms[i] instanceof X509TrustManager) {
         * X509TrustManager x509 = (X509TrustManager)tms[i]; X509Certificate[]
         * certs = x509.getAcceptedIssuers();
         * System.out.println("certs.length:"+certs.length); for(int
         * j=0;j<certs.length;j++) {
         * System.out.println(certs[i].getSubjectDN().getName()); }
         * System.out.println(x509.getAcceptedIssuers()); } else {
         * System.out.println("other:" + tms[i]); } }
         *
         */
    }
}
