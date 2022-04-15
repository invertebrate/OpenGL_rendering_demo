/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadows.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/04 16:50:55 by veilo             #+#    #+#             */
/*   Updated: 2022/04/15 15:36:52 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lm_matrix.h"
#include "rendering.h"

void render_shadow_pass(t_app *app) {
  float tempview[16];
  float dir[3];
  float up[3];
  float right[3];
  float guide[3];

  app->shadow_pass = 1;
  // memcpy(dir, (float[3]){-1.0, -1.0, 1.0}, sizeof(dir));
  memcpy(guide, (float[3]){0, 1.0, 0}, sizeof(guide));
  lm_vec3_sub((float[3]){0.0, 0.0, 0.0}, app->lights[0]->pos,
              app->lights[0]->dir); // direction towards scene origin
  lm_vec3_scale(app->lights[0]->dir, -1, dir);
  lm_vec3_find_perp(dir, guide, up);
  lm_vec3_normalize(dir, dir);
  lm_vec3_normalize(up, up);
  lm_vec3_cross(up, dir, right);
  lm_vec3_normalize(right, right);
  memcpy(tempview, app->view_matrix, sizeof(tempview));
  float invpos[3];
  lm_vec3_scale(app->lights[0]->pos, -1, invpos);
  lm_mat4_lookat(invpos, dir, right, up, app->view_matrix);
  glBindFramebuffer(GL_FRAMEBUFFER, app->depthMapFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                         app->depthMap, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);

  render_object(app, app->objects[app->active_object]);
  render_object(app, app->objects[app->active_object + 2]);
  render_object(app, app->objects[app->active_object + 3]);
  render_ground(app);
  memcpy(app->light_view, app->view_matrix, sizeof(app->light_view));
  memcpy(app->view_matrix, tempview, sizeof(tempview));
  glDisable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  app->shadow_pass = 0;
}

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
  unsigned int depthMapFBO;
  unsigned int depthMap;

  glGenFramebuffers(1, &depthMapFBO);
  glGenTextures(1, &depthMap);
  glBindTexture(GL_TEXTURE_2D, depthMap);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH,
               SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  app->depthMapFBO = depthMapFBO;
  app->depthMap = depthMap;
}
