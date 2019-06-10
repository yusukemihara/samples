all: orcbarcode.so BARCODE

clean:
	rm -f *.so *.o BARCODE

orcbarcode.so:	orcbarcode.c
	gcc -g -Wl,--no-as-needed `pkg-config --cflags --libs libpng12 ` -lm -fPIC -shared -Wl,-soname,$@ -o $@ $? /usr/lib/libbarcode.a 

BARCODE:		BARCODE.CBL BARCODE.INC
	cobc -x -fixed -fimplicit-init -std=cobol85 $<
