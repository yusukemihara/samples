public class TestExtends {

	public void print() {
		System.out.println("nugugu");
	}

	public class TestExtends2 extends TestExtends {
		public void print() {
			System.out.println("gununu");
		}
	}

	public void print2() {
		TestExtends t2 = new TestExtends2();
		t2.print();
	}

	static public void main(String args[]) {
		TestExtends t1 = new TestExtends();
		t1.print();
		t1.print2();
	}
}
