#if 0
#!/bin/bash -x
src=$0
out=${src%.*}
obj=$out.o
gcc -g -Wl,--no-as-needed `pkg-config --cflags --libs libpng12 ` -lm -c $src
gcc -g -Wl,--no-as-needed `pkg-config --cflags --libs libpng12 ` -lm -o $out $obj /usr/lib/libbarcode.a
$out
rm $obj $out
exit
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <barcode.h>
#include <png.h>

static int
print_barcode(
	struct Barcode_Item *bc,
	const char *fname,
	int margin,
	int size,
	int height)
{
	fprintf(stderr,"bc->partial[%s]\n",bc->partial);
	FILE *fp;
	png_structp png_ptr;
	png_infop info_ptr;
	unsigned char *row,*p,*q;
	int barwidth, realwidth;
	int y,bit,i,j,k;

	if(fname[0] == '-' && fname[1] == '\0') {
		fp = stdout;
	} else {
		fp = fopen(fname, "w");
		if(fp == NULL) {
			return -1;
		}
	}

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(png_ptr == NULL) {
		fclose(fp);
		return -1;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if(info_ptr == NULL) {
		fclose(fp);
		return -1;
	}

	if(setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(fp);
		return -1;
	}

	barwidth = 0;
	for(p=bc->partial+1;*p;p++) {
		if(isdigit(*p)){ 
			barwidth += *p - '0';
		} else {
			barwidth += *p - 'a' + 1;
		}
	}
	realwidth = barwidth * size + margin * 2;
	row = (unsigned char*)malloc((realwidth+7)/8);

	png_init_io(png_ptr, fp);
	png_set_IHDR(png_ptr, info_ptr,
			realwidth, height+margin*2,
			1,
			PNG_COLOR_TYPE_GRAY,
			PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_DEFAULT,
			PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png_ptr, info_ptr);

	/* top margin */
	memset(row, 0xff, (realwidth + 7) / 8);
	for(y=0; y<margin; y++) {
		png_write_row(png_ptr, row);
	}

	/* data */
	memset(row,0xff,(realwidth+7)/8);
	q = row + margin/ 8;
	bit = 7 - (margin%8);
	for(p=bc->partial+1,i=0;*p;p++,i++) {
		if(isdigit(*p)){ 
			j = *p - '0';
		} else {
			j = *p - 'a' + 1;
		}
		for(k=0;k<j*size;k++) {
			if (i%2==0) {
				*q ^= 1 << bit;
			} else {
				*q ^= 0 << bit;
			}
			bit--;
			if (bit<0) {
				q++;
				bit = 7;
			}
		}
	}
	for(y=0; y<height;y++) {
		png_write_row(png_ptr,row);
	}

	/* bottom margin */
	memset(row, 0xff, (realwidth + 7) / 8);
	for(y=0; y<margin; y++) {
		png_write_row(png_ptr, row);
	}

	png_write_end(png_ptr, info_ptr);
	png_destroy_write_struct(&png_ptr, &info_ptr);

	fclose(fp);
	free(row);
	return 0;
}

static int
make_barcode(
	char *text,
	char *fname,
	int margin,
	int size,
	int height)
{
	struct Barcode_Item *bc;
	int flags = 0,ret = -1;

	flags = BARCODE_128C | BARCODE_OUT_PS | BARCODE_OUT_NOHEADERS;
	bc = Barcode_Create(text);
	if (!bc) {
		fprintf(stderr,"Barcode_Create():%s\n",strerror(errno));
		return -1;
	}
	bc->margin = 0;
	if ((Barcode_Position(bc,1,1,0,0,0)<0)||
		(Barcode_Encode(bc,flags)<0)) {
		fprintf(stderr,"Barcode_Encode():%s\n",strerror(bc->error));
	} else {
		ret = print_barcode(bc,fname,margin,size,height);
	}
	Barcode_Delete(bc);
	return ret;
}

int
main(int argc,char **argv)
{
	make_barcode("123456","/tmp/out.png",36,1,35);
}
