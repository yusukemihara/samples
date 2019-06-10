/* 
* $ javac GetPrivateKey.java 
* $ java GetPrivateKey storepass keypass keystore alias >key.out 


（１）コンパイル 
javac△-classpath△.△GetPrivateKey.java 

（２）実行 
C:\\>java -classpath . GetPrivateKey *** *** *** *** > key.der 

（３）der→pem 
C:\\OpenSSL\\bin>openssl pkcs8 -inform der -in key.der -outform pem -out key.pem -nocrypt 


*/ 
import java.security.KeyStore; 
import java.security.Key; 
import java.io.FileInputStream; 
public class GetPrivateKey { 
public static void main(String[] args) throws Exception { 
String spass = args[0]; 
String kpass = args[1]; 
String keystore = args[2]; 
String alias = args[3]; 
KeyStore ks = KeyStore.getInstance("JKS"); 
FileInputStream fs = new FileInputStream(keystore); 
ks.load(fs, spass.toCharArray()); 
Key key = ks.getKey(alias, kpass.toCharArray()); 
System.out.write(key.getEncoded()); 
} 
} 
