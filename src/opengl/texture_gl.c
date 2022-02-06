/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_gl.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 15:36:35 by veilo             #+#    #+#             */
/*   Updated: 2022/02/06 17:26:24 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bitmap_reading.h"
#include "h_opengl.h"

GLuint create_texture(unsigned int *pixel_data, t_texture_data *metadata) {
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  // set the texture wrapping/filtering options (on currently bound texture)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load and generate the texture
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, metadata->width, metadata->height,
                 0, GL_BGRA, GL_UNSIGNED_BYTE, pixel_data);
    glGenerateMipmap(GL_TEXTURE_2D); // load this
  } else {
    printf("Failed to load texture\n");
  }
  return (texture);
}