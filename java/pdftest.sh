#!/bin/bash -x
#lib=/home/mihara/NetBeansProjects/monsiaj/bin/jmareceipt.jar:/home/mihara/NetBeansProjects/monsiaj/bin/log4j-api-2.0.1.jar:/home/mihara/NetBeansProjects/monsiaj/bin/log4j-core-2.0.1.jar:/home/mihara/NetBeansProjects/pdf-renderer-head/trunk/dist/PDFRenderer.jar
lib=/home/mihara/NetBeansProjects/pdf-renderer-head/trunk/dist/PDFRenderer.jar:/home/mihara/NetBeansProjects/monsiaj/bin/jmareceipt.jar:/home/mihara/NetBeansProjects/monsiaj/bin/log4j-api-2.0.1.jar:/home/mihara/NetBeansProjects/monsiaj/bin/log4j-core-2.0.1.jar

java -cp $lib org.montsuqi.widgets.PandaPreview $1
