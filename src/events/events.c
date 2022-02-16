/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 15:47:20 by veilo             #+#    #+#             */
/*   Updated: 2022/02/16 21:26:42 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "events.h"
#include "app.h"
#include "asset_handling.h"

void *custom_event_dummy(void *dummy) {
  int x = 5;
  x += 10;
  (void)dummy;
  return (NULL);
}

void custom_event_handles_register(t_app *app) {
  // auto pair = std::make_pair(e_event_code_gamewon, game_won);
  // custom_events.insert(pair);
  // pair = std::make_pair(e_event_code_gamelost, game_lost);
  // custom_events.insert(pair);
  app->custom_event_handles[app->custom_event_count - 1] = custom_event_dummy;
  app->custom_event_count++;
}

void custom_event_handle(SDL_Event *event) {
  // auto handle = custom_events.find((e_event_code)event->user.code);
  // handle->second(event->user.data1, event->user.data2);
  ((void *(*)(void *))event->user.data1)(event->user.data2);
}

void custom_event_add(t_app *app, e_event_code event_code, void *data) {
  if (app->custom_event_type != ((Uint32)-1)) {
    SDL_Event event;
    SDL_memset(&event, 0, sizeof(event));
    event.type = app->custom_event_type;
    event.user.code = event_code;
    event.user.data1 = (void *)(app->custom_event_handles[event_code]);
    event.user.data2 = data;
    SDL_PushEvent(&event);
  }
}

void dummy_event_add(t_app *app) {
  custom_event_add(app, e_event_code_dummy, NULL);
}

// void events_init(t_app *app) {
// app->custom_event_type = SDL_RegisterEvents(1);
// app->custom_event_count = 0;
// app->custom_event_handles =
//     (void *(**)(void *))malloc(sizeof(void *(*)(void *)) *
//     MAX_CUSTOM_EVENTS);
// custom_event_handles_register(app);(void)app; }

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