/** 自分のPC環境のデフォルトプリントサービスを表示する

  * MyPrintService.java

  * Date: February 2003

  * Author: Terry

  * Coryright: Cresc Corp

  */

import java.io.*;

import java.util.*;

import java.util.Properties;

import javax.print.*;

import javax.print.attribute.*;

import javax.print.attribute.standard.*;

import javax.print.event.*;

 

/**

 *MyPrintServiceクラス

 */

public class MyPrintService{

 

 

/**

 *メイン・メソッド

 */

public static void main(String args[]) {

 

//デフォルトのプリントサービスを探す

PrintService myDefService = PrintServiceLookup.lookupDefaultPrintService();

    System.out.println("Default Print service is " + myDefService);

//そのプリントサービスの属性を出力

PrintServiceAttributeSet myAset = myDefService.getAttributes();

Attribute[] attr = myAset.toArray();

int loop = attr.length;

System.out.println("My Attributes set:");

for (int i = 0; i < attr.length; i++) {

    System.out.println("   " + attr[i]);

}

System.out.println();

//そのプリントサービスがサポートするフレバーを出力

DocFlavor[] myFlavors = myDefService.getSupportedDocFlavors();

loop = myFlavors.length;

DocFlavor flavor;

for (int i = 0; i < myFlavors.length; i++) {

    flavor = myFlavors[i];

//  System.out.println("MediaSubtype: " + flavor.getMediaSubtype());

//  System.out.println("MediaType:    " + flavor.getMediaType());

//  System.out.println("MimeType:     " + flavor.getMimeType());

    System.out.println("toString:     " + flavor.toString());

 

}

}

}
