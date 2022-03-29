/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 15:39:47 by veilo             #+#    #+#             */
/*   Updated: 2022/03/29 18:59:50 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.h"
#include "events.h"
#include "lm_matrix.h"

void update_camera(t_app *app) {
  float invpos[3];

  lm_vec3_normalize(app->move_vector, app->move_vector);
  for (int i = 0; i < 3; i++) {
    app->camera_pos[i] += app->move_vector[i] * MOVE_SPEED * app->delta_time;
  }
  lm_vec3_rotate(app->camera_pos, (float[3]){0.0, 1.0, 0.0},
                 0.3 * app->delta_time, app->camera_pos);

  lm_vec3_sub(app->camera_pos, (float[3]){0.0, 0.0, 0.0}, app->camera_dir);
  lm_vec3_normalize(app->camera_dir, app->camera_dir);

  lm_vec3_find_perp(app->camera_dir, (float[3]){0.0, 1.0, 0.0}, app->camera_up);
  lm_vec3_normalize(app->camera_up, app->camera_up);
  lm_vec3_cross(app->camera_up, app->camera_dir, app->camera_right);
  lm_vec3_normalize(app->camera_right, app->camera_right);

  lm_vec3_scale(app->camera_pos, -1.0, invpos);
  lm_mat4_lookat(invpos, app->camera_dir, app->camera_right, app->camera_up,
                 app->view_matrix);
}