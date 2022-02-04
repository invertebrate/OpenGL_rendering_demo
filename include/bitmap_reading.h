/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bitmap_reading.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 16:27:00 by veilo             #+#    #+#             */
/*   Updated: 2022/02/04 17:09:40 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITMAP_READING_H
#define BITMAP_READING_H

#include <stdlib.h>

#define IMGR_BMP_HEADER_SIZE 53

typedef struct s_bitmap_metadata {
  int width;
  int height;
  unsigned int bits_per_pixel;
  unsigned int res_x;
  unsigned int res_y;
  unsigned int data_offset;
  unsigned int pixel_data_size;
  size_t file_size;
} t_bitmap_metadata;

unsigned char *get_bitmap_from_file(char *filename);

#endif