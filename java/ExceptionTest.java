import java.io.*;
import java.util.*;

class ExceptionTest {

  public ExceptionTest(String hoge) {
    try {
      throw new IllegalArgumentException("moge");
    } catch (Exception ex) {
      System.out.println(ex);
    }
  }

  static public void main(String args[]) {
	ExceptionTest test = new ExceptionTest("moge");
  }
}
