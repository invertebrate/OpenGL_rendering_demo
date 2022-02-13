/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 15:46:50 by veilo             #+#    #+#             */
/*   Updated: 2022/02/13 15:13:50 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"
#include <math.h>

void update_matrix(float *mat) {
  static float tim = 0;
  tim += 0.01;
  memcpy(mat, (float[16]){1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
         4 * 16);
  lm_mat4_create_rotmat(mat, (float[3]){0, 1, 0}, tim * (3.14159 / 2));
  mat[13] -= 1;
  (void)mat;
  (void)tim;
}

void update_objects(t_app *app) { update_matrix(app->matrix); }

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
    update_objects(app);
    render_frame(app);
  }
  printf("OpenGL version: %s\n", glGetString(GL_VERSION));
  SDL_DestroyWindow(app->window);
  SDL_Quit();
  (void)app;
  (void)event;
}

void events_init(t_app *app) { (void)app; }

void assets_init(t_app *app) {
  // assets_read();
  objects_load(app);
  textures_load(app);
  vaos_create(app);
  shaders_init(app);
  // shaders_load();
  // shaders_compile();
}

t_app *app_init() {
  t_app *app;

  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();
  app = (t_app *)calloc(1, sizeof(t_app));
  app->objects = (t_3d_object **)calloc(MAX_OBJECTS, sizeof(t_3d_object *));
  app->texture_data =
      (t_texture_data **)calloc(MAX_TEXTURES, sizeof(t_texture_data *));
  for (uint i = 0; i < MAX_TEXTURES; i++) {
    app->texture_data[i] = (t_texture_data *)calloc(1, sizeof(t_texture_data));
  }
  app->textures_gl = (GLuint *)calloc(MAX_TEXTURES, sizeof(GLuint));
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
  app = app_init();
  load_gl_functions();
  assets_init(app);
  main_loop(app);
  (void)app;
  return (0);
}

// TODO:
//[...]     REFACTOR TO MORE SUSTAINABLE STRUCTURE
//[]     GL MATRICES AND TRANSFORMATIONS, PROJECTIONS
//[]     PARSING OBJ DATA TO VAO
//[x]     OBJ READER FROM FILE
//[x]     BITMAP READER AND PARSING TO A TEXTURE
//[x]     UV MAPPING IN SHADERS
//[]     CONTROLS
