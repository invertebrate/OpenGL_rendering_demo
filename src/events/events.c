/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 15:47:20 by veilo             #+#    #+#             */
/*   Updated: 2022/03/22 17:48:03 by veilo            ###   ########.fr       */
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
      memcpy(app->move_vector, (float[3]){0, 0, -1.0}, sizeof(float) * 3);
    if (event->type == SDL_KEYUP)
      lm_vec3_scale(app->move_vector, 0.0, app->move_vector);
  }
  if (event->key.keysym.sym == SDLK_s) {
    if (event->type == SDL_KEYDOWN) {
      memcpy(app->move_vector, (float[3]){0, 0, 1.0}, sizeof(float) * 3);
    }
    if (event->type == SDL_KEYUP)
      lm_vec3_scale(app->move_vector, 0.0, app->move_vector);
  }
  if (event->key.keysym.sym == SDLK_a) {
    if (event->type == SDL_KEYDOWN) {
      memcpy(app->move_vector, (float[3]){-1.0, 0, 0}, sizeof(float) * 3);
    }
    if (event->type == SDL_KEYUP)
      lm_vec3_scale(app->move_vector, 0.0, app->move_vector);
  }
  if (event->key.keysym.sym == SDLK_d) {
    if (event->type == SDL_KEYDOWN)
      memcpy(app->move_vector, (float[3]){1.0, 0, 0}, sizeof(float) * 3);
    if (event->type == SDL_KEYUP)
      lm_vec3_scale(app->move_vector, 0.0, app->move_vector);
  }
  if (event->key.keysym.sym == SDLK_DOWN) {
    if (event->type == SDL_KEYDOWN) {
      memcpy(app->move_vector, (float[3]){0, -1.0, 0}, sizeof(float) * 3);
    }
    if (event->type == SDL_KEYUP)
      lm_vec3_scale(app->move_vector, 0.0, app->move_vector);
  }
  if (event->key.keysym.sym == SDLK_UP) {
    if (event->type == SDL_KEYDOWN)
      memcpy(app->move_vector, (float[3]){0, 1.0, 0}, sizeof(float) * 3);
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
    app->yaw += event->motion.xrel / (180 * M_PI);
    // lm_vec3_rotate(app->camera_dir, (float[3]){0.0, 1.0, 0.0}, angle,
    //                app->camera_dir);

    // lm_vec3_rotate(app->camera_right, (float[3]){0.0, 1.0, 0.0}, angle,
    //                app->camera_right);

    app->pitch += event->motion.yrel / (180 * M_PI);
    // lm_vec3_rotate(app->camera_dir, (float[3]){1.0, 0.0, 0.0}, angle,
    //                app->camera_dir);
    // lm_vec3_rotate(app->camera_up, (float[3]){1.0, 0.0, 0.0}, angle,
    //                app->camera_up);

    app->camera_dir[0] = cos(app->yaw) * cos(app->pitch);
    app->camera_dir[1] = sin(app->pitch);
    app->camera_dir[2] = sin(app->yaw) * cos(app->pitch);
    lm_vec3_cross(app->camera_up, app->camera_dir, app->camera_right);
    // lm_vec3_normalize(app->camera_up, app->camera_up);
    // lm_vec3_normalize(app->camera_dir, app->camera_dir);
    lm_vec3_normalize(app->camera_right, app->camera_right);

    float invpos[3];
    lm_vec3_scale(app->camera_pos, -1.0, invpos);
    lm_mat4_lookat(invpos, app->camera_dir, app->camera_right, app->camera_up,
                   app->view_matrix);
  } // check book;
}
