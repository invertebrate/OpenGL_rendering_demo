/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 16:36:43 by veilo             #+#    #+#             */
/*   Updated: 2022/05/05 19:03:32 by veilo            ###   ########.fr       */
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

  int shadowmap_count = 0;
  char sampler_s[20];
  for (int i = 0; i < app->d_light_count; i++) {
    glActiveTexture(TU_SHADOWMAP_GL + i);
    glBindTexture(GL_TEXTURE_2D,
                  app->shadow_map[i]); // change these to multiple lights
    glUniform1i(glGetUniformLocation(app->shaders[object->shader], sampler_s),
                TU_SHADOWMAP_GL + i - GL_TEXTURE0);
    shadowmap_count++;
  }
  for (int i = 0; i < app->p_light_count; i++) {
    snprintf(sampler_s, 20, "shadow_cubemap[%i]", i);

    glActiveTexture(TU_SHADOW_CUBEMAP_GL + shadowmap_count + i);
    glBindTexture(GL_TEXTURE_CUBE_MAP,
                  app->cube_shadow_map[i]); // change these to multiple lights
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
  // lm_mat4_multiply(app->d_lights[0]->view, world, camera_view);
}

void render_skybox(t_app *app) {
  glDepthMask(GL_FALSE);
  glUseProgram(app->shaders[app->skybox_obj->shader]);
  float world[16];
  float screen[16];

  glActiveTexture(TU_DIFFUSE_GL);
  glBindTexture(GL_TEXTURE_CUBE_MAP,
                app->cube_shadow_map[0]); // change these to multiple lights
  // glBindTexture(GL_TEXTURE_CUBE_MAP, app->skybox_obj->diffuse_id);
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
  t_point_light *light;

  light = app->p_lights[0];
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
      app->p_lights[0]->color);

  glDrawElements(GL_TRIANGLES, object->triangle_count * 3, GL_UNSIGNED_INT, 0);
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
  // render_object(app, app->objects[app->active_object + 2],
  //               app->shaders[shader_type_debug]);
  // render_object(app, app->objects[app->active_object + 3],
  //               app->shaders[shader_type_debug]);
  // render_ground(app, shader_type_debug);
}

/*
 */
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
}

void render_shadow_casters(t_app *app,
                           t_shader_type shader) { // maybe input shader type
                                                   // depending on light
  render_object(app, app->objects[app->active_object], shader);
  // render_object(app, app->objects[app->active_object + 2], shader);
  // render_object(app, app->objects[app->active_object + 3], shader);
  // render_object(app, app->objects[app->active_object + 4], shader);
  render_ground(app, shader);
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

void p_light_data_into_shader(t_app *app, int index) { // this here fix this
  float dir[3];
  float up[3];
  float right[3];
  float guide[3];
  float pos[3];
  float light_proj[16];
  t_point_light *light = app->p_lights[index];
  light = malloc(sizeof(t_point_light));
  memcpy(light, app->p_lights[0], sizeof(*(app->p_lights[0])));

  memcpy(guide, (float[3]){0, 1.0, 0.0}, sizeof(guide));
  memcpy(up, guide, sizeof(guide));
  memcpy(dir, (float[3]){1.0, 0.0, 0.0}, sizeof(dir));
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
  //
  memcpy(dir, (float[3]){0.0, -1.0, 0.0}, sizeof(guide));
  memcpy(up, (float[3]){0.0, 0.0, -1.0}, sizeof(dir));
  lm_vec3_cross(up, dir, right);
  lm_mat4_lookat(pos, dir, right, v3inv(up), light->view + 48);
  //
  memcpy(dir, (float[3]){0.0, 0.0, 1.0}, sizeof(guide));
  memcpy(up, (float[3]){0.0, -1.0, 0.0}, sizeof(dir));
  lm_vec3_cross(up, dir, right);
  lm_mat4_lookat(pos, dir, right, v3inv(up), light->view + 64);

  memcpy(dir, (float[3]){0.0, 0.0, -1.0}, sizeof(guide));
  memcpy(up, (float[3]){0.0, -1.0, 0.0}, sizeof(dir));
  lm_vec3_cross(up, dir, right);
  lm_mat4_lookat(pos, dir, right, v3inv(up), light->view + 80);

  char uniform_s[20];
  snprintf(uniform_s, 20, "cube_view[%i]", index * 6);

  glUseProgram(app->shaders[shader_type_cube_shadow]);

  glUniformMatrix4fv(
      glGetUniformLocation(app->shaders[shader_type_cube_shadow], uniform_s), 6,
      GL_FALSE, light->view);
  glUniformMatrix4fv(
      glGetUniformLocation(app->shaders[shader_type_cube_shadow], "light_proj"),
      1, GL_FALSE, light_proj);
  glUniform1f(
      glGetUniformLocation(app->shaders[shader_type_cube_shadow], "far_plane"),
      FAR_PLANE);
  glUniform3fv(
      glGetUniformLocation(app->shaders[shader_type_cube_shadow], "light_pos"),
      1, light->pos);

  // glActiveTexture(TU_SHADOW_CUBEMAP_GL + app->d_light_count + index);
  // glBindTexture(GL_TEXTURE_CUBE_MAP,
  //               app->cube_depth_map[index]); // change these to multiple
  //               lights
  // glUniform1i(glGetUniformLocation(app->shaders[object->shader], sampler_s),
  //             TU_SHADOW_CUBEMAP_GL + shadowmap_count + i - GL_TEXTURE0);

  glActiveTexture(TU_SHADOW_CUBEMAP_GL);
  glBindTexture(GL_TEXTURE_CUBE_MAP,
                app->cube_shadow_map[0]); // change these to multiple lights
  glUniform1i(glGetUniformLocation(app->shaders[shader_type_cube_shadow],
                                   "shadow_cubemap"),
              TU_SHADOW_CUBEMAP_GL - GL_TEXTURE0);

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

  glUseProgram(app->shaders[shader_type_cube_shadow]);
}

void pass_light_data_to_shadow_shader(t_app *app) {

  glUseProgram(app->shaders[shader_type_depth]);
  for (int i = 0; i < app->d_light_count; i++) {
    d_light_data_into_shader(app, i);
  }
  glUseProgram(app->shaders[shader_type_cube_shadow]);
  for (int i = 0; i < app->p_light_count;
       i++) { //!!!change to loop all lights!!!
    p_light_data_into_shader(app, i);
  }
}

void render_shadows(t_app *app) {

  pass_light_data_to_shadow_shader(app); //

  glBindFramebuffer(GL_FRAMEBUFFER, app->depth_map_FBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                         app->shadow_map[0],
                         0); // change these to multiple lights
  // glClear(GL_DEPTH_BUFFER_BIT);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);

  render_shadow_casters(
      app, shader_type_depth); // bind multiple depth maps, loop through
  // lights in shader to render to different targets
  glBindFramebuffer(GL_FRAMEBUFFER,
                    app->cube_depth_map_FBO); // necessary??
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                       app->cube_shadow_map[0], 0);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, app->cube_depth_map,
                       0);
  // glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
  //                      app->cube_depth_map[1], 1);
  unsigned int buffers[1] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, buffers);
  glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
  glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  render_shadow_casters(app, shader_type_cube_shadow);

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

void draw_objects(t_app *app) { // maybe input shader type depending on
                                // light

  render_object(app, app->objects[app->active_object], shader_type_lighting);
  // render_object(app, app->objects[app->active_object + 2],
  //               shader_type_lighting);
  // render_object(app, app->objects[app->active_object + 3],
  //               shader_type_lighting);
  // render_object(app, app->objects[app->active_object + 4],
  //               shader_type_lighting);
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
  // glEnable(GL_CULL_FACE);
  // glCullFace(GL_BACK);
  draw_objects(app); // use different texture units for different shadowmaps
  // render_debug(app);
  // and
  //  blend them in shader
  SDL_GL_SwapWindow(app->window);
  // light count being 16 sus bc diff normal etc maps take slots
  // if each light has their own map
}
/*
 */

// TODO:
//[ ]Cleanup rendering
//[ ]Add multiple light support
///+[ ]Light data in shader
///+[ ]Blending shadows and light values

//[ ]Create a scene
///+[ ]Models, lights, camera
//[ ]Create camera path and animate
//[ ]Light path and animation
