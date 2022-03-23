/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 15:47:20 by veilo             #+#    #+#             */
/*   Updated: 2022/03/23 17:24:34 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "events.h"
#include "app.h"
#include "asset_handling.h"

void rotate_light(t_app *app, SDL_Keycode code) {
  if (code == SDLK_j) {
    lm_vec3_rotate(app->light_dir, (float[3]){1, 0, 0},
                   app->delta_time * ROTATION_SPEED * 2, app->light_dir);
  } else if (code == SDLK_k) {
    lm_vec3_rotate(app->light_dir, (float[3]){0, 1, 0},
                   app->delta_time * ROTATION_SPEED * 2, app->light_dir);
  } else if (code == SDLK_l) {
    lm_vec3_rotate(app->light_dir, (float[3]){0, 0, 1},
                   app->delta_time * ROTATION_SPEED * 2, app->light_dir);
  }
  app->light_dir[3] = 1;
}

void events_handle(t_app *app, SDL_Event *event) {
  if (event->key.keysym.sym == SDLK_w) {
    if (event->type == SDL_KEYDOWN)
      lm_vec3_scale(app->camera_dir, -1.0, app->move_vector);
    if (event->type == SDL_KEYUP)
      lm_vec3_scale(app->move_vector, 0.0, app->move_vector);
  }
  if (event->key.keysym.sym == SDLK_s) {
    if (event->type == SDL_KEYDOWN) {
      lm_vec3_scale(app->camera_dir, 1.0, app->move_vector);
    }
    if (event->type == SDL_KEYUP)
      lm_vec3_scale(app->move_vector, 0.0, app->move_vector);
  }
  if (event->key.keysym.sym == SDLK_a) {
    if (event->type == SDL_KEYDOWN) {
      lm_vec3_scale(app->camera_right, -1.0, app->move_vector);
    }
    if (event->type == SDL_KEYUP)
      lm_vec3_scale(app->move_vector, 0.0, app->move_vector);
  }
  if (event->key.keysym.sym == SDLK_d) {
    if (event->type == SDL_KEYDOWN)
      lm_vec3_scale(app->camera_right, 1.0, app->move_vector);
    if (event->type == SDL_KEYUP)
      lm_vec3_scale(app->move_vector, 0.0, app->move_vector);
  }
  if (event->key.keysym.sym == SDLK_DOWN) {
    if (event->type == SDL_KEYDOWN) {
      lm_vec3_scale(app->camera_up, -1.0, app->move_vector);
    }
    if (event->type == SDL_KEYUP)
      lm_vec3_scale(app->move_vector, 0.0, app->move_vector);
  }
  if (event->key.keysym.sym == SDLK_UP) {
    if (event->type == SDL_KEYDOWN)
      lm_vec3_scale(app->camera_up, 1.0, app->move_vector);
    if (event->type == SDL_KEYUP)
      lm_vec3_scale(app->move_vector, 0.0, app->move_vector);
  }
  if (event->key.keysym.sym == SDLK_q) {
    if (event->type == SDL_KEYDOWN)
      app->rotating = -1;
    else if (event->type == SDL_KEYUP)
      app->rotating = 0;
  }
  if (event->key.keysym.sym == SDLK_e) {
    if (event->type == SDL_KEYDOWN)
      app->rotating = 1;
    else if (event->type == SDL_KEYUP)
      app->rotating = 0;
  }
  if ((event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_b))
    app->blending = SDL_TRUE;
  if ((event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_t))
    cycle_textures(app);
  if ((event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_n))
    cycle_normalmaps(app);
  if ((event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_o))
    cycle_objects(app);
  if ((event->type == SDL_KEYDOWN &&
       (event->key.keysym.sym == SDLK_j || event->key.keysym.sym == SDLK_k ||
        event->key.keysym.sym == SDLK_l)))
    rotate_light(app, event->key.keysym.sym);
  if (event->type == SDL_MOUSEMOTION) {
    app->yaw += -event->motion.xrel / (180 * M_PI);
    app->pitch += -event->motion.yrel / (180 * M_PI);
    if (app->pitch > (2 * M_PI) / 4)
      app->pitch = (2 * M_PI) / 4;
    if (app->pitch < -(2 * M_PI) / 4)
      app->pitch = -(2 * M_PI) / 4;

    float rotmatx[16];
    float rotmaty[16];
    float rotmat[16];

    lm_mat4_create_rotmat(rotmatx, (float[3]){0.0, 1.0, 0.0}, app->yaw);
    lm_mat4_create_rotmat(rotmaty, (float[3]){1.0, 0.0, 0.0}, app->pitch);
    lm_mat4_multiply(rotmatx, rotmaty, rotmat);

    lm_mat4vec4_mul(rotmat, VECTOR_Z, app->camera_dir);
    lm_mat4vec4_mul(rotmat, VECTOR_Y, app->camera_up);
    // lm_mat4vec4_mul(rotmat, VECTOR_X, app->camera_right);

    lm_vec3_cross(app->camera_up, app->camera_dir, app->camera_right);
    lm_vec3_normalize(app->camera_right, app->camera_right);
    // lm_mat4vec4_mul(rotmat, app->camera_right, app->camera_right);

    // ml_matrix4_rotation_x(ml_rad(app->player.rot_y), rotation_y);
    // ml_matrix4_rotation_y(ml_rad(app->player.rot_x), rotation_x);
    // ml_matrix4_mul(rotation_x, rotation_y, rotation);
    // ml_matrix4_mul_vec3(rotation, (t_vec3){0, 0, Z_DIR},
    // app->player.forward); ml_matrix4_mul_vec3(rotation, (t_vec3){X_DIR, 0,
    // 0}, app->player.sideways); ml_matrix4_mul_vec3(rotation, (t_vec3){0,
    // Y_DIR, 0}, app->player.up); ml_matrix4_inverse(rotation,
    // app->player.inv_rotation); ml_matrix4_copy(rotation,
    // app->player.rotation);

  } // check book;
}
