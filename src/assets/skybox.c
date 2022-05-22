/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skybox.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 15:12:16 by veilo             #+#    #+#             */
/*   Updated: 2022/05/17 19:45:10 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.h"
#include "assets.h"
#include "h_opengl.h"
#include "texture_gl.h"

void *skybox_model_create(t_app *app) {
  t_3d_object *skybox;

  if (!(skybox = obj_read_from_file("resources/skybox/skybox.obj"))) {
    printf("ERROR: Object reading failed for file: %s\n",
           "resources/skybox/skybox.obj");
    return (NULL);
  }
  skybox->shader = shader_type_skybox;
  app->skybox_obj = skybox;
  return (skybox);
}

SDL_bool skybox_vao_create(t_app *app) {
  unsigned int VBO;
  unsigned int EBO;
  unsigned int VAO;
  unsigned int tcount = 0;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glGenBuffers(1, &VBO);
  tcount = app->skybox_obj->triangle_count;
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, tcount * 3 * VERTEX_STRIDE_PUVNTB,
               app->skybox_obj->vertex_data_array, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_STRIDE_PUVNTB,
                        (void *)(0 * sizeof(float)));
  glEnableVertexAttribArray(0);
  unsigned int indices[tcount * 3];
  for (unsigned int i = 0; i < tcount * 3; i++)
    indices[i] = i;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, tcount * 3 * sizeof(float), indices,
               GL_STATIC_DRAW);
  app->skybox_vao = VAO;
  return SDL_TRUE;
}

void load_skybox(t_app *app) {
  unsigned int textureID;
  if (!(skybox_model_create(app))) {
    printf("ERROR: Skybox model creation failed.\n");
    return;
  }
  skybox_vao_create(app);
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
  t_texture_data tempdata[6];
  char *filename0 = "resources/skybox/right.bmp";
  char *filename1 = "resources/skybox/left.bmp";
  char *filename2 = "resources/skybox/bottom.bmp";
  char *filename3 = "resources/skybox/top.bmp";
  char *filename4 = "resources/skybox/front.bmp";
  char *filename5 = "resources/skybox/back.bmp";
  char *filenames[6];
  filenames[0] = filename0;
  filenames[1] = filename1;
  filenames[2] = filename2;
  filenames[3] = filename3;
  filenames[4] = filename4;
  filenames[5] = filename5;
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  for (int i = 0; i < 6; i++) {
    if (!(tempdata[i].pixels =
              get_bitmap_from_file(filenames[i], &(tempdata[i])))) {
      printf("ERROR: Skybox loading failed\n");
      return;
    }
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA,
                 tempdata[i].width, abs(tempdata[i].height), 0, GL_BGRA,
                 GL_UNSIGNED_BYTE, tempdata[i].pixels);
    free(tempdata[i].pixels);
    tempdata[i].pixels = NULL;
  }
  app->skybox_obj->diffuse_id = textureID;
}
