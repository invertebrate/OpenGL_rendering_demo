/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 16:36:43 by veilo             #+#    #+#             */
/*   Updated: 2022/03/29 18:46:27 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"
#include "lm_matrix.h"

void object_render(t_app *app, t_3d_object *object) {
  if (app->object_count > 0) {
    glUseProgram(app->shaders[object->shader]);

    glActiveTexture(TU_DIFFUSE_GL);
    glBindTexture(GL_TEXTURE_2D, app->diffuses_gl[object->diffuse_id]);
    glUniform1i(
        glGetUniformLocation(app->shaders[object->shader], "material.diffuse"),
        TU_DIFFUSE_GL - GL_TEXTURE0);

    glActiveTexture(TU_NORMALMAP_GL);
    glBindTexture(GL_TEXTURE_2D, app->normalmaps_gl[object->normalmap_id]);
    glUniform1i(glGetUniformLocation(app->shaders[object->shader],
                                     "material.normalmap"),
                TU_NORMALMAP_GL - GL_TEXTURE0);

    glActiveTexture(TU_SPECULARMAP_GL);
    glBindTexture(GL_TEXTURE_2D, app->specularmaps_gl[object->specularmap_id]);
    glUniform1i(glGetUniformLocation(app->shaders[object->shader],
                                     "material.specularmap"),
                TU_SPECULARMAP_GL - GL_TEXTURE0);

    glBindVertexArray(app->VAOs[object->object_id]);
    float world[16];
    float screen[16];
    lm_mat4_identity(world);
    lm_mat4_multiply(object->rotation, object->model_matrix, world);
    lm_mat4_multiply(object->scale, world, world);
    lm_mat4_multiply(object->translation, world, world);
    lm_mat4_identity(screen);
    lm_mat4_multiply(app->view_matrix, world, screen);
    lm_mat4_multiply(app->projection_matrix, screen, screen);
    glUniformMatrix4fv(
        glGetUniformLocation(app->shaders[object->shader], "world"), 1,
        GL_FALSE, world);
    glUniformMatrix4fv(
        glGetUniformLocation(app->shaders[object->shader], "screen"), 1,
        GL_FALSE, screen);
    if (object->shader == shader_type_lighting) {
      glUniform4f(
          glGetUniformLocation(app->shaders[object->shader], "light_dir"),
          app->light_dir[0], app->light_dir[1], app->light_dir[2], 1.0);

      glUniform3f(glGetUniformLocation(app->shaders[object->shader], "viewpos"),
                  app->camera_pos[0], app->camera_pos[1], app->camera_pos[2]);
      glUniform1f(glGetUniformLocation(app->shaders[object->shader],
                                       "material.specular_strength"),
                  1.0);
    }
    glDrawElements(GL_TRIANGLES, object->triangle_count * 3, GL_UNSIGNED_INT,
                   0);
  }
}

void render_skybox(t_app *app) {
  glDepthMask(GL_FALSE);
  glUseProgram(app->shaders[app->skybox_obj->shader]);
  float world[16];
  float screen[16];

  glActiveTexture(TU_DIFFUSE_GL);

  glBindTexture(GL_TEXTURE_CUBE_MAP, app->skybox_obj->diffuse_id);
  // printf("skybox texture: %u other texture %u\n",
  // app->skybox_obj->diffuse_id,
  //        app->diffuses_gl[app->objects[0]->diffuse_id]);
  glUniform1i(
      glGetUniformLocation(app->shaders[app->skybox_obj->shader], "skybox"),
      TU_DIFFUSE_GL - GL_TEXTURE0);

  lm_mat4_identity(world);
  lm_mat4_identity(screen);
  lm_mat4_multiply(app->view_matrix, world, screen);
  lm_mat4_topleftmat3(screen, screen); // investigate this
  lm_mat4_multiply(app->projection_matrix, screen, screen);

  glUniformMatrix4fv(
      glGetUniformLocation(app->shaders[app->skybox_obj->shader], "screen"), 1,
      GL_FALSE, screen);
  glBindVertexArray(app->skybox_vao);
  glDrawElements(GL_TRIANGLES, app->skybox_obj->triangle_count * 3,
                 GL_UNSIGNED_INT, 0);
  glDepthMask(GL_TRUE);
}

void render_frame(t_app *app) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  render_skybox(app);

  object_render(app, app->objects[app->active_object]);
  object_render(app, app->objects[1]);
  SDL_GL_SwapWindow(app->window);
}
