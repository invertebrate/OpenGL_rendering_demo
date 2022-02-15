/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 16:36:43 by veilo             #+#    #+#             */
/*   Updated: 2022/02/15 15:21:48 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"
#include "lm_matrix.h"

void objects_render(t_app *app) {

  if (app->object_count > 0) {
    for (uint i = 0; i < app->object_count; i++) {
      glUseProgram(app->shaders[app->objects[i]->shader]);
      glBindTexture(GL_TEXTURE_2D,
                    app->textures_gl[app->objects[i]->texture_id]);
      glBindVertexArray(app->VAOs[i]);
      int scalelocation =
          glGetUniformLocation(app->shaders[shader_type_default], "scale");
      glUniformMatrix4fv(scalelocation, 1, GL_FALSE, app->objects[i]->scale);
      int rotationlocation =
          glGetUniformLocation(app->shaders[shader_type_default], "rotation");
      glUniformMatrix4fv(rotationlocation, 1, GL_FALSE,
                         app->objects[i]->rotation);
      int positionlocation =
          glGetUniformLocation(app->shaders[shader_type_default], "position");
      glUniformMatrix4fv(positionlocation, 1, GL_FALSE,
                         app->objects[i]->position);
      int viewlocation =
          glGetUniformLocation(app->shaders[shader_type_default], "view");
      glUniformMatrix4fv(viewlocation, 1, GL_FALSE, app->view_matrix);
      int projectionlocation =
          glGetUniformLocation(app->shaders[shader_type_default], "projection");
      glUniformMatrix4fv(projectionlocation, 1, GL_TRUE,
                         app->projection_matrix);
      // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // draws all triangles with
      // line
      glDrawElements(GL_TRIANGLES, app->objects[i]->triangle_count * 3,
                     GL_UNSIGNED_INT, 0);
    }
  }
}

void render_frame(t_app *app) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  objects_render(app);
  SDL_GL_SwapWindow(app->window);
}
