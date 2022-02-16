/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 16:36:43 by veilo             #+#    #+#             */
/*   Updated: 2022/02/16 15:26:59 by veilo            ###   ########.fr       */
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
      int modelloc =
          glGetUniformLocation(app->shaders[app->objects[i]->shader], "model");
      glUniformMatrix4fv(modelloc, 1, GL_FALSE, app->objects[i]->model_matrix);
      int scaleloc =
          glGetUniformLocation(app->shaders[app->objects[i]->shader], "scale");
      glUniformMatrix4fv(scaleloc, 1, GL_FALSE, app->objects[i]->scale);
      int rotloc = glGetUniformLocation(app->shaders[app->objects[i]->shader],
                                        "rotation");
      glUniformMatrix4fv(rotloc, 1, GL_FALSE, app->objects[i]->rotation);
      int transloc = glGetUniformLocation(app->shaders[app->objects[i]->shader],
                                          "translation");
      glUniformMatrix4fv(transloc, 1, GL_FALSE, app->objects[i]->translation);
      int viewloc =
          glGetUniformLocation(app->shaders[app->objects[i]->shader], "view");
      glUniformMatrix4fv(viewloc, 1, GL_FALSE, app->view_matrix);
      int projloc = glGetUniformLocation(app->shaders[app->objects[i]->shader],
                                         "projection");
      glUniformMatrix4fv(projloc, 1, GL_TRUE, app->projection_matrix);
      // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // draws all triangles with
      // line
      if (app->objects[i]->shader == shader_type_42_demo) {
        int blendloc = glGetUniformLocation(
            app->shaders[app->objects[i]->shader], "blend_value");
        glUniform1f(blendloc, app->demo_blend_value);
      }
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
