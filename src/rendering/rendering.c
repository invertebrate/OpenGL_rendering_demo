/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 16:36:43 by veilo             #+#    #+#             */
/*   Updated: 2022/05/20 15:34:36 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"
#include "assets.h"
#include "lm_matrix.h"
#include "window.h"

int lm_max_int(int a, int b) { return (a > b ? a : b); }
int lm_min_int(int a, int b) { return (a < b ? a : b); }

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

  int shadowmap_count = 0;
  char sampler_s[20];
  snprintf(sampler_s, 20, "shadow_array[%d]", 0);
  for (int i = 0; i < MAX_LIGHTS; i++) {
    snprintf(sampler_s, 20, "shadow_array[%d]", i);

    glActiveTexture(TU_SHADOW_CUBEMAP_GL + shadowmap_count + i);
    glBindTexture(GL_TEXTURE_CUBE_MAP,
                  app->cube_shadow_map[lm_min_int(i, app->p_light_count)]);
    glUniform1i(glGetUniformLocation(app->shaders[object->shader], sampler_s),
                TU_SHADOW_CUBEMAP_GL + shadowmap_count + i - GL_TEXTURE0);
  }
}

void calculate_matrices(t_app *app, t_3d_object *object, float *world,
                        float *camera_view) {
  lm_mat4_identity(world);
  lm_mat4_multiply(object->scale, object->model_matrix, world);
  lm_mat4_multiply(object->rotation, world, world);
  lm_mat4_multiply(object->translation, world, world);
  lm_mat4_identity(camera_view);
  lm_mat4_multiply(app->view_matrix, world, camera_view);
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

void render_light(t_app *app, int index) {
  t_3d_object *object;
  t_point_light *light;

  light = app->p_lights[index];
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
  glUniform3fv(
      glGetUniformLocation(app->shaders[object->shader], "light_color"), 1,
      app->p_lights[index]->color);
  glUniform3fv(glGetUniformLocation(app->shaders[object->shader], "light_pos"),
               1, app->p_lights[index]->pos);

  glDrawElements(GL_TRIANGLES, object->triangle_count * 3, GL_UNSIGNED_INT, 0);
}

void set_lighting_shader_uniforms(t_app *app) {
  glUniform3fv(
      glGetUniformLocation(app->shaders[shader_type_lighting], "viewpos"), 1,
      app->camera_pos);

  glUniform1f(glGetUniformLocation(app->shaders[shader_type_lighting],
                                   "material.specular_strength"),
              0.5);
  glUniform3fv(
      glGetUniformLocation(app->shaders[shader_type_lighting], "ambient"), 1,
      app->ambient_light);
  glUniform1i(
      glGetUniformLocation(app->shaders[shader_type_lighting], "p_light_count"),
      app->p_light_count);
  glUniform1i(
      glGetUniformLocation(app->shaders[shader_type_lighting], "d_light_count"),
      app->d_light_count);
  glUniform1f(
      glGetUniformLocation(app->shaders[shader_type_lighting], "far_plane"),
      FAR_PLANE);
}

void render_shadow_casters(t_app *app, t_shader_type shader) {
  for (unsigned int i = 0; i < app->terrain_obj_count; i++)
    render_object(app, app->objects[i], shader);
}

void d_light_data_into_shader(t_app *app, int index) {
  float dir[3];
  float up[3];
  float right[3];
  float guide[3];
  float pos[3];
  memcpy(guide, (float[3]){0, 1.0, 0}, sizeof(guide));
  memcpy(dir, app->d_lights[index]->dir, sizeof(dir));
  lm_vec3_sub(app->d_lights[index]->pos, (float[3]){0, 0, 0}, dir);
  memcpy(pos, app->d_lights[index]->pos, sizeof(pos));
  lm_vec3_scale(pos, -1, pos);
  lm_vec3_find_perp(dir, guide, up);
  lm_vec3_normalize(dir, dir);
  lm_vec3_normalize(up, up);
  lm_vec3_cross(up, dir, right);
  lm_vec3_normalize(right, right);
  lm_mat4_lookat(pos, dir, right, up, app->d_lights[index]->view);
  char uniform_s[20];
  snprintf(uniform_s, 20, "light_view[%i]", index);
  glUniformMatrix4fv(
      glGetUniformLocation(app->shaders[shader_type_depth], "light_proj"), 1,
      GL_FALSE, app->persp_proj);
  glUniformMatrix4fv(
      glGetUniformLocation(app->shaders[shader_type_depth], uniform_s), 1,
      GL_FALSE, app->d_lights[index]->view);

  glUseProgram(app->shaders[shader_type_lighting]);
  glUniformMatrix4fv(
      glGetUniformLocation(app->shaders[shader_type_lighting], "light_proj"), 1,
      GL_FALSE, app->persp_proj);
  glUniformMatrix4fv(
      glGetUniformLocation(app->shaders[shader_type_lighting], uniform_s), 1,
      GL_FALSE, app->d_lights[index]->view);
  glUniform3fv(
      glGetUniformLocation(app->shaders[shader_type_lighting], "light_pos"), 1,
      app->d_lights[0]->pos);
  glUniform3fv(
      glGetUniformLocation(app->shaders[shader_type_lighting], "light_dir"), 1,
      app->d_lights[0]->dir);

  glUniform3fv(
      glGetUniformLocation(app->shaders[shader_type_lighting], "light_color"),
      1, app->d_lights[0]->color);
  glUniform1f(glGetUniformLocation(app->shaders[shader_type_lighting],
                                   "light_strength"),
              app->d_lights[0]->strength);
  glUseProgram(app->shaders[shader_type_depth]);
}

void p_light_data_into_shader(t_app *app, int index) {
  float dir[3];
  float up[3];
  float right[3];
  float guide[3];
  float pos[3];
  float light_proj[16];
  t_point_light *light = (t_point_light *)calloc(1, sizeof(t_point_light));
  memcpy(light, app->p_lights[index], sizeof(t_point_light));
  memcpy(pos, light->pos, sizeof(pos));
  lm_vec3_scale(pos, -1, pos);
  lm_vec3_normalize(dir, dir);
  lm_vec3_normalize(up, up);
  lm_vec3_cross(up, dir, right);
  lm_vec3_normalize(right, right);
  lm_mat4_projection(90, 90, NEAR_PLANE, FAR_PLANE, light_proj, 1);

  memcpy(dir, (float[3]){1.0, 0.0, 0.0}, sizeof(guide));
  memcpy(up, (float[3]){0.0, -1.0, 0.0}, sizeof(dir));
  lm_vec3_cross(up, dir, right);

  lm_mat4_lookat(pos, dir, right, v3inv(up), light->view);

  memcpy(dir, (float[3]){-1.0, 0.0, 0.0}, sizeof(guide));
  memcpy(up, (float[3]){0.0, -1.0, 0.0}, sizeof(dir));
  lm_vec3_cross(up, dir, right);

  lm_mat4_lookat(pos, dir, right, v3inv(up), light->view + 16);

  memcpy(dir, (float[3]){0.0, 1.0, 0.0}, sizeof(guide));
  memcpy(up, (float[3]){0.0, 0.0, 1.0}, sizeof(dir));
  lm_vec3_cross(up, dir, right);

  lm_mat4_lookat(pos, dir, right, v3inv(up), light->view + 32);
  memcpy(dir, (float[3]){0.0, -1.0, 0.0}, sizeof(guide));
  memcpy(up, (float[3]){0.0, 0.0, -1.0}, sizeof(dir));
  lm_vec3_cross(up, dir, right);

  lm_mat4_lookat(pos, dir, right, v3inv(up), light->view + 48);
  memcpy(dir, (float[3]){0.0, 0.0, 1.0}, sizeof(guide));
  memcpy(up, (float[3]){0.0, -1.0, 0.0}, sizeof(dir));
  lm_vec3_cross(up, dir, right);
  lm_mat4_lookat(pos, dir, right, v3inv(up), light->view + 64);

  memcpy(dir, (float[3]){0.0, 0.0, -1.0}, sizeof(guide));
  memcpy(up, (float[3]){0.0, -1.0, 0.0}, sizeof(dir));
  lm_vec3_cross(up, dir, right);
  lm_mat4_lookat(pos, dir, right, v3inv(up), light->view + 80);

  char uniform_s[20];
  glUseProgram(app->shaders[shader_type_cube_shadow]);

  glUniformMatrix4fv(
      glGetUniformLocation(app->shaders[shader_type_cube_shadow], "cube_view"),
      6, GL_FALSE, light->view);

  glUniformMatrix4fv(
      glGetUniformLocation(app->shaders[shader_type_cube_shadow], "light_proj"),
      1, GL_FALSE, light_proj);
  glUniform1f(
      glGetUniformLocation(app->shaders[shader_type_cube_shadow], "far_plane"),
      FAR_PLANE);
  glUniform3fv(
      glGetUniformLocation(app->shaders[shader_type_cube_shadow], "light_pos"),
      1, light->pos);
  glUniform1i(glGetUniformLocation(app->shaders[shader_type_cube_shadow],
                                   "light_count"),
              app->p_light_count);

  glUseProgram(app->shaders[shader_type_lighting]);
  snprintf(uniform_s, 20, "p_light_pos[%i]", index);
  glUniform3fv(
      glGetUniformLocation(app->shaders[shader_type_lighting], uniform_s), 1,
      light->pos);
  snprintf(uniform_s, 20, "p_light_color[%i]", index);
  glUniform3fv(
      glGetUniformLocation(app->shaders[shader_type_lighting], uniform_s), 1,
      light->color);
  snprintf(uniform_s, 20, "p_light_strength[%i]", index);
  glUniform1f(
      glGetUniformLocation(app->shaders[shader_type_lighting], uniform_s),
      light->strength);
  snprintf(uniform_s, 20, "p_light_falloff[%i]", index);
  glUniform1f(
      glGetUniformLocation(app->shaders[shader_type_lighting], uniform_s),
      light->falloff);
  glUniform1i(
      glGetUniformLocation(app->shaders[shader_type_lighting], "p_light_count"),
      app->p_light_count);

  glUseProgram(app->shaders[shader_type_cube_shadow]);
  free(light);
  light = NULL;
}

void render_shadows(t_app *app) {
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  for (int i = 0; i < app->d_light_count; i++) {
    glUseProgram(app->shaders[shader_type_depth]);
    d_light_data_into_shader(app, i);
    // glBindFramebuffer(GL_FRAMEBUFFER, app->depth_map_FBO);
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
    // GL_TEXTURE_2D,
    //                        app->shadow_map[0],
    //                        0); // change these to multiple lights
    // // glClear(GL_DEPTH_BUFFER_BIT);
    // glDrawBuffer(GL_NONE);
    // glReadBuffer(GL_NONE);
    // glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
  }
  for (int i = 0; i < app->p_light_count; i++) {
    glUseProgram(app->shaders[shader_type_cube_shadow]);
    p_light_data_into_shader(app, i);
    glBindFramebuffer(GL_FRAMEBUFFER, app->cube_depth_map_FBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                         app->cube_shadow_map[i], 0);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                         app->cube_depth_map, 0);
    unsigned int buffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, buffers);
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    render_shadow_casters(app, shader_type_cube_shadow);
  }
  glDisable(GL_CULL_FACE);
}

void render_object(t_app *app, t_3d_object *object, t_shader_type shader) {
  float world[16];
  float camera_view[16];
  glUseProgram(app->shaders[shader]);
  if (shader == shader_type_lighting) {
    set_texture_units(app, object);
    set_lighting_shader_uniforms(app);
  }
  glBindVertexArray(app->VAOs[object->object_id]);
  calculate_matrices(app, object, world, camera_view);
  lm_mat4_multiply(app->persp_proj, camera_view, camera_view);
  glUniformMatrix4fv(glGetUniformLocation(app->shaders[shader], "world"), 1,
                     GL_FALSE, world);
  glUniformMatrix4fv(glGetUniformLocation(app->shaders[shader], "camera_view"),
                     1, GL_FALSE, camera_view);
  glDrawElements(GL_TRIANGLES, object->triangle_count * 3, GL_UNSIGNED_INT, 0);
}

void draw_objects(t_app *app) {
  for (unsigned int i = 0; i < app->terrain_obj_count; i++)
    render_object(app, app->objects[i], shader_type_lighting);
}

void render_lights(t_app *app) {
  for (int i = 0; i < app->p_light_count; i++) {
    render_light(app, i);
  }
}

void draw_scene(t_app *app) {
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  render_shadows(app);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, app->w_width, app->w_height);
  // render_skybox(app);
  render_lights(app);
  draw_objects(app);
  SDL_GL_SwapWindow(app->window);
}
