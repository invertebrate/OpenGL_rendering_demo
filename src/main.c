/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 15:46:50 by veilo             #+#    #+#             */
/*   Updated: 2022/01/23 18:50:57 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void main_loop(t_app *app) {
  SDL_Event event;

  while (app->is_running == SDL_TRUE) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT ||
          (event.type == SDL_KEYDOWN &&
           event.key.keysym.sym == SDLK_ESCAPE)) { // if crashes, check this if
        printf("sdl event quit: %u\n", event.type);
        app->is_running = SDL_FALSE;
      }
    }
    // poll events
    // update matrices/meshes
    render_frame(app);
  }
  printf("sdl event crash: %u\n", event.type);
  printf("OpenGL version: %s\n", glGetString(GL_VERSION));
  SDL_DestroyWindow(app->window);
  SDL_Quit();
  (void)app;
  (void)event;
}

void window_init(t_app *app) {
  SDL_Window *window;

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                      SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
  window =
      SDL_CreateWindow("SCOP", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       1000, 1000, SDL_WINDOW_OPENGL);
  app->main_context = SDL_GL_CreateContext(window);
  app->window = window;
  if (SDL_GL_SetSwapInterval(1) < 0) {
    printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
  }
}

void events_init(t_app *app) { (void)app; }

void assets_init(t_app *app) {
  // assets_read();
  objects_gl_create(app);
  shaders_init(app);
  // shaders_load();
  // shaders_compile();
}

t_app *app_init() {
  t_app *app;

  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();
  app = (t_app *)malloc(sizeof(t_app));
  // app->custom_event_type = SDL_RegisterEvents(1);
  // app->custom_event_count = 0;
  // app->custom_event_handles =
  //     (void *(**)(void *))malloc(sizeof(void *(*)(void *)) *
  //     MAX_CUSTOM_EVENTS);
  // custom_event_handles_register(app);
  app->is_running = SDL_TRUE;
  app->program_id_count = 0;
  events_init(app);
  window_init(app);
  return (app);
}

int main() {
  t_app *app;
  obj_read_from_file("resources/42.obj");
  app = app_init();
  load_gl_functions();
  assets_init(app);
  main_loop(app);
  return (0);
}

// TODO: REFACTOR TO MORE SUSTAINABLE STRUCTURE
//       GL MATRICES AND TRANSFORMATIONS, PROJECTIONS
//       PARSING OBJ DATA TO VAO
//       OBJ READER FROM FILE
//       BITMAP READER AND PARSING TO A TEXTURE
//       UV MAPPING IN SHADERS
//       CONTROLS
