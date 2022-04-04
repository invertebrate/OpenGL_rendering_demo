/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_gl.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 17:39:59 by veilo             #+#    #+#             */
/*   Updated: 2022/04/04 15:58:48 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURE_GL_H
#define TEXTURE_GL_H

#include "bitmap_reading.h"
#include "h_opengl.h"

typedef enum e_texture_type t_texture_type;
unsigned int create_texture(t_texture_data *data, t_texture_type type);

#endif