/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bitmap_reading.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 16:27:00 by veilo             #+#    #+#             */
/*   Updated: 2022/02/06 17:32:09 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITMAP_READING_H
#define BITMAP_READING_H

#include <stdlib.h>

#define IMGR_BMP_HEADER_SIZE 53
#define MAX_TEXTURES 512

typedef struct s_texture_data {
  int width;
  int height;
  unsigned int bits_per_pixel;
  unsigned int res_x;
  unsigned int res_y;
  unsigned int data_offset;
  unsigned int pixel_data_size;
  unsigned int *pixels;
  size_t file_size;
} t_texture_data;

unsigned int *get_bitmap_from_file(char *filename, t_texture_data *metadata);

#endif