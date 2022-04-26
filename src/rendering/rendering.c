/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 16:36:43 by veilo             #+#    #+#             */
/*   Updated: 2022/04/26 19:12:44 by veilo            ###   ########.fr       */
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
                        float *camera_view) {
  lm_mat4_identity(world);
  lm_mat4_multiply(object->scale, object->model_matrix, world);
  lm_mat4_multiply(object->rotation, world, world);
  lm_mat4_multiply(object->translation, world, world);
  lm_mat4_identity(camera_view);
  lm_mat4_multiply(app->view_matrix, world, camera_view);
}

// void render_object(t_app *app, t_3d_object *object) {
//   unsigned int tempshader = 0;
//   float world[16];
//   float screen[16];

//   tempshader = object->shader;
//   if (app->shadow_pass)
//     object->shader = shader_type_depth;
//   glUseProgram(app->shaders[object->shader]);
//   if (!app->shadow_pass)
//     set_texture_units(app, object);
//   glBindVertexArray(app->VAOs[object->object_id]);
//   calculate_matrices(app, object, world, screen);
//   lm_mat4_multiply(app->persp_proj, screen, screen);
//   glUniformMatrix4fv(
//       glGetUniformLocation(app->shaders[object->shader], "world"), 1,
//       GL_FALSE, world);
//   glUniformMatrix4fv(
//       glGetUniformLocation(app->shaders[object->shader], "screen"), 1,
//       GL_FALSE, screen);

//   if (object->shader == shader_type_lighting) {
//     glUniform3f(glGetUniformLocation(app->shaders[object->shader],
//     "viewpos"),
//                 app->camera_pos[0], app->camera_pos[1], app->camera_pos[2]);

//     glUniform1f(glGetUniformLocation(app->shaders[object->shader],
//                                      "material.specular_strength"),
//                 0.5);
//     glUniform3f(glGetUniformLocation(app->shaders[object->shader],
//     "ambient"),
//                 app->ambient_light[0], app->ambient_light[1],
//                 app->ambient_light[2]);
//     glUniformMatrix4fv(
//         glGetUniformLocation(app->shaders[object->shader], "light_view"), 1,
//         GL_FALSE, app->light_view);
//     glUniformMatrix4fv(
//         glGetUniformLocation(app->shaders[object->shader], "light_proj"), 1,
//         GL_FALSE, app->persp_proj);
//   }
//   glDrawElements(GL_TRIANGLES, object->triangle_count * 3, GL_UNSIGNED_INT,
//   0); object->shader = tempshader;
// }

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
  t_dir_light *light;

  light = app->d_lights[0];
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
              app->d_lights[0]->color[0], app->d_lights[0]->color[1],
              app->d_lights[0]->color[2]);

  glDrawElements(GL_TRIANGLES, object->triangle_count * 3, GL_UNSIGNED_INT, 0);
}

void update_light_data(t_app *app) {
  // for(lights){
  glUseProgram(app->shaders[shader_type_lighting]);
  glUniform3f(
      glGetUniformLocation(app->shaders[shader_type_lighting], "light_dir"),
      app->d_lights[0]->dir[0], app->d_lights[0]->dir[1],
      app->d_lights[0]->dir[2]);

  glUniform3f(
      glGetUniformLocation(app->shaders[shader_type_lighting], "light_color"),
      app->d_lights[0]->color[0], app->d_lights[0]->color[1],
      app->d_lights[0]->color[2]);

  glUniform3f(
      glGetUniformLocation(app->shaders[shader_type_lighting], "light_pos"),
      app->d_lights[0]->pos[0], app->d_lights[0]->pos[1],
      app->d_lights[0]->pos[2]);

  glUniform1f(glGetUniformLocation(app->shaders[shader_type_lighting],
                                   "light_strength"),
              app->d_lights[0]->strength);
  //}
}

void object_instantiate_render(t_app *app, t_3d_object *object,
                               float *translation_v) {
  float temp_transl[16];

  memcpy(temp_transl, object->translation, sizeof(temp_transl));
  lm_mat4_translate(object->translation, translation_v, object->translation);
  // object->shader = shader_type_default;
  render_object(app, object, object->shader);

  memcpy(object->translation, temp_transl, sizeof(temp_transl));
}

void render_ground(t_app *app, t_shader_type shader) {
  float scale = app->objects[1]->scale[0] * 2;
  t_3d_object *object = app->objects[1];
  t_shader_type tempshader = object->shader;
  object->shader = shader;
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
  object->shader = tempshader;
}

void render_debug(t_app *app) {
  render_object(app, app->objects[app->active_object],
                app->shaders[shader_type_debug]);
  render_object(app, app->objects[app->active_object + 2],
                app->shaders[shader_type_debug]);
  render_object(app, app->objects[app->active_object + 3],
                app->shaders[shader_type_debug]);
  render_ground(app, shader_type_debug);
}

/*
 */
void set_lighting_shader_uniforms(t_app *app) {
  glUniform3f(
      glGetUniformLocation(app->shaders[shader_type_lighting], "viewpos"),
      app->camera_pos[0], app->camera_pos[1], app->camera_pos[2]);

  glUniform1f(glGetUniformLocation(app->shaders[shader_type_lighting],
                                   "material.specular_strength"),
              0.5);
  glUniform3f(
      glGetUniformLocation(app->shaders[shader_type_lighting], "ambient"),
      app->ambient_light[0], app->ambient_light[1], app->ambient_light[2]);
}

void render_shadow_casters(t_app *app) { // maybe input shader type depending on
                                         // light
  render_object(app, app->objects[app->active_object], shader_type_depth);
  render_object(app, app->objects[app->active_object + 2], shader_type_depth);
  render_object(app, app->objects[app->active_object + 3], shader_type_depth);
  render_ground(app, shader_type_depth);
}

void d_light_data_into_shader(t_app *app, int index) {
  float dir[3];
  float up[3];
  float right[3];
  float guide[3];
  float pos[3];
  memcpy(guide, (float[3]){0, 1.0, 0}, sizeof(guide));
  memcpy(dir, app->d_lights[index]->dir, sizeof(dir));
  lm_vec3_sub(app->d_lights[index]->pos, (float[3]){0, 0, 0},
              dir); // look at origin
  memcpy(pos, app->d_lights[index]->pos, sizeof(pos));
  lm_vec3_scale(pos, -1, pos);
  lm_vec3_find_perp(dir, guide, up);
  lm_vec3_normalize(dir, dir);
  lm_vec3_normalize(up, up);
  lm_vec3_cross(up, dir, right);
  lm_vec3_normalize(right, right);
  lm_mat4_lookat(pos, dir, right, up, app->d_lights[index]->view);
  // memcpy(app->view_matrix, app->d_lights[0]->view, sizeof(app->view_matrix));
  char uniform_s[20];
  snprintf(uniform_s, 20, "light_view[%i]", index);
  glUniformMatrix4fv(
      glGetUniformLocation(app->shaders[shader_type_depth], "light_proj"), 1,
      GL_FALSE, app->persp_proj);
  glUniformMatrix4fv(
      glGetUniformLocation(app->shaders[shader_type_depth], uniform_s), 1,
      GL_FALSE, app->d_lights[index]->view); // light_view is screen atm

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
  t_point_light *light = app->p_lights[index];

  memcpy(guide, (float[3]){0, 1.0, 0}, sizeof(guide));
  memcpy(dir, app->p_lights[index]->dir, sizeof(dir));
  memcpy(pos, app->p_lights[index]->pos, sizeof(pos));
  lm_vec3_scale(pos, -1, pos);
  lm_vec3_find_perp(dir, guide, up);
  lm_vec3_normalize(dir, dir);
  lm_vec3_normalize(up, up);
  lm_vec3_cross(up, dir, right);
  lm_vec3_normalize(right, right);
  lm_mat4_lookat(pos, dir, right, up, app->p_lights[index]->view);

  lm_mat4_projection(90, 90, NEAR_PLANE, FAR_PLANE, light_proj, 1);
  lm_mat4_lookat(pos, v3inv(dir), v3inv(right), up, light->view + 16);
  lm_mat4_lookat(pos, v3id(up), right, v3inv(dir), light->view + 32);
  lm_mat4_lookat(pos, v3inv(up), right, v3id(dir), light->view + 48);
  lm_mat4_lookat(pos, right, v3inv(dir), up, light->view + 64);
  lm_mat4_lookat(pos, v3inv(right), v3id(dir), up, light->view + 80);
  // cube_view[16]     // dir = -dir, right = -right
  //     cube_view[32] // dir = up, up = -dir
  //     cube_view[48] // dir = -up, up = dir
  //     cube_view[64] // dir = right, right = -dir
  //     cube_view[80] // dir = -right, right = dir
  char uniform_s[20];
  snprintf(uniform_s, 20, "cube_view[%i]", index * 6);
  glUniformMatrix4fv(
      glGetUniformLocation(app->shaders[shader_type_depth], "cube_view"), 6,
      GL_FALSE, light->view);
  glUseProgram(app->shaders[shader_type_lighting]);
  glUniformMatrix4fv(
      glGetUniformLocation(app->shaders[shader_type_lighting], uniform_s), 6,
      GL_FALSE, light->view);
  glUseProgram(app->shaders[shader_type_depth]);
}

void pass_light_data_to_shadow_shader(t_app *app) {

  glUseProgram(app->shaders[shader_type_depth]);
  for (int i = 0; i < app->d_light_count; i++) {
    d_light_data_into_shader(app, i);
  }
  glUseProgram(app->shaders[shader_type_depth]);
  for (int i = 0; i < app->p_light_count; i++) {
    p_light_data_into_shader(app, i);
  }
}

void render_shadows(t_app *app) {

  // for (lights)
  // create_light_space_matrices(app);
  pass_light_data_to_shadow_shader(app); //

  glBindFramebuffer(GL_FRAMEBUFFER, app->depth_map_FBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                         app->depth_map, 0);
  glClear(GL_DEPTH_BUFFER_BIT);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);

  render_shadow_casters(app); // bind multiple depth maps, loop through
  // lights
  // in shader to render to different targets
  // }

  glDisable(GL_CULL_FACE);
  glCullFace(GL_BACK);
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

void draw_objects(t_app *app) { // maybe input shader type depending on
                                // light
  render_object(app, app->objects[app->active_object], shader_type_lighting);
  render_object(app, app->objects[app->active_object + 2],
                shader_type_lighting);
  render_object(app, app->objects[app->active_object + 3],
                shader_type_lighting);
  render_ground(app, shader_type_lighting);
}

void draw_scene(t_app *app) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  render_shadows(app);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, app->w_width, app->w_height);
  render_skybox(app);
  render_lights(app);
  // pass_light_data_to_drawing( app); // pass the light data e.g. color,
  // intensity for blending
  draw_objects(app); // use different texture units for different shadowmaps
  // render_debug(app);
  // and
  //  blend them in shader
  SDL_GL_SwapWindow(app->window);
}
/*
 */
