import java.io.*;
import java.util.Iterator;
import org.json.*;

class TestJSON {

public void p(Object obj) throws JSONException {
  //System.out.println(obj.getClass().getName());
  if (obj instanceof JSONObject) {
    JSONObject j = (JSONObject)obj; 
    for(Iterator i = j.keys();i.hasNext();) {
      String key = (String)i.next();
      System.out.print(key+":");
      p(j.get(key));
    }
  } else if (obj instanceof JSONArray) {
    JSONArray a = (JSONArray)obj;
    for(int i=0;i<a.length();i++) {
      System.out.print(i+":");
      p(a.get(i));
    }
  } else {
    System.out.println(obj);
  }
}

static public void main(String args[]) throws JSONException {
TestJSON t = new TestJSON();
JSONObject obj = new JSONObject("{\"a\":0,\"b\":1,\"c\":2,\"d\":5}}");
System.out.println(obj);
t.p((Object)obj);
System.out.println("----");
obj.put("a",3);
obj.put("c",5);
obj.put("b",4);
System.out.println(obj);
t.p((Object)obj);
}
}
