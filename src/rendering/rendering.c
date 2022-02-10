/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 16:36:43 by veilo             #+#    #+#             */
/*   Updated: 2022/02/10 15:53:44 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"

void objects_render(t_app *app) {
  glBindTexture(GL_TEXTURE_2D, app->textures_gl[0]);
  glBindVertexArray(app->VAO);
  glUseProgram(app->default_shader_program);
  // glDrawArrays(GL_TRIANGLES, 0, 3);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // draws all triangles with line
  glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT,
                 0); // change numbers so no segfault
  glBindTexture(GL_TEXTURE_2D, 0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // draws all triangles with line
  glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT,
                 0); // change numbers so no segfault
}

void render_frame(t_app *app) {
  glClear(GL_COLOR_BUFFER_BIT);
  objects_render(app);
  SDL_GL_SwapWindow(app->window);
}
