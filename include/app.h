/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 15:47:28 by veilo             #+#    #+#             */
/*   Updated: 2022/02/16 19:47:03 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APP_H
#define APP_H

#include "bitmap_reading.h"
#include "object_reading.h"
#include <GL/gl.h>
#include <SDL2/SDL.h>

#define MAX_PROGRAM_IDS 256
#define MAX_OBJECTS 512
#define MAX_SHADERS 512

typedef struct s_app {
  SDL_Window *window;
  SDL_GLContext main_context;
  t_3d_object **objects;
  t_texture_data **texture_data;
  GLuint *textures_gl;
  uint object_count;
  uint texture_count;
  uint32_t custom_event_type;
  void *(**custom_event_handles)(void *);
  uint32_t custom_event_count;
  SDL_bool is_running;

  int active_object;

  SDL_GLContext gl_context;

  GLuint VAOs[MAX_OBJECTS];
  uint default_shader_program;
  uint shaders[MAX_SHADERS];
  uint shader_count;
  float demo_blend_value;
  SDL_bool blending;
  int8_t blend_dir;
  float view_matrix[16];
  float projection_matrix[16];

} t_app;

#endif