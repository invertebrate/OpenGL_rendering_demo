/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_reading.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/23 16:52:35 by veilo             #+#    #+#             */
/*   Updated: 2022/02/16 20:09:43 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECT_READING_H
#define OBJECT_READING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERTEX_PREFIX "v "
#define UV_PREFIX "vt "
#define NORMAL_PREFIX "vn "
#define FACE_PREFIX "f "
#define OBJ_SUCCESS 1
#define OBJ_FAILURE 0

typedef struct s_float3 {
  float x;
  float y;
  float z;
} t_float3;

typedef struct s_float2 {
  float u;
  float v;
} t_float2;

typedef struct s_uint3 {
  unsigned int x;
  unsigned int y;
  unsigned int z;
} t_uint3;

/*
** Supports up to 8 sided convex n-gons.
*/
typedef struct s_face {
  t_uint3 vertices[8];
  unsigned int vertex_count;
} t_face;

typedef struct s_3d_object {
  unsigned int object_id;
  unsigned int texture_id;
  t_uint3 *triangles;
  float *vertex_data_array;
  unsigned int vertex_count;
  unsigned int triangle_count;
  float model_matrix[16];
  float translation[16];
  float rotation[16];
  float scale[16];
  float center_point[3];
  unsigned int shader;
  float scale_factor;
} t_3d_object;

t_3d_object *obj_read_from_file(char *filename);

#endif