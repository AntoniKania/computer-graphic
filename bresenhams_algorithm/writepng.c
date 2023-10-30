/* Copyright 2002-2008 Guillaume Cottenceau, 2015 Aleksander Denisiuk
 *
 * This software may be freely redistributed under the terms
 * of the X11 license.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define PNG_DEBUG 3
#include <png.h>
#include <ctype.h>


#define OUT_FILE "initials.png"
#define WIDTH 600
#define HEIGHT 600
#define COLOR_TYPE PNG_COLOR_TYPE_RGB
#define BIT_DEPTH 8
#define MARGIN_LEFT 50
#define MARGIN_TOP 80
#define SCALE 2


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

struct color{
    png_byte r;
    png_byte g;
    png_byte b;
};

int is_angle_greater_than_180(int i1, int i2) {
    int di = i2 - i1;

    return di < 0;
}

char* read_file_content(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* content = malloc(length + 1);
    fread(content, 1, length, file);
    content[length] = '\0';

    fclose(file);
    return content;
}

char* extract_path_data(const char* content) {
    const char* path_start = strstr(content, "<path");
    const char* d_start = strstr(path_start, "d=\"") + 3;
    const char* d_end = strstr(d_start, "\"");

    size_t length = d_end - d_start;
    char* path_data = malloc(length + 1);
    strncpy(path_data, d_start, length);
    path_data[length] = '\0';

    return path_data;
}

void write_pixel(int x, int y, png_byte cr, png_byte cg, png_byte cb) {
    png_byte* row = row_pointers[y];
    png_byte* ptr = &(row[x*3]);
    ptr[0] = cr;
    ptr[1] = cg;
    ptr[2] = cb;
}

struct color get_pixel(int x, int y) {
    png_byte* row = row_pointers[y];
    png_byte* ptr = &(row[x*3]);
    struct color c = {ptr[0], ptr[1], ptr[2]};
    return c;
}

void write_pixel8(int m1, int m2, int x, int y, png_byte cr, png_byte cg, png_byte cb) {
    write_pixel(m1 + x, m2 + y, cr, cg, cb);
    write_pixel(m1 + y, m2 + x, cr, cg, cb);
    write_pixel(m1 + x, m2 - y, cr, cg, cb);
    write_pixel(m1 + y, m2 - x, cr, cg, cb);
    write_pixel(m1 - x, m2 + y, cr, cg, cb);
    write_pixel(m1 - y, m2 + x, cr, cg, cb);
    write_pixel(m1 - x, m2 - y, cr, cg, cb);
    write_pixel(m1 - y, m2 - x, cr, cg, cb);
}

void bresenham(int i1, int j1, int i2, int j2, png_byte cr, png_byte cg, png_byte cb) {
    int m, b, P, i, j;

    if(i2 > i1 && j2 >= j1 && j2 - j1 <= i2 - i1) {
        m = 2 * (j2 - j1);
        b = 0;
        write_pixel(i1, j1, cr, cg, cb);
        j = j1;
        P = i2 - i1;
        for(i = i1 + 1; i <= i2; i++) {
            b += m;
            if(b >= 0) {
                j += 1;
                b -= 2 * P;
            }
            write_pixel(i, j, cr, cg, cb);
        }
    } else if(j2 > j1 && i2 >= i1 && i2 - i1 <= j2 - j1) {
        m = 2 * (i2 - i1);
        b = 0;
        write_pixel(i1, j1, cr, cg, cb);
        i = i1;
        P = j2 - j1;
        for(j = j1 + 1; j <= j2; j++) {
            b += m;
            if(b > P) {
                i += 1;
                b -= 2 * P;
            }
            write_pixel(i, j, cr, cg, cb);
        }
    } else if (i2 > i1 && -j2 >= -j1 && j1 - j2 <= i2 - i1) {
        m = 2 * (j1 - j2);
        b = 0;
        write_pixel(i1, j2, cr, cg, cb);
        j = j2;
        P = i2 - i1;
        for(i = i1 + 1; i <= i2; i++) {
            b += m;
            if(b > P) {
                j--;
                b -= 2 * P;
            }
            write_pixel(i, j, cr, cg, cb);
        }
    } else if(j2 < j1 && i2 >= i1 && i2 - i1 <= j1 - j2) {
        m = 2 * (i2 - i1);
        b = 0;
        write_pixel(i1, j1, cr, cg, cb);
        i = i2;
        P = j1 - j2;
        for(j = j2 + 1; j <= j1; j++) {
            b += m;
            if (b > P) {
                i--;
                b -= 2 * P;
            }
            write_pixel(i, j, cr, cg, cb);
        }
    }
}

void process_line_segment(int i1, int j1, int i2, int j2) {
    if (is_angle_greater_than_180(i1, i2)) {
        bresenham(i2, j2, i1, j1, 255, 0, 0);
    } else {
        bresenham(i1, j1, i2, j2, 255, 0, 0);
    }
}

char** split_by_commands(const char* input, int* count) {
    char** result = malloc(100 * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    const char* start = input;
    int index = 0;

    while (*input) {
        if (isalpha(*input)) {
            if (input != start) {
                int len = input - start;
                result[index] = malloc((len + 1) * sizeof(char));
                if (result[index] == NULL) {
                    return NULL; // memory allocation failed
                }
                strncpy(result[index], start, len);
                result[index][len] = '\0';
                index++;
            }
            start = input;
        }
        input++;
    }

    result[index] = strdup(start);
    index++;

    *count = index;
    return result;
}

void process_file(const char* filename) {
    char *content = read_file_content(filename);
    if (!content) {
        return;
    }

    char *path_data = extract_path_data(content);
    free(content);

    int count;
    char **tokens = split_by_commands(path_data, &count);
    int start_x, start_y, prev_x, prev_y;

    for (int i = 0; i < count; i++) {
        char *token = tokens[i];
        char cmd = token[0];
        int x, y;

        switch (cmd) {
            case 'M':
                sscanf(token + 1, "%d %d", &x, &y);
                x = (x + MARGIN_LEFT) * SCALE;
                y = (y + MARGIN_TOP) * SCALE;

                start_x = x;
                start_y = y;

                prev_x = x;
                prev_y = y;
                break;

            case 'L':
                sscanf(token + 1, "%d %d", &x, &y);
                x = (x + MARGIN_LEFT) * SCALE;
                y = (y + MARGIN_TOP) * SCALE;

                process_line_segment(prev_x, prev_y, x, y);

                prev_x = x;
                prev_y = y;
                break;

            case 'Z':
                process_line_segment(prev_x, prev_y, start_x, start_y);
                break;
        }
    }
}

void circle(int R, png_byte cr, png_byte cg, png_byte cb) {
    int i, j, f;
    i = 0;
    j = R;
    f = 5 - 4 * R;
    write_pixel8(WIDTH/2, HEIGHT/2, i, R, cr, cg, cb);

    while (i < j) {
        if (f > 0) {
            f = f + 8 * i - 8 * j + 20;
            j = j - 1;
        } else {
            f = f + 8 * i + 12;
        }
        i = i + 1;
        write_pixel8(WIDTH/2, HEIGHT/2, i, j, cr, cg, cb);
    }
}

void flood_fill(int x, int y, struct color prev_color, png_byte cr, png_byte cg, png_byte cb) {
    if (x <= 0 || x >= WIDTH || y <= 0 || y >= HEIGHT)
        return;

    struct color current_color = get_pixel(x, y);

    if (current_color.r != prev_color.r || current_color.g != prev_color.g || current_color.b != prev_color.b)
        return;


    write_pixel(x, y, cr, cg, cb);

    flood_fill(x + 1, y, prev_color, cr, cg, cb);
    flood_fill(x - 1, y, prev_color, cr, cg, cb);
    flood_fill(x, y + 1, prev_color, cr, cg, cb);
    flood_fill(x, y - 1, prev_color, cr, cg, cb);
}

int main(int argc, char **argv)
{
	create_png_file();
    circle(250, 155, 0, 0);
	process_file("initials.svg");
	write_png_file(OUT_FILE);

    struct color old_color = get_pixel(200, 200);
    flood_fill(300, 300, old_color, 255, 255, 255);

    return 0;
}
