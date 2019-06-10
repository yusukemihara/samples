import java.util.HashMap;

public class ClassJudge {
	public static void main(String args[]) {
		HashMap hash = new HashMap();
		hash.put("test1", 100);
		hash.put("test2", false);
		hash.put("test3", "test3");
		System.out.println(hash.get("test1").getClass());
		System.out.println(hash.get("test2").getClass());
		System.out.println(hash.get("test3").getClass());
	}
}
