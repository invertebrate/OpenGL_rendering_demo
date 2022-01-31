/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_reading.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/23 16:52:35 by veilo             #+#    #+#             */
/*   Updated: 2022/01/31 16:49:40 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECT_READING_H
#define OBJECT_READING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERTEX_PREFIX "\nv "
#define UV_PREFIX "\nvt "
#define NORMAL_PREFIX "\nvn "
#define FACE_PREFIX "\nf "
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
  uint x;
  uint y;
  uint z;
} t_uint3;

/*
** Supports up to 8 sided convex n-gons.
*/
typedef struct s_face {
  t_uint3 vertices[8];
  uint vertex_count;
} t_face;

typedef struct s_3d_object {
  uint *triangles;
  float *vertex_data_array;
  uint vertex_count;
} t_3d_object;

t_3d_object *obj_read_from_file(char *filename);

#endif