/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 15:46:50 by veilo             #+#    #+#             */
/*   Updated: 2022/03/22 17:27:08 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"
#include <math.h>

void app_delete(t_app *app) {
  if (!app)
    return;
  for (uint i = 0; i < app->object_count; i++) {
    obj_delete(app->objects[i]);
  }
  free(app->objects);
  app->objects = NULL;
  SDL_DestroyWindow(app->window);
  SDL_GL_DeleteContext(app->main_context);
  free(app);
  app = NULL;
}

void frame_time(t_app *app, int start) {
  static unsigned int start_counter = 0;
  unsigned int end_counter = 0;

  if (start)
    start_counter = SDL_GetPerformanceCounter();
  else {
    end_counter = SDL_GetPerformanceCounter();
    app->delta_time =
        (double)(end_counter - start_counter) / SDL_GetPerformanceFrequency();
  }
  printf("Delta time: %f\nFPS: %f\n", app->delta_time, 1 / app->delta_time);
}

void update_camera(t_app *app) {

  float mat[16];
  lm_mat4_identity(mat);
  for (int i = 0; i < 3; i++) {
    printf("cameradir: %f\n", app->camera_dir[i]);
  }
  for (int i = 0; i < 3; i++) {
    printf("cameraup: %f\n", app->camera_up[i]);
  }
  for (int i = 0; i < 3; i++) {
    printf("cameraright: %f\n", app->camera_right[i]);
  }
  for (int i = 0; i < 3; i++) {
    printf("move: %f\n", app->move_vector[i]);
  }
  lm_vec3_normalize(app->move_vector, app->move_vector);

  mat[12] -= app->move_vector[0] * MOVE_SPEED * app->delta_time;
  mat[13] -= app->move_vector[1] * MOVE_SPEED * app->delta_time;
  mat[14] -= app->move_vector[2] * MOVE_SPEED * app->delta_time;
  for (int i = 0; i < 3; i++) {
    app->camera_pos[i] -= mat[i + 12];
  }
  // lm_mat4vec4_mul(mat, app->camera_pos, app->camera_pos);
  lm_mat4_multiply(mat, app->view_matrix, app->view_matrix);
}

t_app *app_init() {
  t_app *app;

  SDL_Init(SDL_INIT_VIDEO);
  app = (t_app *)calloc(1, sizeof(t_app));
  app->objects = (t_3d_object **)calloc(MAX_OBJECTS, sizeof(t_3d_object *));
  app->is_running = SDL_TRUE;
  lm_mat4_identity(app->view_matrix);
  lm_mat4_projection(50, 50, 0.1, 100, app->projection_matrix, 1);
  memcpy(app->light_dir, (float[3]){1.0, -1.0, -1.5}, sizeof(app->light_dir));
  memcpy(app->camera_dir, (float[3]){0.0, 0.0, 1.0}, sizeof(app->camera_dir));
  memcpy(app->camera_up, (float[3]){0.0, 1.0, 0.0}, sizeof(app->camera_up));
  memcpy(app->camera_right, (float[3]){1.0, 0.0, 0.0},
         sizeof(app->camera_right));
  window_init(app);
  SDL_SetRelativeMouseMode(SDL_TRUE);
  return (app);
}

void main_loop(t_app *app) {
  SDL_Event event;
  while (app->is_running == SDL_TRUE) {
    frame_time(app, 1);
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT ||
          (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
        app->is_running = SDL_FALSE;
      }
      events_handle(app, &event);
    }
    update_objects(app);
    update_camera(app);
    render_frame(app);
    frame_time(app, 0);
  }
  printf("OpenGL version: %s\n", glGetString(GL_VERSION));
  SDL_DestroyWindow(app->window);
  SDL_Quit();
}

int main(int argc, char **argv) {
  t_app *app;

  app = app_init();
  load_gl_functions();
  init_gl_properties();
  if (!(assets_init(app, argc, argv)))
    return (1);
  main_loop(app);
  app_delete(app);
  SDL_Quit();
  return (0);
}

// TODO:
// [x]Fix matrix multiplication function
// [o]Rotation controls every direction
// [x]Smooth controls
// [x]Normal mapping
// [x]TBN in v attributes
// [x]Specular mapping
// [o]Light system
// [ ]Camera lookat (book)
// [ ]Camera look around(book)
// [x]Normal map loading and handling
// [ ]Skybox
// [ ]Scene with multiple objects
// [?]Transparency, alpha texture reading
// [?]Simple shadow mapping
// page 116 lighting