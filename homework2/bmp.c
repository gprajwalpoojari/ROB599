#include "bmp.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
// calculate the number of bytes of memory needed to serialize the bitmap
// that is, to write a valid bmp file to memory
size_t bmp_calculate_size(bitmap_t *bmp) {

  size_t fh_size = sizeof(BITMAPFILEHEADER);
  size_t ih_size = sizeof(BITMAPINFOHEADER);
  size_t img_size = bmp->width * bmp->height * sizeof(color_bgr_t);
  size_t bitmap_size = fh_size + ih_size + img_size;
  return bitmap_size;
}

// write the bmp file to memory at data, which must be at least
// bmp_calculate_size large.
void bmp_serialize(bitmap_t *bmp, uint8_t *data) {

  BITMAPFILEHEADER file_header = {0};
  file_header.bfType1 = 'B';
  file_header.bfType2 = 'M';
  file_header.bfSize = bmp_calculate_size(bmp);
  file_header.bfReserved1 = 0;
  file_header.bfReserved2 = 0;
  file_header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

  BITMAPINFOHEADER info_header = {0};
  info_header.biSize =  sizeof(BITMAPINFOHEADER);
  info_header.biWidth = bmp->width;
  info_header.biHeight = bmp->height;
  info_header.biPlanes = 1;
  info_header.biBitCount =  24;
  info_header.biCompression = 0;
  info_header.biSizeImage = 0 ;
  info_header.biXPelsPerMeter = 2835;
  info_header.biYPelsPerMeter = 2835;
  info_header.biClrUsed = 0;
  info_header.biClrImportant = 0;

  uint8_t *data_out = data;
  memcpy(data_out, &file_header, sizeof(file_header));
  data_out += sizeof(file_header);

  memcpy(data_out, &info_header, sizeof(info_header));
  data_out += sizeof(info_header);

  for (int i = bmp->height -1; i >= 0; i--){
    memcpy(data_out, &bmp->data[i * bmp->width], bmp->width * sizeof(color_bgr_t));
    data_out += bmp->width * sizeof(color_bgr_t);

  }
}
