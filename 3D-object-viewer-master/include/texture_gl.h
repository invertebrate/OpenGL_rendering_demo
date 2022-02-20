/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_gl.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 17:39:59 by veilo             #+#    #+#             */
/*   Updated: 2022/02/20 16:13:37 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURE_GL_H
#define TEXTURE_GL_H

#include "bitmap_reading.h"
#include "h_opengl.h"

unsigned int create_texture(t_texture_data *data);

#endif