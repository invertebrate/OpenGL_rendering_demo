/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 16:36:43 by veilo             #+#    #+#             */
/*   Updated: 2022/02/11 18:08:47 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"

void objects_render(t_app *app) {
  float scale[16];
  float s = 0.003;
  memset(scale, 0, sizeof(scale));
  scale[0] = s;
  scale[5] = s;
  scale[10] = s;
  scale[15] = 1;
  glBindTexture(GL_TEXTURE_2D, app->textures_gl[0]);
  glBindVertexArray(app->VAOs[0]);
  // glUseProgram(app->default_shader_program);
  int scaleloc = glGetUniformLocation(app->default_shader_program, "scale");
  glUniformMatrix4fv(scaleloc, 1, GL_FALSE, scale);

  int transformlocation =
      glGetUniformLocation(app->default_shader_program, "transform"); //
  glUniformMatrix4fv(transformlocation, 1, GL_FALSE, app->matrix);
  // glDrawArrays(GL_TRIANGLES, 0, 3);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // draws all triangles with line
  glDrawElements(GL_TRIANGLES, app->objects[0]->triangle_count * 3,
                 GL_UNSIGNED_INT,
                 0); // change numbers so no segfault
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
