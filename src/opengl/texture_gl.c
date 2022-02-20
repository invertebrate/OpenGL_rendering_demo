/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_gl.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 15:36:35 by veilo             #+#    #+#             */
/*   Updated: 2022/02/20 16:46:14 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "texture_gl.h"

unsigned int create_texture(t_texture_data *data) {
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  if (data->pixels) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, data->width, abs(data->height), 0,
                 GL_BGRA, GL_UNSIGNED_BYTE, data->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    printf("Failed to create texture\n");
  }
  return (texture);
}
