/* Copyright 2002-2008 Guillaume Cottenceau, 2015 Aleksander Denisiuk
 *
 * This software may be freely redistributed under the terms
 * of the X11 license.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#define PNG_DEBUG 3
#include <png.h>


#define OUT_FILE "initials.png"
#define WIDTH 600
#define HEIGHT 600
#define COLOR_TYPE PNG_COLOR_TYPE_RGB
#define BIT_DEPTH 8


void abort_(const char * s, ...)
{
	va_list args;
	va_start(args, s);
	vfprintf(stderr, s, args);
	fprintf(stderr, "\n");
	va_end(args);
	abort();
}

int x, y;

int width, height;
png_byte color_type;
png_byte bit_depth;

png_structp png_ptr;
png_infop info_ptr;
int number_of_passes;
png_bytep * row_pointers;

void create_png_file()
{
	width = WIDTH;
	height = HEIGHT;
        bit_depth = BIT_DEPTH;
        color_type = COLOR_TYPE;

	row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
	for (y=0; y<height; y++)
		row_pointers[y] = (png_byte*) malloc(width*bit_depth*3);


}


void write_png_file(char* file_name)
{
	/* create file */
	FILE *fp = fopen(file_name, "wb");
	if (!fp)
		abort_("[write_png_file] File %s could not be opened for writing", file_name);


	/* initialize stuff */
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	
	if (!png_ptr)
		abort_("[write_png_file] png_create_write_struct failed");

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
		abort_("[write_png_file] png_create_info_struct failed");

	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[write_png_file] Error during init_io");

	png_init_io(png_ptr, fp);


	/* write header */
	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[write_png_file] Error during writing header");

	png_set_IHDR(png_ptr, info_ptr, width, height,
		     bit_depth, color_type, PNG_INTERLACE_NONE,
		     PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_write_info(png_ptr, info_ptr);


	/* write bytes */
	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[write_png_file] Error during writing bytes");

	png_write_image(png_ptr, row_pointers);


	/* end write */
	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[write_png_file] Error during end of write");

	png_write_end(png_ptr, NULL);

        /* cleanup heap allocation */
	for (y=0; y<height; y++)
		free(row_pointers[y]);
	free(row_pointers);

        fclose(fp);
}

void process_file(void)
{
	for (y=0; y<height; y++) {
		png_byte* row = row_pointers[y];
		for (x=0; x<width; x++) {
			png_byte* ptr = &(row[x*3]);
			ptr[0] = 0;
			ptr[1] = ptr[2] = 255;
		}
	}

}

void write_pixel(int x, int y, png_byte cr, png_byte cg, png_byte cb) {
    png_byte* row = row_pointers[y];
    png_byte* ptr = &(row[x*3]);
    ptr[0] = cr;
    ptr[1] = cg;
    ptr[2] = cb;
}

void write_pixel8(int x, int y, png_byte cr, png_byte cg, png_byte cb) {
    write_pixel(x, y, cr, cg, cb);
    write_pixel(y, x, cr, cg, cb);
    write_pixel(x, -y, cr, cg, cb);
    write_pixel(y, -x, cr, cg, cb);
    write_pixel(-x, y, cr, cg, cb);
    write_pixel(-y, x, cr, cg, cb);
    write_pixel(-x, -y, cr, cg, cb);
    write_pixel(-y, -x, cr, cg, cb);
}

void circle(int R, png_byte cr, png_byte cg, png_byte cb) {
    int i, j, f;
    i = 0;
    j = R;
    f = 5 - 4 * R;
    write_pixel8(i, j, cr, cg, cb);
    
    while (i < j) {
        if (f >= 0) {
            f = f + 8 * i - 8 * j + 20;
            j = j - 1;
        } else {
            f = f + 8 * i + 12;
            i = i + 1;
        }
        write_pixel8(i, j, cr, cg, cb);
    }
}

void bresenham(int i1, int j1, int i2, int j2, png_byte cr, png_byte cg, png_byte cb) {
    int m, b, P, i, j;

    if(i2 > i1 && j2 >= j1 && j2 - j1 <= i2 - i1) {
        printf("przypadek 1\n");
        m = 2 * (j2 - j1);
        b = m - (i2 - i1);
        write_pixel(i1, j1, cr, cg, cb);
        j = j1;
        for(i = i1 + 1; i <= i2; i++) {
            if(b >= 0) {
                j += 1;
                b -= 2 * (i2 - i1);
            }
            b += m;
            write_pixel(i, j, cr, cg, cb);
        }
    } else if(j2 > j1 && i2 >= i1 && i2 - i1 <= j2 - j1) {
        printf("przypadek 2\n");
        m = 2 * (i2 - i1);
        b = m - (j2 - j1);
        write_pixel(i1, j1, cr, cg, cb);
        i = i1;
        for(j = j1 + 1; j <= j2; j++) {
            if(b >= 0) {
                i += 1;
                b -= 2 * (j2 - j1);
            }
            b += m;
            write_pixel(i, j, cr, cg, cb);
        }
    } else if(i1 > i2 && j2 > j1 && j2 - j1 >= i1 - i2) {
        printf("przypadek 3\n");
        m = 2 * (i1 - i2);
        b = m - (j2 - j1);
        write_pixel(i1, j1, cr, cg, cb);
        i = i1;
        for(j = j1 + 1; j <= j2; j++) {
            if(b >= 0) {
                i -= 1;
                b -= 2 * (j2 - j1);
            }
            b += m;
            write_pixel(i, j, cr, cg, cb);
        }
    } else if(i1 > i2 && j2 >= j1 && j2 - j1 <= i1 - i2) {
        printf("przypadek 4\n");
        m = 2 * (j2 - j1);
        b = m - (i1 - i2);
        write_pixel(i1, j1, cr, cg, cb);
        j = j1;
        for(i = i1 - 1; i >= i2; i--) {
            if(b >= 0) {
                j += 1;
                b -= 2 * (i1 - i2);
            }
            b += m;
            write_pixel(i, j, cr, cg, cb);
        }
    }
}

int main(int argc, char **argv)
{
	create_png_file();
	process_file();
	write_png_file(OUT_FILE);

        return 0;
}
