import java.io.*;

class FileBasename {

static public void main(String args[]) {
  File file = new File("C:/hoge/moge/goge.txt");
  System.out.println(file.getName());
  File file2 = new File("/hoge/moge/age.txt");
  System.out.println(file2.getName());
  File file3 = new File("/hoge/../moge.txt");
  System.out.println(file3.getName());
}
}
