/*
 * A simple libpng example program
 * http://zarb.org/~gc/html/libpng.html
 *
 * Modified by Yoshimasa Niwa to make it much simpler
 * and support all defined color_type.
 *
 * To build, use the next instruction on OS X.
 * $ brew install libpng
 * $ clang -lz -lpng15 libpng_test.c
 *
 * Copyright 2002-2010 Guillaume Cottenceau.
 *
 * This software may be freely redistributed under the terms
 * of the X11 license.
 *
 */

#include "Image.h"
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <algorithm>

using std::min;
using std::max;

char toByte(float a) {
    a = max(min(sqrt(a), 1.0), 0.0);
    return (char) rint(a * 255);
}

float toFloat(int a) {
    float af = a * 1.0 / 255;
    return af * af;
}

Image::Image(int w, int h, Color* colors) {
	width = w; height = h;
  for(int y = 0; y < height; y++) {
    png_bytep row = row_pointers[y];
    for(int x = 0; x < width; x++) {
      Color& color = colors[x + y * width];
      row[x * 4] = toByte(color.x);
      row[x * 4 + 1] = toByte(color.y);
      row[x * 4 + 2] = toByte(color.z);
      row[x * 4 + 3] = 255;
    }
  }
}

Color Image::getColor(int x, int y) {
	png_bytep row = row_pointers[y];
	return Color(toFloat(row[x * 4]), toFloat(row[x * 4 + 1]), toFloat(row[x * 4 + 2]));
}

bool Image::load(string& file) {
  FILE *fp = fopen(file.c_str(), "rb");

  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if(!png) return false;

  png_infop info = png_create_info_struct(png);
  if(!info) return false;

  if(setjmp(png_jmpbuf(png))) abort();

  png_init_io(png, fp);

  png_read_info(png, info);

  width      = png_get_image_width(png, info);
  height     = png_get_image_height(png, info);
  color_type = png_get_color_type(png, info);
  bit_depth  = png_get_bit_depth(png, info);

  // Read any color_type into 8bit depth, RGBA format.
  // See http://www.libpng.org/pub/png/libpng-manual.txt

  if(bit_depth == 16)
    png_set_strip_16(png);

  if(color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png);

  // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
  if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8(png);

  if(png_get_valid(png, info, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png);

  // These color_type don't have an alpha channel then fill it with 0xff.
  if(color_type == PNG_COLOR_TYPE_RGB ||
     color_type == PNG_COLOR_TYPE_GRAY ||
     color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

  if(color_type == PNG_COLOR_TYPE_GRAY ||
     color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png);

  png_read_update_info(png, info);

  row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
  for(int y = 0; y < height; y++) {
    row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
  }

  png_read_image(png, row_pointers);

  fclose(fp);

  return true;
}

bool Image::save(string& file) {
  int y;

  FILE *fp = fopen(file.c_str(), "wb");
  if(!fp) return false;

  png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) return false;

  png_infop info = png_create_info_struct(png);
  if (!info) return false;

  if (setjmp(png_jmpbuf(png))) abort();

  png_init_io(png, fp);

  // Output is 8bit depth, RGBA format.
  png_set_IHDR(
    png,
    info,
    width, height,
    8,
    PNG_COLOR_TYPE_RGBA,
    PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_DEFAULT,
    PNG_FILTER_TYPE_DEFAULT
  );
  png_write_info(png, info);

  // To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
  // Use png_set_filler().
  //png_set_filler(png, 0, PNG_FILLER_AFTER);

  png_write_image(png, row_pointers);
  png_write_end(png, NULL);

  for(int y = 0; y < height; y++) {
    free(row_pointers[y]);
  }
  free(row_pointers);

  fclose(fp);

  return true;
}