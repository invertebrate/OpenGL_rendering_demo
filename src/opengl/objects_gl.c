/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects_gl.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 15:26:52 by veilo             #+#    #+#             */
/*   Updated: 2022/03/19 16:42:32 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "h_opengl.h"
#include "lm_matrix.h"

SDL_bool vaos_create(t_app *app) {
  unsigned int VBO;
  unsigned int EBO;
  unsigned int VAO;
  unsigned int tcount = 0;
  for (unsigned int i = 0; i < app->object_count; i++) {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    tcount = app->objects[i]->triangle_count;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, tcount * 3 * VERTEX_STRIDE_PUVNTB,
                 app->objects[i]->vertex_data_array, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_STRIDE_PUVNTB,
                          (void *)(0 * sizeof(float)));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, VERTEX_STRIDE_PUVNTB,
                          (void *)(3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_STRIDE_PUVNTB,
                          (void *)(5 * sizeof(float)));
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, VERTEX_STRIDE_PUVNTB,
                          (void *)(8 * sizeof(float)));
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, VERTEX_STRIDE_PUVNTB,
                          (void *)(11 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    unsigned int indices[tcount * 3];
    for (unsigned int i = 0; i < tcount * 3; i++)
      indices[i] = i;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, tcount * 3 * sizeof(float), indices,
                 GL_STATIC_DRAW);
    app->VAOs[i] = VAO;
  }
  return SDL_TRUE;
}