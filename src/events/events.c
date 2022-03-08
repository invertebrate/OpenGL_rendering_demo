/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 15:47:20 by veilo             #+#    #+#             */
/*   Updated: 2022/03/08 15:06:27 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "events.h"
#include "app.h"
#include "asset_handling.h"

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
}