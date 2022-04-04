/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 15:47:20 by veilo             #+#    #+#             */
/*   Updated: 2022/04/04 16:03:53 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "events.h"
#include "app.h"
#include "assets.h"

void rotate_light(t_app *app, SDL_Keycode code) {
  if (code == SDLK_j) {
    lm_vec3_rotate(app->light_dir, (float[3]){1, 0, 0},
                   app->delta_time * ROTATION_SPEED * 2, app->light_dir);
  } else if (code == SDLK_k) {
    lm_vec3_rotate(app->light_dir, (float[3]){0, 1, 0},
                   app->delta_time * ROTATION_SPEED * 2, app->light_dir);
  } else if (code == SDLK_l) {
    lm_vec3_rotate(app->light_dir, (float[3]){0, 0, 1},
                   app->delta_time * ROTATION_SPEED * 2, app->light_dir);
  }
  app->light_dir[3] = 1;
}
void get_move_state(int *move_state) {
  const unsigned char *kb_state = SDL_GetKeyboardState(NULL);

  if (kb_state[SDL_SCANCODE_W])
    move_state[4] = -1;
  else
    move_state[4] = 0;
  if (kb_state[SDL_SCANCODE_S])
    move_state[5] = 1;
  else
    move_state[5] = 0;
  if (kb_state[SDL_SCANCODE_UP])
    move_state[2] = 1;
  else
    move_state[2] = 0;
  if (kb_state[SDL_SCANCODE_DOWN])
    move_state[3] = -1;
  else
    move_state[3] = 0;
  if (kb_state[SDL_SCANCODE_D])
    move_state[0] = 1;
  else
    move_state[0] = 0;
  if (kb_state[SDL_SCANCODE_A])
    move_state[1] = -1;
  else
    move_state[1] = 0;
}

void handle_player_movement(t_app *app) {
  float vec[3] = {0.0, 0.0, 0.0};
  float temp_dir[3] = {0.0, 0.0, 0.0};
  int move_state[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

  get_move_state(move_state);
  memcpy(temp_dir, app->camera_dir, sizeof(temp_dir));
  temp_dir[1] = 0;
  lm_vec3_scale(app->move_vector, 0.0, app->move_vector);
  lm_vec3_scale(temp_dir, move_state[4] + move_state[5], vec);
  lm_vec3_add(app->move_vector, vec, app->move_vector);
  lm_vec3_scale(app->camera_right, move_state[0] + move_state[1], vec);
  lm_vec3_add(app->move_vector, vec, app->move_vector);
  lm_vec3_scale(app->camera_up, move_state[2] + move_state[3], vec);
  lm_vec3_add(app->move_vector, vec, app->move_vector);
}

void handle_mouse_look(t_app *app, SDL_Event *event) {
  if (event->type == SDL_MOUSEMOTION) {
    float rotmatx[16];
    float rotmaty[16];
    float rotmat[16];

    app->yaw += -event->motion.xrel / (180 * M_PI);
    app->pitch += -event->motion.yrel / (180 * M_PI);
    if (app->pitch > (2 * M_PI) / 4)
      app->pitch = (2 * M_PI) / 4;
    if (app->pitch < -(2 * M_PI) / 4)
      app->pitch = -(2 * M_PI) / 4;
    lm_mat4_create_rotmat(rotmatx, (float[3]){0.0, 1.0, 0.0}, app->yaw);
    lm_mat4_create_rotmat(rotmaty, (float[3]){1.0, 0.0, 0.0}, app->pitch);
    lm_mat4_multiply(rotmatx, rotmaty, rotmat);
    lm_mat4vec4_mul(rotmat, VECTOR_Z, app->camera_dir);
    lm_mat4vec4_mul(rotmat, VECTOR_Y, app->camera_up);
    lm_vec3_cross(app->camera_up, app->camera_dir, app->camera_right);
    lm_vec3_normalize(app->camera_right, app->camera_right);
  }
}

void events_handle(t_app *app, SDL_Event *event) {
  handle_player_movement(app);
  handle_mouse_look(app, event);

  if (event->key.keysym.sym == SDLK_q) {
    if (event->type == SDL_KEYDOWN)
      app->rotating = -1;
    else if (event->type == SDL_KEYUP)
      app->rotating = 0;
  }
  if (event->key.keysym.sym == SDLK_e) {
    if (event->type == SDL_KEYDOWN)
      app->rotating = 1;
    else if (event->type == SDL_KEYUP)
      app->rotating = 0;
  }
  if ((event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_b))
    app->blending = SDL_TRUE;
  if ((event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_t))
    cycle_textures(app);
  if ((event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_n))
    cycle_normalmaps(app);
  if ((event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_o))
    cycle_objects(app);
  if ((event->type == SDL_KEYDOWN &&
       (event->key.keysym.sym == SDLK_j || event->key.keysym.sym == SDLK_k ||
        event->key.keysym.sym == SDLK_l)))
    rotate_light(app, event->key.keysym.sym);
}
