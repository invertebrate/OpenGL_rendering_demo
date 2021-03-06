/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 15:46:50 by veilo             #+#    #+#             */
/*   Updated: 2022/05/20 18:28:53 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include <math.h>

void app_delete(t_app *app) {
  if (!app)
    return;
  for (uint i = 0; i < app->object_count; i++) {
    obj_delete(app->objects[i]);
  }
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
    if (APP_LIMIT_FPS60 && app->delta_time < 1.0 / 60.0) {

      if ((1.0 / 60.0 - app->delta_time) * 1000 > 0)
        SDL_Delay((1.0 / 60.0 - app->delta_time) * 1000);
      app->delta_time = 1.0 / 60.0;
    }
  }
  static float s = 0.0;
  s += app->delta_time;
  if (s - 1 > 0) {
    printf("FPS: %f\n", 1 / app->delta_time);
    s = 0.0;
  }
}

t_app *app_init() {
  t_app *app;

  SDL_Init(SDL_INIT_VIDEO);
  app = (t_app *)calloc(1, sizeof(t_app));
  app->is_running = SDL_TRUE;
  lm_mat4_identity(app->view_matrix);
  lm_mat4_projection(atan(W_ASPECT * 50) * 50, 50, NEAR_PLANE, FAR_PLANE,
                     app->persp_proj, 1);
  lm_mat4_ortho(FAR_PLANE, NEAR_PLANE, 15, -15, -15, 15, app->ortho_proj, 0);
  memcpy(app->camera_dir, (float[3]){0.0, 0.0, 1.0}, sizeof(app->camera_dir));
  memcpy(app->camera_up, (float[3]){0.0, 1.0, 0.0}, sizeof(app->camera_up));
  memcpy(app->camera_right, (float[3]){1.0, 0.0, 0.0},
         sizeof(app->camera_right));
  memcpy(app->camera_pos, (float[3]){0.0, 11.0, 22.0}, sizeof(float[3]));
  lm_vec3_rotate(app->camera_pos, VECTOR_Y, -(M_PI / 3.0), app->camera_pos);
  lm_vec3_normalize(app->camera_dir, app->camera_dir);
  memcpy(
      app->ambient_light,
      (float[3]){AMBIENT_LIGHT_VALUE, AMBIENT_LIGHT_VALUE, AMBIENT_LIGHT_VALUE},
      sizeof(app->ambient_light));
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
    draw_scene(app);
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
  generate_shadowmap(app);
  generate_shadow_cubemap(app);
  generate_depth_cubemap(app);
  main_loop(app);
  app_delete(app);
  SDL_Quit();
  return (0);
}
