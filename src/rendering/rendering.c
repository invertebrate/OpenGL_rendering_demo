/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 16:36:43 by veilo             #+#    #+#             */
/*   Updated: 2022/02/14 18:49:36 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"
#include "lm_matrix.h"

void objects_render(t_app *app) {
  float scale[16];
  float s = 0.005;
  memset(scale, 0, sizeof(scale));
  scale[0] = s;
  scale[5] = s;
  scale[10] = s;
  scale[15] = 1;

  float persp[16];
  persp[0] = 1;
  persp[5] = 1;
  persp[10] = 1;
  persp[15] = 1;
  lm_mat4_perspective(90, 90, 1, 2, persp);
  glUseProgram(app->shaders[shader_type_default]);
  glBindTexture(GL_TEXTURE_2D, app->textures_gl[0]);
  glBindVertexArray(app->VAOs[0]);
  // glUseProgram(app->default_shader_program);
  int scaleloc =
      glGetUniformLocation(app->shaders[shader_type_default], "scale");
  glUniformMatrix4fv(scaleloc, 1, GL_FALSE, scale);

  app->matrix[14] -= 200;
  app->matrix[13] -= 100;

  int transformlocation =
      glGetUniformLocation(app->shaders[shader_type_default], "transform"); //
  glUniformMatrix4fv(transformlocation, 1, GL_FALSE, app->matrix);

  int cameralocation =
      glGetUniformLocation(app->shaders[shader_type_default], "camera"); //
  glUniformMatrix4fv(cameralocation, 1, GL_FALSE, app->camera_position);

  int persplocation =
      glGetUniformLocation(app->shaders[shader_type_default], "perspective"); //
  glUniformMatrix4fv(persplocation, 1, GL_TRUE, persp);

  // glDrawArrays(GL_TRIANGLES, 0, 3);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // draws all triangles with line
  glDrawElements(GL_TRIANGLES, app->objects[0]->triangle_count * 3,
                 GL_UNSIGNED_INT, 0);
  // glBindVertexArray(app->VAOs[1]);
  // glDrawElements(GL_TRIANGLES, app->objects[1]->triangle_count * 3,
  //                GL_UNSIGNED_INT, 0);
  // glBindTexture(GL_TEXTURE_2D, 0);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // draws all triangles with line
  // glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT,
  //                0); // change numbers so no segfault
}

void render_frame(t_app *app) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  objects_render(app);
  SDL_GL_SwapWindow(app->window);
}
