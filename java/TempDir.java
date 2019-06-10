import java.io.*;
import java.util.*;

class TempDir {
  private File tempDirRoot;
  private File tempDir;

  public TempDir() {
    tempDirRoot = new File(new File(new File(System.getProperty("user.home")),".monsiaj"),"tmp");
    tempDir = new File(tempDirRoot,UUID.randomUUID().toString());
    tempDir.mkdirs();
System.out.println(tempDir);
  }

  private void deleteAll(File f) throws SecurityException{
    if (f.isDirectory()) {
      for(File g: f.listFiles()) {
        deleteAll(g);
      }
    }
    f.delete();
  }

  public void cleanOld() {
    for(File f: tempDirRoot.listFiles()) {
      System.out.println("file:"+ f + " lastmodified:" + f.lastModified());
      try {
        long elaps = System.currentTimeMillis() - f.lastModified();
        if (elaps > 86400000) { /* 1day */
          deleteAll(f);
System.out.println("delete");
        } else {
System.out.println("no delete");
        }
      } catch (SecurityException e) {
      } 
    }
  }

  static public void main(String args[]) {
    TempDir dir = new TempDir();
    dir.cleanOld();
  }
}
