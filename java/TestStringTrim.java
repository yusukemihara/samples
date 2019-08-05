class TestStringTrim {
  static public void main(String args[]){
    String str = "\n\r moge   \n\n";
    System.out.println("---- before");
    System.out.println("|"+str+"|");
    System.out.println("---- after");
    System.out.println("|"+str.trim()+"|");
    System.out.println("----");
  }
}
