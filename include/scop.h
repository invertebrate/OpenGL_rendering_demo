/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 15:47:34 by veilo             #+#    #+#             */
/*   Updated: 2022/02/20 15:38:53 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCOP_H
#define SCOP_H

#include "_os_check.h"
#include "asset_handling.h"
#include "bitmap_reading.h"
#include "events.h"
#include "h_opengl.h"
#include "lm_matrix.h"
#include "object_reading.h"
#include "rendering.h"
#include "shaders.h"
#include "texture_gl.h"
#include "window.h"
#ifdef SCOP_APPLE
#include <OpenGL.gl3.h>
#endif
#ifdef SCOP_UNIX
#include <GL/gl.h>
#include <GL/glx.h>
#endif
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>

#endif