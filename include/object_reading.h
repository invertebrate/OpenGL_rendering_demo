/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_reading.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/23 16:52:35 by veilo             #+#    #+#             */
/*   Updated: 2022/01/23 18:27:07 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECT_READING_H
#define OBJECT_READING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_float3 {
  float x;
  float y;
  float z;
} t_float3;

typedef struct s_uint3 {
  uint x;
  uint y;
  uint z;
} t_uint3;

typedef struct s_3d_object {
  t_float3* vtc;
  t_float3* normals;
  t_float3* uvs;
  t_uint3* triangles;
} t_3d_object;

t_3d_object* obj_read_from_file(char* filename);

#endif