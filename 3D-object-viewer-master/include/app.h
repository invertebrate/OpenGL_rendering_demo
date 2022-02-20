/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 15:47:28 by veilo             #+#    #+#             */
/*   Updated: 2022/02/20 16:13:26 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APP_H
#define APP_H

#include "bitmap_reading.h"
#include "object_reading.h"
#include <OpenGL/gl3.h>
#include <SDL2/SDL.h>

#define MAX_PROGRAM_IDS 256
#define MAX_OBJECTS 512
#define MAX_SHADERS 512

typedef struct s_app {
  SDL_Window *window;
  SDL_GLContext main_context;
  t_3d_object **objects;
  t_texture_data **texture_data;
  unsigned int *textures_gl;
  unsigned int object_count;
  unsigned int texture_count;
  unsigned int custom_event_type;
  void *(**custom_event_handles)(void *);
  unsigned int custom_event_count;
  SDL_bool is_running;
  int active_object;
  SDL_GLContext gl_context;
  unsigned int VAOs[MAX_OBJECTS];
  unsigned int default_shader_program;
  unsigned int shaders[MAX_SHADERS];
  unsigned int shader_count;
  float demo_blend_value;
  SDL_bool blending;
  SDL_bool rotating;
  int8_t blend_dir;
  float view_matrix[16];
  float projection_matrix[16];

} t_app;

#endif