/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 15:47:20 by veilo             #+#    #+#             */
/*   Updated: 2022/02/20 16:36:52 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "events.h"
#include "app.h"
#include "asset_handling.h"

void events_handle(t_app *app, SDL_Event *event) {
  if ((event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_w))
    app->view_matrix[14] += 0.1;
  if ((event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_s))
    app->view_matrix[14] -= 0.1;
  if ((event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_a))
    app->view_matrix[12] += 0.1;
  if ((event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_d))
    app->view_matrix[12] -= 0.1;
  if ((event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_DOWN))
    app->view_matrix[13] += 0.1;
  if ((event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_UP))
    app->view_matrix[13] -= 0.1;
  if ((event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_b))
    app->blending = SDL_TRUE;
  if ((event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_t))
    cycle_textures(app);
  if ((event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_o))
    cycle_objects(app);
  if ((event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_r)) {
    if (app->rotating)
      app->rotating = SDL_FALSE;
    else
      app->rotating = SDL_TRUE;
  }
}