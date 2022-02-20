/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   h_opengl.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 15:38:38 by veilo             #+#    #+#             */
/*   Updated: 2022/02/20 15:31:14 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef H_OPENGL_H
#define H_OPENGL_H

#include "app.h"
#include "shaders.h"
#include <OpenGL/gl3.h>
//#include <OpenGL/glu.h>
#include <SDL2/SDL.h>

#define VERTEX_STRIDE_PUVN 4 * (3 + 2 + 3)
#define CLEAR_R 0.2
#define CLEAR_G 0.3
#define CLEAR_B 0.3
#define CLEAR_A 1.0

void load_gl_functions();
SDL_bool vaos_create(t_app *app);
void gl_temp(t_app *app);
void render_frame(t_app *app);
void init_gl_properties();

#endif