import java.net.URL;

public class TestURL {
  public static void main(String[] args) throws Exception {
     URL url = new URL("https://localhost:3000/moge");
     System.out.println(url);
     System.out.println(url.getProtocol());
  }
}
