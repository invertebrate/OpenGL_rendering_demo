/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadows.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/04 16:50:55 by veilo             #+#    #+#             */
/*   Updated: 2022/05/10 16:17:11 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lm_matrix.h"
#include "rendering.h"

void generate_shadowmap(t_app *app) {
  unsigned int depth_map_FBO;
  unsigned int depth_map;

  glGenFramebuffers(1, &depth_map_FBO);
  for (int i = 0; i < app->d_light_count; ++i) {
    glGenTextures(1, &depth_map);
    glBindTexture(GL_TEXTURE_2D, depth_map);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH,
                 SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    app->shadow_map[i] = depth_map;
  }
  app->depth_map_FBO = depth_map_FBO;
}

void generate_shadow_cubemap(t_app *app) {
  unsigned int cube_depth_map;
  unsigned int cube_depth_map_FBO;

  glGenFramebuffers(1, &cube_depth_map_FBO);

  for (int i = 0; i < app->p_light_count; ++i) {
    glGenTextures(1, &cube_depth_map);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cube_depth_map);
    for (unsigned int i = 0; i < 6; ++i)
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RED, SHADOW_WIDTH,
                   SHADOW_HEIGHT, 0, GL_RED, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    app->cube_shadow_map[i] = cube_depth_map;
  }
  app->cube_depth_map_FBO = cube_depth_map_FBO;
}

void generate_depth_cubemap(t_app *app) {

  if (app->cube_depth_map_FBO)
    glBindFramebuffer(GL_FRAMEBUFFER, app->cube_depth_map_FBO);
  else
    return;
  glGenTextures(1, &(app->cube_depth_map));
  glBindTexture(GL_TEXTURE_CUBE_MAP, app->cube_depth_map);
  for (unsigned int i = 0; i < 6; ++i)
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
                 NULL);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
