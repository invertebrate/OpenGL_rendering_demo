/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shaders.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 15:30:05 by veilo             #+#    #+#             */
/*   Updated: 2022/04/17 15:26:29 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHADERS_H
#define SHADERS_H

#include "h_opengl.h"

typedef enum e_shader_type {
  shader_type_default = 0,
  shader_type_42_demo = 1,
  shader_type_lighting = 2,
  shader_type_skybox = 3,
  shader_type_light = 4,
  shader_type_depth = 5,
  shader_type_cube_shadow = 6,
  shader_type_debug = 7
} t_shader_type;

void initialize_shaders(t_app *app);

#endif