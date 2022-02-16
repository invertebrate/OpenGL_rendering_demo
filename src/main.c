/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 15:46:50 by veilo             #+#    #+#             */
/*   Updated: 2022/02/16 16:13:33 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"
#include <math.h>

void update_matrix(t_app *app, t_3d_object *obj) {
  static float tim = 0;
  tim += 0.005;

  lm_mat4_create_rotmat(obj->rotation, (float[3]){0, 1, 0},
                        tim * (3.14159 / 2));
  // lm_mat4_create_rotmat(app->view_matrix, (float[3]){0, 1, 0},
  //                       tim * (3.14159 / 2));
  (void)obj;
  (void)app;
}

void center_model(t_3d_object *obj) {
  obj->model_matrix[12] -= obj->center_point[0];
  obj->model_matrix[13] -= obj->center_point[1];
  obj->model_matrix[14] -= obj->center_point[2];
}

void cycle_textures(t_app *app) {
  if (app->active_object == NULL)
    return;
  if (app->texture_count == 0)
    return;
  else {
    app->active_object->texture_id++;
    app->active_object->texture_id =
        app->active_object->texture_id % app->texture_count;
  }
}

void events_handle(t_app *app, SDL_Event *event) {
  if ((event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_w)) {
    // printf("keydown\n");
    app->view_matrix[14] += 0.1;
  }
  if ((event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_s)) {
    // printf("keydown\n");
    app->view_matrix[14] -= 0.1;
  }
  if ((event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_b)) {
    // printf("keydown\n");
    app->blending = SDL_TRUE;
  }
  if ((event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_t)) {
    cycle_textures(app);
  }
}

void update_blending(t_app *app) {
  if (app->blending == SDL_TRUE) {
    app->demo_blend_value += 0.01 * app->blend_dir;
    if (app->demo_blend_value > 1.0) {
      app->demo_blend_value = 1.0;
      app->blending = SDL_FALSE;
      app->blend_dir = -1;
    }
    if (app->demo_blend_value < 0.0) {
      app->demo_blend_value = 0.0;
      app->blending = SDL_FALSE;
      app->blend_dir = 1;
    }
  }
}

void update_objects(t_app *app) {
  if (app->object_count > 0)
    for (uint i = 0; i < app->object_count; i++) {
      update_matrix(app, app->objects[i]);
    }
  update_blending(app);
}

void main_loop(t_app *app) {
  SDL_Event event;
  static float s = 0.0;
  while (app->is_running == SDL_TRUE) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT ||
          (event.type == SDL_KEYDOWN &&
           event.key.keysym.sym == SDLK_ESCAPE)) { // if crashes, check this if
        printf("sdl event quit: %u\n", event.type);
        app->is_running = SDL_FALSE;
      }
      events_handle(app, &event);
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
  (void)s;
}

void events_init(t_app *app) { (void)app; }

int load_42_demo(t_app *app) {
  t_3d_object *obj = NULL;
  GLuint texture = 0;

  if (!(obj = object_load(app, "resources/42.obj")))
    return (0);
  if (!(texture = texture_load(app, "resources/test.bmp")))
    return (0);
  if (!(texture = texture_load(app, "resources/warning.bmp")))
    return (0);
  center_model(obj);
  obj->shader = shader_type_42_demo;
  lm_mat4_translate(obj->translation, (float[3]){0, 0, -3}, obj->translation);
  obj->texture_id = 0;
  return (1);
}

int load_default(t_app *app) {
  t_3d_object *obj = NULL;

  if (!(obj = object_load(app, "resources/monster02.obj")))
    return (0);
  if (!(texture_load(app, "resources/monster01_diffuse.bmp")))
    return (0);
  obj->shader = shader_type_default;
  center_model(obj);
  lm_mat4_translate(obj->translation, (float[3]){0, 0, -1}, obj->translation);
  lm_mat4_scale(obj->scale, 0.005, 0.005, 0.005, obj->scale);
  return (1);
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

int parse_arguments(t_app *app, int argc, char **argv) {
  if (argc == 2 && strcmp(argv[1], "42_demo") == 0) {
    printf("42demo\n");
    if (!(load_42_demo(app)))
      return (0);
  } else if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      if (!(parse_asset(app, argv[i])))
        return (0);
    }
  } else {
    printf("default\n");
    if (!(load_default(app)))
      return (0);
  }
  return (1);
  // "o:resources/monster.obj o:resources/monster01.obj t:resources/test.bmp"
}

int assets_init(t_app *app, int argc, char **argv) {
  // assets_read();
  if (!(parse_arguments(app, argc, argv))) {
    printf("ERROR: Invalid arguments. Asset initialization failed.\nCorrect "
           "arguments:\n42_demo\n[no "
           "argument]\no:[object_path]\nt:[texture_path]\n");
    return (0);
  }
  vaos_create(app);
  initialize_shaders(app);
  return (1);
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
  app->blend_dir = 1;
  lm_mat4_identity(app->view_matrix);
  lm_mat4_projection(90, 90, 0.1, 100, app->projection_matrix);
  events_init(app);
  window_init(app);
  return (app);
}

int main(int argc, char **argv) { //
  t_app *app;
  app = app_init();
  //
  load_gl_functions();
  init_gl_properties();
  if (!(assets_init(app, argc, argv)))
    return (1);
  main_loop(app);
  return (0);
}

// TODO:
//[...]   REFACTOR TO MORE SUSTAINABLE STRUCTURE
//[x]      GL MATRICES AND TRANSFORMATIONS, PROJECTIONS
//[x]     PARSING OBJ DATA TO VAO
//[x]     OBJ READER FROM FILE
//[x]     BITMAP READER AND PARSING TO A TEXTURE
//[x]     UV MAPPING IN SHADERS
//[]      CONTROLS
//[]      MULTI ASSET RENDERING
//[x]      PREVENT SEGFAULT WHEN NO ARGUMENTS

//[x]      PERSPECTIVE
//[]      ROTATE AROUND MAIN SYMMETRICAL AXIS
//[]      MOVE IN 3 AXIS BOTH DIRECTIONS
//[]      TEXTURE USING KEY, CYCLE THROUGH TEXTURES/COLORS WITH SOFT
// TRANSITION

//[]      it is crucial that you can present
//        during defense at least the 42 logo given as resources, turning
//        around its central axis (careful, not around one of its corners),
//        with some shades of gray on the sides and a texture of poneys,
//        kitten or unicorn your choice.
