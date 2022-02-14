/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 15:46:50 by veilo             #+#    #+#             */
/*   Updated: 2022/02/14 14:57:43 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"
#include <math.h>

void update_matrix(float *mat) {
  static float tim = 0;
  tim += 0.005;
  memcpy(mat, (float[16]){1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
         4 * 16);
  lm_mat4_create_rotmat(mat, (float[3]){0, 1, 0}, tim * (3.14159 / 2));
  // mat[13] -= 70;
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

void load_42_demo(t_app *app) {
  t_3d_object *object;

  object_load(app, "resources/42.obj");
  texture_load(app, "resources/test.bmp");
  (void)object;
}

void load_default(t_app *app) {
  object_load(app, "resources/monster.obj");
  texture_load(app, "resources/monster01_diffuse.bmp");
}

char *parse_asset(t_app *app, char *asset) {
  char *filepath = NULL;

  if (strncmp(asset, "o:", 2) == 0) {
    filepath = asset + 2;
    if (!(object_load(app, filepath)))
      return (NULL);
    return (filepath);
  } else if (strncmp(asset, "t:", 2) == 0) {
    filepath = asset + 2;
    if (!(texture_load(app, filepath)))
      return (NULL);
    return (filepath);
  }
  return (NULL);
}

void parse_arguments(t_app *app, int argc, char **argv) {
  if (argc == 1) { // change to argc > 1
    if (strcmp(argv[1], "42_demo") == 0 || argc == 1) {
      printf("42demo\n");
      load_42_demo(app);
    } else {
      for (int i = 1; i < argc; i++) {
        parse_asset(app, argv[i]);
      }
    }
  } else {
    printf("default\n");
    load_default(app);
  }
  // "o:resources/monster.obj o:resources/monster01.obj t:resources/test.bmp"
}

void assets_init(t_app *app, int argc, char **argv) {
  // assets_read();
  parse_arguments(app, argc, argv);
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
  app->shader_count = 0;
  app->object_count = 0;
  app->texture_count = 0;
  events_init(app);
  window_init(app);
  return (app);
}

int main(int argc, char **argv) { //
  t_app *app;
  app = app_init();
  //
  load_gl_functions();
  assets_init(app, argc, argv);
  main_loop(app);
  (void)app;
  return (0);
}

// TODO:
//[...]   REFACTOR TO MORE SUSTAINABLE STRUCTURE
//[]      GL MATRICES AND TRANSFORMATIONS, PROJECTIONS
//[x]     PARSING OBJ DATA TO VAO
//[x]     OBJ READER FROM FILE
//[x]     BITMAP READER AND PARSING TO A TEXTURE
//[x]     UV MAPPING IN SHADERS
//[]      CONTROLS
//[]      MULTI ASSET RENDERING
//[]      PREVENT SEGFAULT WHEN NO ARGUMENTS

//[]      PERSPECTIVE
//[]      ROTATE AROUND MAIN SYMMETRICAL AXIS
//[]      MOVE IN 3 AXIS BOTH DIRECTIONS
//[]      TEXTURE USING KEY, CYCLE THROUGH TEXTURES/COLORS WITH SOFT TRANSITION

//[]      it is crucial that you can present
//        during defense at least the 42 logo given as resources, turning around
//        its central axis (careful, not around one of its corners), with some
//        shades of gray on the sides and a texture of poneys, kitten or unicorn
//        your choice.
