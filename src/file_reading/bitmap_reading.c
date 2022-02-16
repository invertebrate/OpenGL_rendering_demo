/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bitmap_reading.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 16:27:48 by veilo             #+#    #+#             */
/*   Updated: 2022/02/16 15:36:51 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bitmap_reading.h"
#include "file_utils.h"

void bmpr_delete(void *data) {
  if (data != NULL) {
    free(data);
    data = NULL;
  }
}

// 0x00 - 0x35> header 0-53
unsigned char *extract_header(unsigned char *contents) {
  unsigned char *header = NULL;

  header = (unsigned char *)calloc(sizeof(unsigned char), IMGR_BMP_HEADER_SIZE);
  memcpy(header, contents, IMGR_BMP_HEADER_SIZE);
  return (header);
}

int check_header(unsigned char *header, uint file_size, uint *data_offset) {
  uint bmp_size = 0;

  if (header[0] != 'B' || header[1] != 'M') {
    printf("bmp reading failed: bad signature\n");
    return (0);
  }
  bmp_size =
      header[2] + (header[3] << 8) + (header[4] << 16) + (header[5] << 24);
  if (bmp_size != file_size) {
    printf("bmp reading failed: bad file size: size %u | %u filesize\n",
           bmp_size, file_size);
    return (0);
  }
  *data_offset =
      header[10] + (header[11] << 8) + (header[12] << 16) + (header[13] << 24);
  return (1);
}

int parse_info_header(unsigned char *header, t_texture_data *data) {
  header += 18;
  data->width =
      header[0] + (header[1] << 8) + (header[2] << 16) + (header[3] << 24);
  header += 4;
  data->height =
      header[0] + (header[1] << 8) + (header[2] << 16) + (header[3] << 24);
  header += 4;
  // planes 2
  header += 2;
  data->bits_per_pixel = header[0] + (header[1] << 8);
  header += 2;
  if (data->bits_per_pixel == 32) {
  } else if (data->bits_per_pixel == 24) {
  } else {
    // only support 24 or 32 bit bmps
    return (0);
  }
  // compression 4
  // image size 4
  header += 8;
  data->res_x =
      header[0] + (header[1] << 8) + (header[2] << 16) + (header[3] << 24);
  header += 4;
  data->res_y =
      header[0] + (header[1] << 8) + (header[2] << 16) + (header[3] << 24);
  // vertical resolution 4;
  // horizontal resolution 4;
  // 766 x 575
  return (1);
}

// Size 	4 bytes 	000Eh 	Size of InfoHeader =40
// Width 	4 bytes 	0012h 	Horizontal width of bitmap in pixels
// Height 	4 bytes 	0016h 	Vertical height of bitmap in pixels
// Planes 	2 bytes 	001Ah 	Number of Planes (=1)
// Bits Per Pixel 	2 bytes 	001Ch 	Bits per Pixel used to store
// palette entry information. This also identifies in an indirect way the number
// of possible colors. Possible values are: 1 = monochrome palette. NumColors =
// 1 4 = 4bit palletized. NumColors = 16 8 = 8bit palletized. NumColors = 256 16
// = 16bit RGB. NumColors = 65536 24 = 24bit RGB. NumColors = 16M Compression 4
// bytes 	001Eh 	Type of Compression 0 = BI_RGB   no compression 1 =
// BI_RLE8 8bit RLE encoding 2 = BI_RLE4 4bit RLE encoding ImageSize 	4 bytes
// 0022h 	(compressed) Size of Image It is valid to set this =0 if
// Compression = 0 XpixelsPerM 	4 bytes 	0026h 	horizontal resolution:
// Pixels/meter YpixelsPerM 	4 bytes 	002Ah 	vertical resolution:
// Pixels/meter Colors Used 	4 bytes 	002Eh 	Number of actually used
// colors. For a 8-bit / pixel bitmap this will be 100h or 256. Important Colors
// 4 bytes 	0032h 	Number of important colors 0 = all

unsigned int *
parse_pixel_data(unsigned char *contents,
                 t_texture_data *data) { // handle negative/positive height
  unsigned int *pixel_data = NULL;
  uint c = 0;

  data->pixel_data_size = (data->width + data->width % 4) * abs(data->height);
  if (!(pixel_data = (unsigned int *)calloc(sizeof(unsigned int),
                                            data->pixel_data_size))) {
    return (NULL);
  }
  if (data->bits_per_pixel == 32)
    for (int i = 0; i < abs(data->height); i++) {
      for (int k = 0; k < (data->width); k++) {
        pixel_data[i * data->width + k] = contents[c] + (contents[c + 1] << 8) +
                                          (contents[c + 2] << 16) +
                                          (contents[c + 3] << 24);
        c += 4;
      }
      c += c % 4;
    }
  else if (data->bits_per_pixel == 24) {
    for (int i = 0; i < abs(data->height); i++) {
      for (int k = 0; k < (data->width); k++) {
        pixel_data[i * data->width + k] = (contents[c]) +
                                          (contents[c + 1] << 8) +
                                          (contents[c + 2] << 16) + (255 << 24);
        c += 3;
      }
      c += c % 4;
    }
  }
  return (pixel_data);
}

unsigned int *get_bitmap_from_file(
    char *filename,
    t_texture_data *data) { // output unsigned char* ABGR//horizontal
                            // padding to nearest 4byte
  unsigned int *pixels = NULL;
  unsigned char *file_contents = NULL;
  unsigned char *header = NULL;
  t_texture_data tempdata;

  if (!(file_contents = file_contents_get(filename, &(tempdata.file_size), 0)))
    return (NULL);
  header = extract_header(file_contents);
  if (!(check_header(header, tempdata.file_size, &(tempdata.data_offset)))) {
    printf("bmp reading failed: bad file\n");
    return (NULL);
  }
  if (!(parse_info_header(header, &tempdata))) {
    return (NULL);
  }
  if (!(pixels = parse_pixel_data(file_contents + tempdata.data_offset,
                                  &tempdata))) {
    printf("pixel data parsing failed\n");
    return (NULL);
  }
  memcpy(data, &tempdata, sizeof(tempdata));
  data->pixels = pixels;
  bmpr_delete(file_contents);
  bmpr_delete(header);
  return (pixels);
}
// Signature 	2 bytes 	0000h 	'BM'
// FileSize 	4 bytes 	0002h 	File size in bytes
// reserved 	4 bytes 	0006h 	unused (=0)
// DataOffset 	4 bytes 	000Ah 	Offset from beginning of file to the
// beginning of the bitmap data