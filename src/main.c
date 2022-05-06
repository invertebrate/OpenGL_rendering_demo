/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 15:46:50 by veilo             #+#    #+#             */
/*   Updated: 2022/05/06 16:27:03 by veilo            ###   ########.fr       */
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
  // printf("FPS: %f\n", 1 / app->delta_time);
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
  memcpy(app->light_dir, (float[3]){1.0, -1.0, -1.5}, sizeof(app->light_dir));
  memcpy(app->camera_dir, (float[3]){0.0, 0.0, 1.0}, sizeof(app->camera_dir));
  memcpy(app->camera_up, (float[3]){0.0, 1.0, 0.0}, sizeof(app->camera_up));
  memcpy(app->camera_right, (float[3]){1.0, 0.0, 0.0},
         sizeof(app->camera_right));

  memcpy(app->camera_pos, (float[3]){0.0, 1.5, 10.0}, sizeof(float[3]));
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
    // rotate_light_obj(app, 0, 1); // test only

    update_objects(app);
    update_camera(app);
    // render_frame(app);
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

// TODO:
// change gluniform3f to 3fv
//[!]Point light shadow -> rendering refactor
//[!]Clipping objects shadow artifact fixable by ambient occlusion
//[!]SSAO if not too work intensive

//[ ]Cleanup depthmap rendering//try camera view<- light view
//[ ]Refactor data passing to shaders
//[ ]Clean up shaders

// [x]Simultaneous multidirectional movement
// [ ]Skybox
// [ ]Scene with multiple objects
// [ ]Light objects
// [ ]Camera look at
// [?]Transparency, alpha texture reading
// [?]Simple shadow mapping
// [ ]"Destructors" for objects
// [x]Simple ambient light
// [x]Fix matrix multiplication function
// [x]Smooth controls
// [x]Normal mapping
// [x]TBN in v attributes
// [x]Specular mapping
// [x]Camera look around
// [x]Normal map loading and handling
// [o]Light system
// [o]Rotation controls every direction
// page 116 lighting
// CRASH: "./scop a" crashes