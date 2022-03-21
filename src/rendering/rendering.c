/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 16:36:43 by veilo             #+#    #+#             */
/*   Updated: 2022/03/21 15:23:09 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"
#include "lm_matrix.h"

void object_render(t_app *app, t_3d_object *object) {
  if (app->object_count > 0) {
    glUseProgram(app->shaders[object->shader]);

    glActiveTexture(TU_DIFFUSE_GL);
    glBindTexture(GL_TEXTURE_2D, app->textures_gl[object->texture_id]);
    glUniform1i(
        glGetUniformLocation(app->shaders[object->shader], "material.diffuse"),
        TU_DIFFUSE_GL - GL_TEXTURE0);

    glActiveTexture(TU_NORMALMAP_GL);
    glBindTexture(GL_TEXTURE_2D, app->normalmaps_gl[object->normalmap_id]);
    glUniform1i(glGetUniformLocation(app->shaders[object->shader],
                                     "material.normalmap"),
                TU_NORMALMAP_GL - GL_TEXTURE0);

    glBindVertexArray(app->VAOs[object->object_id]);
    glUniformMatrix4fv(
        glGetUniformLocation(app->shaders[object->shader], "model"), 1,
        GL_FALSE, object->model_matrix);
    glUniformMatrix4fv(
        glGetUniformLocation(app->shaders[object->shader], "scale"), 1,
        GL_FALSE, object->scale);
    glUniformMatrix4fv(
        glGetUniformLocation(app->shaders[object->shader], "rotation"), 1,
        GL_FALSE, object->rotation);
    glUniformMatrix4fv(
        glGetUniformLocation(app->shaders[object->shader], "translation"), 1,
        GL_FALSE, object->translation);
    glUniformMatrix4fv(
        glGetUniformLocation(app->shaders[object->shader], "view"), 1, GL_FALSE,
        app->view_matrix);
    glUniformMatrix4fv(
        glGetUniformLocation(app->shaders[object->shader], "projection"), 1,
        GL_TRUE, app->projection_matrix);
    if (object->shader == shader_type_lighting) {
      glUniform4f(
          glGetUniformLocation(app->shaders[object->shader], "light_dir"),
          app->light_dir[0], app->light_dir[1], app->light_dir[2], 1.0);

      glUniform3f(glGetUniformLocation(app->shaders[object->shader], "viewpos"),
                  app->camera_pos[0], app->camera_pos[1], app->camera_pos[2]);
      glUniform1f(glGetUniformLocation(app->shaders[object->shader],
                                       "material.specular_strength"),
                  0.5);
      glUniform3f(glGetUniformLocation(app->shaders[object->shader],
                                       "material.specular"),
                  1.0, 1.0, 1.0);
    }
    glDrawElements(GL_TRIANGLES, object->triangle_count * 3, GL_UNSIGNED_INT,
                   0);
  }
}

void render_frame(t_app *app) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  object_render(app, app->objects[app->active_object]);
  SDL_GL_SwapWindow(app->window);
}
