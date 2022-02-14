/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shaders.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 15:30:05 by veilo             #+#    #+#             */
/*   Updated: 2022/02/14 16:33:55 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHADERS_H
#define SHADERS_H

#include "h_opengl.h"

typedef enum e_shader_type {
  shader_type_default = 0,
  shader_type_project_z = 1,
  shader_type_normalmap = 2
} t_shader_type;

void initialize_shaders(t_app *app);

#endif