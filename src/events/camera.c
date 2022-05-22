/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 15:39:47 by veilo             #+#    #+#             */
/*   Updated: 2022/05/17 19:36:59 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.h"
#include "events.h"
#include "lm_matrix.h"

void camera_rotate_around(t_app *app, float *target) {
  float dir[3];
  float up[3];
  float right[3];

  lm_vec3_rotate(app->camera_pos, VECTOR_Y, app->delta_time * 0.05,
                 app->camera_pos);
  memcpy(up, app->camera_up, sizeof(up));
  lm_vec3_sub(app->camera_pos, target, dir);
  lm_vec3_normalize(dir, dir);
  lm_vec3_find_perp(dir, VECTOR_Y, up);
  lm_vec3_cross(up, dir, right);
  lm_vec3_normalize(right, right);
  memcpy(app->camera_dir, dir, sizeof(up));
  memcpy(app->camera_up, up, sizeof(up));
  memcpy(app->camera_right, right, sizeof(up));
}

void update_camera(t_app *app) {
  float invpos[3];

  lm_vec3_normalize(app->move_vector, app->move_vector);
  for (int i = 0; i < 3; i++) {
    app->camera_pos[i] += app->move_vector[i] * MOVE_SPEED * app->delta_time;
  }
  camera_rotate_around(app, (float[3]){0.0, 0.0, 0.0});
  lm_vec3_scale(app->camera_pos, -1.0, invpos);
  lm_mat4_lookat(invpos, app->camera_dir, app->camera_right, app->camera_up,
                 app->view_matrix);
}