/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 15:47:34 by veilo             #+#    #+#             */
/*   Updated: 2022/01/23 17:41:36 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCOP_H
#define SCOP_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>

#include <GL/gl.h>
// #include <GL/glext.h> //
#include <GL/glu.h>
#include <GL/glx.h>  //

#include "events.h"
#include "h_opengl.h"
#include "object_reading.h"
#include "rendering.h"
#include "shaders.h"
#include "vector2.h"
#include "window.h"

typedef struct s_vertex {
  float x;
  float y;
  float z;
  float w;
} t_vertex;

typedef struct s_texture {
  int i;
} t_texture;

typedef struct s_mat4 {
  int i;
} t_mat4;

typedef struct s_object3D {
  t_vertex* vertices;
  t_mat4 pos;
  t_mat4 rot;
  t_texture tex;
} t_object3D;

#endif