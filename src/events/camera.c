/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 15:39:47 by veilo             #+#    #+#             */
/*   Updated: 2022/04/15 15:19:45 by veilo            ###   ########.fr       */
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
  // rotate_camera(app);

  lm_vec3_scale(app->camera_pos, -1.0, invpos);
  lm_mat4_lookat(invpos, app->camera_dir, app->camera_right, app->camera_up,
                 app->view_matrix);
}