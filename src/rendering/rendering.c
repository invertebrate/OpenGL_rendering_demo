/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 16:36:43 by veilo             #+#    #+#             */
/*   Updated: 2022/04/19 15:41:11 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"
#include "assets.h"
#include "lm_matrix.h"
#include "window.h"

void set_texture_units(t_app *app, t_3d_object *object) {
  glActiveTexture(TU_DIFFUSE_GL);
  glBindTexture(GL_TEXTURE_2D, app->diffuses_gl[object->diffuse_id]);
  glUniform1i(
      glGetUniformLocation(app->shaders[object->shader], "material.diffuse"),
      TU_DIFFUSE_GL - GL_TEXTURE0);
  glActiveTexture(TU_NORMALMAP_GL);
  glBindTexture(GL_TEXTURE_2D, app->normalmaps_gl[object->normalmap_id]);
  glUniform1i(
      glGetUniformLocation(app->shaders[object->shader], "material.normalmap"),
      TU_NORMALMAP_GL - GL_TEXTURE0);
  glActiveTexture(TU_SPECULARMAP_GL);
  glBindTexture(GL_TEXTURE_2D, app->specularmaps_gl[object->specularmap_id]);
  glUniform1i(glGetUniformLocation(app->shaders[object->shader],
                                   "material.specularmap"),
              TU_SPECULARMAP_GL - GL_TEXTURE0);
  glActiveTexture(TU_SHADOWMAP_GL);
  glBindTexture(GL_TEXTURE_2D, app->depth_map);
  glUniform1i(glGetUniformLocation(app->shaders[object->shader], "shadowmap"),
              TU_SHADOWMAP_GL - GL_TEXTURE0);
}

void calculate_matrices(t_app *app, t_3d_object *object, float *world,
                        float *screen) {
  lm_mat4_identity(world);
  lm_mat4_multiply(object->scale, object->model_matrix, world);
  lm_mat4_multiply(object->rotation, world, world);
  lm_mat4_multiply(object->translation, world, world);
  lm_mat4_identity(screen);
  lm_mat4_multiply(app->view_matrix, world, screen);
}

void render_object(t_app *app, t_3d_object *object) {
  unsigned int tempshader = 0;
  float world[16];
  float screen[16];

  tempshader = object->shader;
  if (app->shadow_pass)
    object->shader = shader_type_depth;
  glUseProgram(app->shaders[object->shader]);
  if (!app->shadow_pass)
    set_texture_units(app, object);
  glBindVertexArray(app->VAOs[object->object_id]);
  calculate_matrices(app, object, world, screen);
  lm_mat4_multiply(app->persp_proj, screen, screen);
  glUniformMatrix4fv(
      glGetUniformLocation(app->shaders[object->shader], "world"), 1, GL_FALSE,
      world);
  glUniformMatrix4fv(
      glGetUniformLocation(app->shaders[object->shader], "screen"), 1, GL_FALSE,
      screen);

  if (object->shader == shader_type_lighting) {
    glUniform3f(glGetUniformLocation(app->shaders[object->shader], "viewpos"),
                app->camera_pos[0], app->camera_pos[1], app->camera_pos[2]);

    glUniform1f(glGetUniformLocation(app->shaders[object->shader],
                                     "material.specular_strength"),
                0.5);
    glUniform3f(glGetUniformLocation(app->shaders[object->shader], "ambient"),
                app->ambient_light[0], app->ambient_light[1],
                app->ambient_light[2]);
    glUniformMatrix4fv(
        glGetUniformLocation(app->shaders[object->shader], "light_view"), 1,
        GL_FALSE, app->light_view);
    glUniformMatrix4fv(
        glGetUniformLocation(app->shaders[object->shader], "light_proj"), 1,
        GL_FALSE, app->persp_proj);
  }
  glDrawElements(GL_TRIANGLES, object->triangle_count * 3, GL_UNSIGNED_INT, 0);
  object->shader = tempshader;
}

void render_skybox(t_app *app) {
  glDepthMask(GL_FALSE);
  glUseProgram(app->shaders[app->skybox_obj->shader]);
  float world[16];
  float screen[16];

  glActiveTexture(TU_DIFFUSE_GL);

  glBindTexture(GL_TEXTURE_CUBE_MAP, app->skybox_obj->diffuse_id);
  glUniform1i(
      glGetUniformLocation(app->shaders[app->skybox_obj->shader], "skybox"),
      TU_DIFFUSE_GL - GL_TEXTURE0);

  lm_mat4_identity(world);
  lm_mat4_identity(screen);
  lm_mat4_multiply(app->view_matrix, world, screen);
  lm_mat4_topleftmat3(screen, screen);
  lm_mat4_multiply(app->persp_proj, screen, screen);

  glUniformMatrix4fv(
      glGetUniformLocation(app->shaders[app->skybox_obj->shader], "screen"), 1,
      GL_FALSE, screen);
  glBindVertexArray(app->skybox_vao);
  glDrawElements(GL_TRIANGLES, app->skybox_obj->triangle_count * 3,
                 GL_UNSIGNED_INT, 0);
  glDepthMask(GL_TRUE);
}

void render_lights(t_app *app) {
  t_3d_object *object;
  t_light *light;

  light = app->lights[0];
  object = light->obj;

  glUseProgram(app->shaders[object->shader]);
  glBindVertexArray(app->VAOs[object->object_id]);
  float world[16];
  float screen[16];
  lm_mat4_identity(world);
  lm_mat4_multiply(object->rotation, object->model_matrix, world);
  lm_mat4_multiply(object->scale, world, world);
  lm_mat4_multiply(object->translation, world, world);
  lm_mat4_identity(screen);
  lm_mat4_multiply(app->view_matrix, world, screen);
  lm_mat4_multiply(app->persp_proj, screen, screen);
  glUniformMatrix4fv(
      glGetUniformLocation(app->shaders[object->shader], "world"), 1, GL_FALSE,
      world);
  glUniformMatrix4fv(
      glGetUniformLocation(app->shaders[object->shader], "screen"), 1, GL_FALSE,
      screen);
  glUniform3f(glGetUniformLocation(app->shaders[object->shader], "light_color"),
              app->lights[0]->color[0], app->lights[0]->color[1],
              app->lights[0]->color[2]);

  glDrawElements(GL_TRIANGLES, object->triangle_count * 3, GL_UNSIGNED_INT, 0);
}

void update_light_data(t_app *app) {
  // for(lights){
  glUseProgram(app->shaders[shader_type_lighting]);
  glUniform3f(
      glGetUniformLocation(app->shaders[shader_type_lighting], "light_dir"),
      app->lights[0]->dir[0], app->lights[0]->dir[1], app->lights[0]->dir[2]);

  glUniform3f(
      glGetUniformLocation(app->shaders[shader_type_lighting], "light_color"),
      app->lights[0]->color[0], app->lights[0]->color[1],
      app->lights[0]->color[2]);

  glUniform3f(
      glGetUniformLocation(app->shaders[shader_type_lighting], "light_pos"),
      app->lights[0]->pos[0], app->lights[0]->pos[1], app->lights[0]->pos[2]);

  glUniform1f(glGetUniformLocation(app->shaders[shader_type_lighting],
                                   "light_strength"),
              app->lights[0]->strength);
  //}
}

void object_instantiate_render(t_app *app, t_3d_object *object,
                               float *translation_v) {
  float temp_transl[16];

  memcpy(temp_transl, object->translation, sizeof(temp_transl));
  lm_mat4_translate(object->translation, translation_v, object->translation);
  // object->shader = shader_type_default;
  render_object(app, object);

  memcpy(object->translation, temp_transl, sizeof(temp_transl));
}

void render_ground(t_app *app) {
  float scale = app->objects[1]->scale[0] * 2;
  object_instantiate_render(app, app->objects[1], (float[3]){0.0, 0.0, 0.0});

  object_instantiate_render(app, app->objects[1], (float[3]){scale, 0.0, 0.0});
  object_instantiate_render(app, app->objects[1],
                            (float[3]){scale, 0.0, scale});
  object_instantiate_render(app, app->objects[1], (float[3]){0.0, 0.0, scale});

  object_instantiate_render(app, app->objects[1], (float[3]){-scale, 0.0, 0.0});
  object_instantiate_render(app, app->objects[1], (float[3]){0.0, 0.0, -scale});
  object_instantiate_render(app, app->objects[1],
                            (float[3]){-scale, 0.0, -scale});
  object_instantiate_render(app, app->objects[1],
                            (float[3]){scale, 0.0, -scale});
  object_instantiate_render(app, app->objects[1],
                            (float[3]){-scale, 0.0, scale});
}

void render_debug(t_app *app) {
  for (int i = 0; i < 4; i++)
    app->objects[i]->shader = shader_type_debug;
  render_object(app, app->objects[app->active_object]);
  render_object(app, app->objects[app->active_object + 2]);
  render_object(app, app->objects[app->active_object + 3]);
  render_ground(app);
  for (int i = 0; i < 4; i++)
    app->objects[i]->shader = shader_type_lighting;
}

void render_frame(t_app *app) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //   shadow_matrices(app);
  render_shadow_pass(app);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, app->w_width, app->w_height);

  update_light_data(app);
  render_skybox(app);
  render_lights(app);

  render_object(app, app->objects[app->active_object]);
  render_object(app, app->objects[app->active_object + 2]);
  render_object(app, app->objects[app->active_object + 3]);
  render_ground(app);
  // render_debug(app); //renders vertex normals

  SDL_GL_SwapWindow(app->window);
}
/*
void render_object(t_app *app, t_3d_object *object, t_shader_type shader) {
  float world[16];
  float screen[16];

  glUseProgram(shader);
  if (shader = shader_type_lighting)
    set_texture_units(app, object);
  glBindVertexArray(app->VAOs[object->object_id]);
  calculate_matrices(app, object, world, screen);
  lm_mat4_multiply(app->persp_proj, screen, screen);
  glUniformMatrix4fv(
      glGetUniformLocation(app->shaders[object->shader], "world"), 1, GL_FALSE,
      world);
  glUniformMatrix4fv(
      glGetUniformLocation(app->shaders[object->shader], "screen"), 1, GL_FALSE,
      screen);

  if (object->shader == shader_type_lighting) {
    glUniform3f(glGetUniformLocation(app->shaders[object->shader], "viewpos"),
                app->camera_pos[0], app->camera_pos[1], app->camera_pos[2]);

    glUniform1f(glGetUniformLocation(app->shaders[object->shader],
                                     "material.specular_strength"),
                0.5);
    glUniform3f(glGetUniformLocation(app->shaders[object->shader], "ambient"),
                app->ambient_light[0], app->ambient_light[1],
                app->ambient_light[2]);
    glUniformMatrix4fv(
        glGetUniformLocation(app->shaders[object->shader], "light_view"), 1,
        GL_FALSE, app->light_view);
    glUniformMatrix4fv(
        glGetUniformLocation(app->shaders[object->shader], "light_proj"), 1,
        GL_FALSE, app->persp_proj);
  }
  glDrawElements(GL_TRIANGLES, object->triangle_count * 3, GL_UNSIGNED_INT, 0);
  object->shader = tempshader;
}

render_shadow_casters(t_app *app) {
  render_object(app, app->objects[app->active_object], shader_type_cube_shadow);
  render_object(app, app->objects[app->active_object + 2],
                shader_type_cube_shadow);
  render_object(app, app->objects[app->active_object + 3],
                shader_type_cube_shadow);
  render_ground(app, shader_type_cube_shadow);
}

pass_light_data_to_shadow_shader(t_app *app) {
  struct light_data {
    int data;
  } useprogram(shadowshader);
  for (lights) {
    // getstructlocation...
    // view matrices
    //
    // lightcount
  }
}

void render_shadows(t_app *app) {
  create_light_space_matrices(app);
  pass_light_data_to_shadow_shader(app); //

  render_shadow_casters(app); // bind multiple depth maps, loop through lights
                              // in shader to render to different targets
}

void draw_scene(t_app *app) {
  render_shadows(app);
  draw_skybox(app);
  draw_lights(app);
  pass_light_data_to_drawing(
      app);          // pass the light data e.g. color, intensity for blending
  draw_objects(app); // use different texture units for different shadowmaps and
                     // blend them in shader
}*/