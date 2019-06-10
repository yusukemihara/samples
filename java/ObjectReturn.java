import java.util.HashMap;

public class ObjectReturn {

	public static Object getValue(String key) {
		HashMap hash = new HashMap();
		hash.put("test1", 100);
		hash.put("test2", false);
		hash.put("test3", "test3");
		return hash.get(key);
	}

	public static void main(String args[]) {
		System.out.println(getValue("test1").getClass());
		System.out.println(getValue("test2").getClass());
		System.out.println(getValue("test3").getClass());
	}
}
