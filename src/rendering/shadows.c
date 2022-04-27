/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadows.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/04 16:50:55 by veilo             #+#    #+#             */
/*   Updated: 2022/04/27 17:54:35 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lm_matrix.h"
#include "rendering.h"

// void render_shadow_pass(t_app *app) {
//   float tempview[16];
//   float dir[3];
//   float up[3];
//   float right[3];
//   float guide[3];

//   app->shadow_pass = 1;
//   // memcpy(dir, (float[3]){-1.0, -1.0, 1.0}, sizeof(dir));
//   memcpy(guide, (float[3]){0, 1.0, 0}, sizeof(guide));
//   lm_vec3_sub((float[3]){0.0, 0.0, 0.0}, app->d_lights[0]->pos,
//               app->d_lights[0]->dir); // direction towards scene origin
//   lm_vec3_scale(app->d_lights[0]->dir, -1,
//                 dir); // we live in an inverted world it seems
//   lm_vec3_find_perp(dir, guide, up);
//   lm_vec3_normalize(dir, dir);
//   lm_vec3_normalize(up, up);
//   lm_vec3_cross(up, dir, right);
//   lm_vec3_normalize(right, right);
//   memcpy(tempview, app->view_matrix, sizeof(tempview));
//   float invpos[3];
//   lm_vec3_scale(app->d_lights[0]->pos, -1, invpos);
//   lm_mat4_lookat(invpos, dir, right, up, app->view_matrix);
//   glBindFramebuffer(GL_FRAMEBUFFER, app->depth_map_FBO);
//   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
//                          app->depth_map, 0);
//   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//   glDrawBuffer(GL_NONE);
//   glReadBuffer(GL_NONE);
//   glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
//   glEnable(GL_CULL_FACE);
//   glCullFace(GL_FRONT);

//   render_object(app, app->objects[app->active_object]);
//   render_object(app, app->objects[app->active_object + 2]);
//   render_object(app, app->objects[app->active_object + 3]);
//   render_ground(app);
//   memcpy(app->light_view, app->view_matrix, sizeof(app->light_view));
//   memcpy(app->view_matrix, tempview, sizeof(tempview));
//   glDisable(GL_CULL_FACE);
//   glCullFace(GL_BACK);
//   app->shadow_pass = 0;
// }

// void shadow_matrices(t_app *app) {
//   float proj[16];
//   float lview[16];
//   float lspace[16];
//   float pos[3] = {0.0, 0.0, 0.0};

//   lm_mat4_identity(proj);
//   lm_mat4_identity(lview);
//   lm_mat4_projection(50, 50, 0.1, 100, proj, 1);
//   //   lm_mat4_lookat(pos, came)
//   memcpy(lview, app->view_matrix, sizeof(lview));
//   lm_mat4_multiply(proj, view, lspace);
// }

void generate_shadowmap(t_app *app) {
  unsigned int depth_map_FBO;
  unsigned int depth_map;

  glGenFramebuffers(1, &depth_map_FBO);
  glGenTextures(1, &depth_map);
  glBindTexture(GL_TEXTURE_2D, depth_map);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH,
               SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  app->depth_map_FBO = depth_map_FBO;
  app->depth_map = depth_map;
}

void generate_shadow_cubemap(t_app *app) {
  unsigned int cube_depth_map;
  unsigned int cube_depth_map_FBO;

  glGenFramebuffers(1, &cube_depth_map_FBO);

  glGenTextures(1, &cube_depth_map);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cube_depth_map);
  for (unsigned int i = 0; i < 6; ++i)
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
                 NULL);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  app->cube_depth_map_FBO = cube_depth_map_FBO;
  app->cube_depth_map = cube_depth_map;
}

// void render_cube_shadow_pass(t_app *app) {
//   float tempview[16];
//   float dir[3];
//   float up[3];
//   float right[3];
//   float guide[3];
//   float light_proj[16];
//   float cube_view[64];

//   app->shadow_pass = 1;
//   // transform stuff
//   memcpy(guide, (float[3]){0, 1.0, 0}, sizeof(guide));
//   lm_vec3_sub((float[3]){0.0, 0.0, 0.0}, app->d_lights[0]->pos,
//               app->d_lights[0]->dir);
//   lm_vec3_scale(app->d_lights[0]->dir, -1, dir);
//   lm_vec3_find_perp(dir, guide, up);
//   lm_vec3_normalize(dir, dir);
//   lm_vec3_normalize(up, up);
//   lm_vec3_cross(up, dir, right);
//   lm_vec3_normalize(right, right);
//   memcpy(tempview, app->view_matrix, sizeof(tempview));
//   float invpos[3];
//   lm_vec3_scale(app->d_lights[0]->pos, -1, invpos);
//   lm_mat4_lookat(invpos, dir, right, up, app->view_matrix);

//   // #define GL_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515
//   // #define GL_TEXTURE_CUBE_MAP_NEGATIVE_X 0x8516
//   // #define GL_TEXTURE_CUBE_MAP_POSITIVE_Y 0x8517
//   // #define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 0x8518
//   // #define GL_TEXTURE_CUBE_MAP_POSITIVE_Z 0x8519
//   // #define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 0x851A
//   // lm_mat4_projection(90, 90, NEAR_PLANE, FAR_PLANE, light_proj, 1);
//   // memcpy(cube_view, app->light_view, sizeof(app->light_view));
//   // lm_mat4_lookat(invpos, v3inv(dir), v3inv(right), up, cube_view + 16);
//   // lm_mat4_lookat(invpos, v3id(up), right, v3inv(dir), cube_view + 32);
//   // lm_mat4_lookat(invpos, v3inv(up), right, v3id(dir), cube_view + 48);
//   // lm_mat4_lookat(invpos, right, v3inv(dir), up, cube_view + 64);
//   // lm_mat4_lookat(invpos, v3inv(right), v3id(dir), up, cube_view + 80);
//   //     cube_view[16] // dir = -dir, right = -right
//   //     cube_view[32] // dir = up, up = -dir
//   //     cube_view[48] // dir = -up, up = dir
//   //     cube_view[64] // dir = right, right = -dir
//   //     cube_view[80] // dir = -right, right = dir
//   (void)cube_view;
//   (void)light_proj;
//   // gl stuff
//   glBindFramebuffer(GL_FRAMEBUFFER, app->cube_depth_map_FBO);
//   glClear(GL_DEPTH_BUFFER_BIT);
//   glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
//   app->cube_depth_map,
//                        0);
//   glDrawBuffer(GL_NONE);
//   glReadBuffer(GL_NONE);
//   glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
//   glEnable(GL_CULL_FACE);
//   glCullFace(GL_FRONT);

//   // render stuff
//   render_object(app, app->objects[app->active_object]);
//   render_object(app, app->objects[app->active_object + 2]);
//   render_object(app, app->objects[app->active_object + 3]);
//   render_ground(app);
//   memcpy(app->light_view, app->view_matrix, sizeof(app->light_view));
//   memcpy(app->view_matrix, tempview, sizeof(tempview));
//   glDisable(GL_CULL_FACE);
//   glCullFace(GL_BACK);
//   app->shadow_pass = 0;
// }