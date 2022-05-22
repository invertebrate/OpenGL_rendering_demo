/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 15:47:28 by veilo             #+#    #+#             */
/*   Updated: 2022/05/20 15:34:11 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APP_H
#define APP_H

#include "_os_check.h"
#include "bitmap_reading.h"
#include "object_reading.h"
#ifdef CHECK_APPLE
#include <OpenGL/gl3.h>
#endif
#ifdef CHECK_UNIX
#include <GL/gl.h>
#endif
#include <SDL2/SDL.h>

#define MAX_PROGRAM_IDS 256
#define MAX_OBJECTS 512
#define MAX_LIGHTS 12
#define MAX_SHADERS 512
#define MOVE_SPEED 8.0
#define ROTATION_SPEED 1.0
#define APP_LIMIT_FPS60 0
#define AMBIENT_LIGHT_VALUE 0.48
#define SHADOW_WIDTH 1024
#define SHADOW_HEIGHT 1024
#define NEAR_PLANE 0.2
#define FAR_PLANE 100

typedef struct s_dir_light t_dir_light;
typedef struct s_point_light t_point_light;

typedef struct s_app {
  SDL_Window *window;
  unsigned int w_width;
  unsigned int w_height;
  SDL_GLContext main_context;
  t_3d_object *objects[MAX_OBJECTS];
  t_dir_light *d_lights[MAX_LIGHTS];
  t_point_light *p_lights[MAX_LIGHTS];
  int d_light_count;
  int p_light_count;
  float light_view[16];

  unsigned int shadow_map[MAX_LIGHTS];
  unsigned int depth_map_FBO;
  unsigned int cube_depth_map_FBO;
  unsigned int cube_shadow_map[MAX_LIGHTS];
  unsigned int cube_depth_map;

  int shadow_pass;

  unsigned int diffuses_gl[MAX_TEXTURES];
  unsigned int normalmaps_gl[MAX_TEXTURES];
  unsigned int specularmaps_gl[MAX_TEXTURES];
  unsigned int object_count;
  unsigned int terrain_obj_count;
  unsigned int light_count;
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
  float yaw;
  float pitch;
  float camera_pos[3];
  float camera_dir[3];
  float camera_right[3];
  float camera_up[3];
  float view_matrix[16];
  float persp_proj[16];
  float ortho_proj[16];
  t_3d_object *skybox_obj;
  unsigned int skybox_vao;
  float ambient_light[3];
  float light_dir[3];

} t_app;

#endif