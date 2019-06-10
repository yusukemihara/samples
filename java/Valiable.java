public class Valiable {

    class TestClass {
		public int a;
		protected int b;
        private int c;

        public TestClass() {
			a = 0;
			b = 1;
			c = 2;
		}
  
		public void print() {
			System.out.println("====");
			System.out.println("a = "+a);
			System.out.println("b = "+b);
			System.out.println("c = "+c);
		}
	}

	public static void main(String args[]) {
      TestClass test = new TestClass();
      test.a = 100;
      test.b = 200;
      test.print();
	}
}
