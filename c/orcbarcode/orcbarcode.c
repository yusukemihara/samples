#if 0
#!/bin/bash -x
src=$0
out=orcbarcode
obj=orcbarcode.o
rm -f $obj $out
gcc -g -Wl,--no-as-needed `pkg-config --cflags --libs libpng12 ` -lm -c $src
gcc -g -Wl,--no-as-needed `pkg-config --cflags --libs libpng12 ` -lm -o $out $obj /usr/lib/libbarcode.a
exit
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <barcode.h>
#include <png.h>

#if 1
#define DEBUG 1
#else
#endif

static int
print_barcode(
	struct Barcode_Item *bc,
	const char *fname,
	int margin,
	int size,
	int height)
{
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

	flags = BARCODE_39 | BARCODE_OUT_PS | BARCODE_OUT_NOHEADERS | BARCODE_NO_CHECKSUM;
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
	return 0;
}

/*
barcode {
	data 	varchar(16);
	fname 	varchar(256);
	margin	varchar(2);
	size	varchar(2);
	height	varchar(3);
};
*/

enum {
	DATA=0,
	FNAME,
	MARGIN,
	SIZE,
	HEIGHT,
	RC,
	LAST
}; 

#define	SIZE_DATA	16
#define	SIZE_FNAME	256
#define	SIZE_MARGIN	2
#define	SIZE_SIZE	2
#define	SIZE_HEIGHT	3
#define SIZE_RC		1
#define	SIZE_LAST	0

static int st_size[] = {
	SIZE_DATA,
	SIZE_FNAME,
	SIZE_MARGIN,
	SIZE_SIZE,
	SIZE_HEIGHT,
	SIZE_RC,
	SIZE_LAST
};

static	char*
offset(
	char *arg,
	int elem)
{
	int i,o;

	for(i=0,o=0;i<elem;i++) {
		o += st_size[i];
	}
	return (arg+o);
}

extern void
cstrize(
	char *s,
	char *d,
	int _size)
{
	int i;

	memset(d,0,(size_t)_size+1);
	memcpy(d,s,(size_t)_size);
	for(i=0;i<_size;i++) {
		if (*(d+i)==' ') {
			*(d+i) = 0;
			break;
		}
	}
}

extern void
orcbarcode(char *arg)
{
	char _data[SIZE_DATA+1];
	char _fname[SIZE_FNAME+1];
	char _margin[SIZE_MARGIN+1];
	char _size[SIZE_SIZE+1];
	char _height[SIZE_HEIGHT+1];
	char *p;
	int m,s,h;

	cstrize(offset(arg,DATA),_data,SIZE_DATA);
	cstrize(offset(arg,FNAME),_fname,SIZE_FNAME);
	cstrize(offset(arg,MARGIN),_margin,SIZE_MARGIN);
	cstrize(offset(arg,SIZE),_size,SIZE_SIZE);
	cstrize(offset(arg,HEIGHT),_height,SIZE_HEIGHT);

#ifdef DEBUG
fprintf(stderr,"data[%s]\n",_data);
fprintf(stderr,"fname[%s]\n",_fname);
fprintf(stderr,"margin[%s]\n",_margin);
fprintf(stderr,"size[%s]\n",_size);
fprintf(stderr,"height[%s]\n",_height);
#endif

	m = atoi(_margin);
	s = atoi(_size);
	h = atoi(_height);

	p = offset(arg,RC);
	if (!make_barcode(_data,_fname,m,s,h)) {
		*p = '0';
	} else {
		*p = '1';
	}
#ifdef DEBUG
fprintf(stderr,"result:%c 0:ok 1:ng\n",*p);
#endif
}
