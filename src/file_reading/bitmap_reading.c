/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bitmap_reading.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 16:27:48 by veilo             #+#    #+#             */
/*   Updated: 2022/05/17 19:45:29 by veilo            ###   ########.fr       */
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

unsigned char *extract_header(unsigned char *contents) {
  unsigned char *header = NULL;

  header = (unsigned char *)calloc(sizeof(unsigned char), IMGR_BMP_HEADER_SIZE);
  memcpy(header, contents, IMGR_BMP_HEADER_SIZE);
  return (header);
}

int check_header(unsigned char *header, unsigned int file_size,
                 unsigned int *data_offset) {
  unsigned int bmp_size = 0;

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
  header += 2;
  data->bits_per_pixel = header[0] + (header[1] << 8);
  header += 2;
  if (data->bits_per_pixel == 32) {
  } else if (data->bits_per_pixel == 24) {
  } else {
    // only support 24 or 32 bit bmps
    return (0);
  }
  header += 8;
  data->res_x =
      header[0] + (header[1] << 8) + (header[2] << 16) + (header[3] << 24);
  header += 4;
  data->res_y =
      header[0] + (header[1] << 8) + (header[2] << 16) + (header[3] << 24);
  return (1);
}

unsigned int *parse_pixel_data(unsigned char *contents, t_texture_data *data) {
  unsigned int *pixel_data = NULL;
  unsigned int c = 0;

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

unsigned int *get_bitmap_from_file(char *filename, t_texture_data *data) {
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
