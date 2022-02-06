/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects_gl.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 15:26:52 by veilo             #+#    #+#             */
/*   Updated: 2022/02/06 18:36:59 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "h_opengl.h"

SDL_bool objects_gl_create(t_app *app) {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  // for (uint i = 0; i < 3 * 3; i++) {
  //   printf("vertex data: %f\n", app->objects[0]->vertex_data_array[8 * i +
  //   5]);
  // }
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, 3 * VERTEX_STRIDE_PUVN,
               app->objects[0]->vertex_data_array, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(0 * sizeof(float)));
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(5 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  unsigned int indices[] = {
      // note that we start from 0!
      0, 1, 2, // first triangle
  };
  unsigned int EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
  // end vao configuration

  app->VAO = VAO;

  return SDL_TRUE;
  (void)app;
  // (void)indices;
}