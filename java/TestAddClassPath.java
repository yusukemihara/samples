import java.io.File;
import java.io.FileWriter;
import java.lang.reflect.Method;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.ResourceBundle;

public class TestAddClassPath 
{
	/**
	 * メイン処理。<br />
	 * @param args
	 * @throws Exception 
	 */
	public static void main(String[] args) throws Exception
	{
		// ClassPathに追加するディレクトリ
		String newClassPath = "/tmp";
		
		// 確認用プロパティファイルの作成
		FileWriter writer = new FileWriter(newClassPath + File.separator + "TestApp.properties");
		writer.write("VAR=1234567890");
		writer.flush();
		writer.close();
		
		// まずClassPathが通っていない状態でプロパティファイルを読み込んでみる
		printPropVal("TestApp", "VAR");
		
		// ClassPathの追加
		addClassPath(newClassPath);
		
		// 再度プロパティファイルを読み込み
		printPropVal("TestApp", "VAR");
	}
	
	/**
	 * ClassPathの追加。<br />
	 * @param path 追加するPATH
	 */
	private static void addClassPath(String path){
		URLClassLoader loader = (URLClassLoader)ClassLoader.getSystemClassLoader();
		try {
			URL u = new File(path).toURI().toURL();
			Method m = URLClassLoader.class.getDeclaredMethod("addURL", new Class[]{ URL.class });
			m.setAccessible(true);
			m.invoke(loader, new Object[]{u});
			System.out.println("ClassPathに「" + path + "」を追加しました");
		} catch (Exception e){
			throw new RuntimeException("ClassPathの追加に失敗しました。(" + path + ")", e);
		}
	}
	
	/**
	 * プロパティファイルから値を読み込んで表示する。<br />
	 * @param propFile プロパティファイル名
	 * @param key キー名
	 */
	private static void printPropVal(String propFile, String key){
	
		try {
			// キャッシュをクリア
			ResourceBundle.clearCache();
	
			// プロパティファイルを読み込み
			ResourceBundle bundle1 = ResourceBundle.getBundle(propFile);
			String var = bundle1.getString(key);
			System.out.println("読込み成功(" + key + "=" + var + ")");
		} catch (Exception e){
			System.out.println("読込み失敗(" + e.getMessage() + ")");
		}
	}
}
