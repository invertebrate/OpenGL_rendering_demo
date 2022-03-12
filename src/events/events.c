/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 15:47:20 by veilo             #+#    #+#             */
/*   Updated: 2022/03/12 14:36:03 by veilo            ###   ########.fr       */
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
      app->move_vector[3] = 1;
    else if (event->type == SDL_KEYUP)
      app->move_vector[3] = 0;
  }
  if (event->key.keysym.sym == SDLK_s) {
    if (event->type == SDL_KEYDOWN)
      app->move_vector[3] = -1;
    else if (event->type == SDL_KEYUP)
      app->move_vector[3] = 0;
  }
  if (event->key.keysym.sym == SDLK_a) {
    if (event->type == SDL_KEYDOWN)
      app->move_vector[1] = 1;
    else if (event->type == SDL_KEYUP)
      app->move_vector[1] = 0;
  }
  if (event->key.keysym.sym == SDLK_d) {
    if (event->type == SDL_KEYDOWN)
      app->move_vector[1] = -1;
    else if (event->type == SDL_KEYUP)
      app->move_vector[1] = 0;
  }
  if (event->key.keysym.sym == SDLK_DOWN) {
    if (event->type == SDL_KEYDOWN)
      app->move_vector[2] = 1;
    else if (event->type == SDL_KEYUP)
      app->move_vector[2] = 0;
  }
  if (event->key.keysym.sym == SDLK_UP) {
    if (event->type == SDL_KEYDOWN)
      app->move_vector[2] = -1;
    else if (event->type == SDL_KEYUP)
      app->move_vector[2] = 0;
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
  if ((event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_o))
    cycle_objects(app);
  if ((event->type == SDL_KEYDOWN &&
       (event->key.keysym.sym == SDLK_j || event->key.keysym.sym == SDLK_k ||
        event->key.keysym.sym == SDLK_l)))
    rotate_light(app, event->key.keysym.sym);
}
