/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 15:47:28 by veilo             #+#    #+#             */
/*   Updated: 2022/03/21 15:34:53 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APP_H
#define APP_H

#include "_os_check.h"
#include "bitmap_reading.h"
#include "object_reading.h"
#ifdef SCOP_APPLE
#include <OpenGL/gl3.h>
#endif
#ifdef SCOP_UNIX
#include <GL/gl.h>
#endif
#include <SDL2/SDL.h>

#define MAX_PROGRAM_IDS 256
#define MAX_OBJECTS 512
#define MAX_SHADERS 512
#define MOVE_SPEED 3.0
#define ROTATION_SPEED 2.0

typedef struct s_app {
  SDL_Window *window;
  SDL_GLContext main_context;
  t_3d_object **objects;
  unsigned int diffuses_gl[MAX_TEXTURES];
  unsigned int normalmaps_gl[MAX_TEXTURES];
  unsigned int specularmaps_gl[MAX_TEXTURES];
  unsigned int object_count;
  unsigned int diffuse_count;
  unsigned int normalmap_count;
  unsigned int specularmap_count;
  SDL_bool is_running;
  double delta_time;
  int active_object;
  SDL_GLContext gl_context;
  unsigned int VAOs[MAX_OBJECTS];
  unsigned int default_shader_program;
  unsigned int shaders[MAX_SHADERS];
  unsigned int shader_count;
  float demo_blend_value;
  float move_vector[4];
  SDL_bool blending;
  int rotating;
  int8_t blend_dir;
  float camera_pos[3];
  float view_matrix[16];
  float projection_matrix[16];

  float light_dir[3];
} t_app;

#endif