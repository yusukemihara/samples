import java.awt.Color;
import javax.swing.plaf.ColorUIResource;


public class MyColorMaker {
  // primarios
  private ColorUIResource primary1 = new ColorUIResource( 229, 189, 0);

  private ColorUIResource primary2 = new ColorUIResource( 239, 199, 0);

  private ColorUIResource primary3 = new ColorUIResource( 249, 209, 0);

  // secondarios
  private ColorUIResource secondary1 = new ColorUIResource( 217, 215, 173);

  private ColorUIResource secondary2 = new ColorUIResource( 227, 225, 183);

  private ColorUIResource secondary3 = new ColorUIResource( 237, 235, 193);
  
  private ColorUIResource black = new ColorUIResource( 0, 0, 0);
  
  private ColorUIResource white = new ColorUIResource( 255, 255, 255);

  private int H2I(String str) {
    return Integer.parseInt(str, 16);
  }
  private String I2H(int integer) {
    return Integer.toString(integer, 16);
  }

  public MyColorMaker( String str) {
    Color prim = new Color(
      H2I(str.substring(0,2)),  
      H2I(str.substring(2,4)),  
      H2I(str.substring(4,6)) 
	  );
    Color sec = new Color(
      H2I(str.substring(6,8)),  
      H2I(str.substring(8,10)),  
      H2I(str.substring(10,12))  
	  );
    setPrimary( sec);
    setSecondary( prim);
  }

  
  public MyColorMaker( Color prim, Color sec) {
    setPrimary( prim);
    setSecondary( sec);
  }

  public void OutputColor(ColorUIResource color) {
    System.out.print(I2H(color.getRed()));
    System.out.print(I2H(color.getGreen()));
    System.out.print(I2H(color.getBlue()));
    System.out.println("");
  }

  public void Output() {
    System.out.print("nimrodlf.p1=#");OutputColor(getPrimary1());
    System.out.print("nimrodlf.p2=#");OutputColor(getPrimary2());
    System.out.print("nimrodlf.p3=#");OutputColor(getPrimary3());
    System.out.print("nimrodlf.s1=#");OutputColor(getSecondary1());
    System.out.print("nimrodlf.s2=#");OutputColor(getSecondary2());
    System.out.print("nimrodlf.s3=#");OutputColor(getSecondary3());
    System.out.println("nimrodlf.w=#FFFFFF");
    System.out.println("nimrodlf.b=#000000");
    System.out.println("mrodlf.menuOpacity=195");
    System.out.println("nimrodlf.frameOpacity=180");
  }

  static public void main(String[] args) {
    MyColorMaker maker = new MyColorMaker(args[0]);
    maker.Output();
  }

  protected ColorUIResource getPrimary1() { 
    return primary1; 
  }
  protected ColorUIResource getPrimary2() { 
    return primary2; 
  }
  protected ColorUIResource getPrimary3() { 
    return primary3; 
  }

  protected ColorUIResource getSecondary1() { 
    return secondary1; 
  }
  protected ColorUIResource getSecondary2() { 
    return secondary2; 
  }
  protected ColorUIResource getSecondary3() { 
    return secondary3; 
  }

  protected ColorUIResource getBlack() { 
    return black; 
  }
  
  protected ColorUIResource getWhite() { 
    return white; 
  }
  
  public void setPrimary( Color selection) {
    int r = selection.getRed();
    int g = selection.getGreen();
    int b = selection.getBlue();
    
    primary1 = new ColorUIResource( new Color( (r>20 ? r-20 : 0), (g>20 ? g-20 : 0), (b>20 ? b-20 : 0)));
    primary2 = new ColorUIResource( new Color( (r>10 ? r-10 : 0), (g>10 ? g-10 : 0), (b>10 ? b-10 : 0)));
    primary3 = new ColorUIResource( selection);
  }
  
  public void setSecondary( Color background) {
    int r = background.getRed();
    int g = background.getGreen();
    int b = background.getBlue();
    
    secondary1 = new ColorUIResource( new Color( (r>20 ? r-20 : 0), (g>20 ? g-20 : 0), (b>20 ? b-20 : 0)));
    secondary2 = new ColorUIResource( new Color( (r>10 ? r-10 : 0), (g>10 ? g-10 : 0), (b>10 ? b-10 : 0)));
    secondary3 = new ColorUIResource( background);
  }
  
  public void setPrimary1( Color col) { 
    primary1 = new ColorUIResource( col); 
  }
  public void setPrimary2( Color col) { 
    primary2 = new ColorUIResource( col); 
  }
  public void setPrimary3( Color col) { 
    primary3 = new ColorUIResource( col); 
  }

  public void setSecondary1( Color col) { 
    secondary1 = new ColorUIResource( col); 
  }
  public void setSecondary2( Color col) { 
    secondary2 = new ColorUIResource( col); 
  }
  public void setSecondary3( Color col) { 
    secondary3 = new ColorUIResource( col); 
  }
  
  public void setBlack( Color col) { 
    black = new ColorUIResource( col); 
  }
  public void setWhite( Color col) { 
    white = new ColorUIResource( col); 
  }
}
