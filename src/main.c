/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 15:46:50 by veilo             #+#    #+#             */
/*   Updated: 2022/01/16 16:59:45 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

PFNGLUSEPROGRAMPROC glUseProgram;

void main_loop(t_app *app) {
  // load objects
  // load vertices and textures to VRAM and keep them there
  SDL_Event event;
  while (app->is_running == SDL_TRUE) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT)
        app->is_running = SDL_FALSE;
    }
    glClearColor(1.0f, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(app->window);
    // poll events
    // update matrices/meshes
    // render frame
  }
  printf("OpenGL version: %s\n", glGetString(GL_VERSION));
  SDL_DestroyWindow(app->window);
  SDL_Quit();
  (void)app;
  (void)event;
}

void window_init(t_app *app) {
  SDL_Window *window;

  window =
      SDL_CreateWindow("SCOP", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       500, 500, SDL_WINDOW_OPENGL);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GLContext context = SDL_GL_CreateContext(window);
  app->main_context = context;
  app->window = window;
}

t_app *app_init() {
  t_app *app;

  SDL_Init(SDL_INIT_VIDEO);
  app = (t_app *)malloc(sizeof(t_app));
  // app->custom_event_type = SDL_RegisterEvents(1);
  // app->custom_event_count = 0;
  // app->custom_event_handles =
  //     (void *(**)(void *))malloc(sizeof(void *(*)(void *)) *
  //     MAX_CUSTOM_EVENTS);
  // custom_event_handles_register(app);
  app->is_running = SDL_TRUE;
  window_init(app);
  return (app);
}

int main() {
  t_app *app;
  app = app_init();
  // void(*)() glXGetProcAddress(	const GLubyte * procName);
  // In an initialization routine
  // glUseProgram =
  //     (PFNGLUSEPROGRAMPROC)glXGetProcAddress((unsigned char
  //     *)"glUseProgram");
  // const GLubyte *procName = "glUseProgram";
  // void(*useprog)() = (PFNGLUSEPROGRAMPROC)glXGetProcAddress(procName);
  // useprog();
  // glGenBuffers();
  // create window with sdl2 //youtube
  // create renderer

  TTF_Init();
  main_loop(app);
  return (0);
}
