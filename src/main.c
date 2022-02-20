/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 15:46:50 by veilo             #+#    #+#             */
/*   Updated: 2022/02/20 16:43:44 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"
#include <math.h>

void main_loop(t_app *app) {
  SDL_Event event;
  while (app->is_running == SDL_TRUE) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT ||
          (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
        app->is_running = SDL_FALSE;
      }
      events_handle(app, &event);
    }
    update_objects(app);
    render_frame(app);
  }
  printf("OpenGL version: %s\n", glGetString(GL_VERSION));
  SDL_DestroyWindow(app->window);
  SDL_Quit();
}

t_app *app_init() {
  t_app *app;

  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();
  app = (t_app *)calloc(1, sizeof(t_app));
  app->objects = (t_3d_object **)calloc(MAX_OBJECTS, sizeof(t_3d_object *));
  app->texture_data =
      (t_texture_data **)calloc(MAX_TEXTURES, sizeof(t_texture_data *));
  for (unsigned int i = 0; i < MAX_TEXTURES; i++) {
    app->texture_data[i] = (t_texture_data *)calloc(1, sizeof(t_texture_data));
  }
  app->textures_gl = (unsigned int *)calloc(MAX_TEXTURES, sizeof(unsigned int));
  app->is_running = SDL_TRUE;
  app->shader_count = 0;
  app->object_count = 0;
  app->texture_count = 0;
  app->blend_dir = 1;
  lm_mat4_identity(app->view_matrix);
  lm_mat4_projection(90, 90, 0.1, 100, app->projection_matrix);
  window_init(app);
  return (app);
}

int main(int argc, char **argv) {
  t_app *app;

  app = app_init();
  load_gl_functions();
  init_gl_properties();
  if (!(assets_init(app, argc, argv)))
    return (1);
  main_loop(app);
  return (0);
}
