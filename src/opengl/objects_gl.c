/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects_gl.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 15:26:52 by veilo             #+#    #+#             */
/*   Updated: 2022/02/11 15:28:50 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "h_opengl.h"
#include "lm_matrix.h"

SDL_bool objects_gl_create(t_app *app) {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  uint tcount = 76;

  // for (uint i = 0; i < tcount * 3; i++) {
  //   printf("vertex data: %d: %f\n", i,
  //          app->objects[0]->vertex_data_array[8 * i + 5]);
  // }
  for (uint i = 0; i < tcount * 3 * 8; i++) {
    printf("vertex data: %d: %f\n", i, app->objects[0]->vertex_data_array[i]);
    if (i % 8 < 3) {
      printf("if data: %d\n", i);
      app->objects[0]->vertex_data_array[i] /= 3;
    }
  }

  float tempmat[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
  // for (uint i = 0; i < tcount * 3 * 8; i += 8) {
  //   if (1) {
  //     float tempvec[4] = {0, 0, 0, 0};
  //     memcpy(tempvec, app->objects[0]->vertex_data_array + i,
  //            sizeof(float) * 3);
  //     lm_vec3_rotate(tempvec, (float[3]){1, 1, 0}, (3.14159 / 2) * 14 / 17,
  //                    app->objects[0]->vertex_data_array + i, tempmat);
  //     // memcpy(app->objects[0]->vertex_data_array + i, tempvec, 4 * 4);
  //   }
  // }
  // (void)tempvec;
  (void)tempmat;
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, tcount * 3 * VERTEX_STRIDE_PUVN,
               app->objects[0]->vertex_data_array, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_STRIDE_PUVN,
                        (void *)(0 * sizeof(float)));
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, VERTEX_STRIDE_PUVN,
                        (void *)(3 * sizeof(float)));
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_STRIDE_PUVN,
                        (void *)(5 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  // unsigned int indices[] = {
  //     // note that we start from 0!
  //     0, 1, 2, // first triangle
  // };
  uint count = tcount * 3;
  unsigned int indices[count];
  for (uint i = 0; i < count; i++) {
    indices[i] = i;
    // printf("vertex: %f\n", app->objects[0]->vertex_data_array[indices[i]]);
  }

  unsigned int EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, tcount * 3 * 4, indices,
               GL_STATIC_DRAW);
  // end vao configuration

  app->VAO = VAO;

  return SDL_TRUE;
  (void)app;
  // (void)indices;
}