/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_gl.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 15:38:38 by veilo             #+#    #+#             */
/*   Updated: 2022/01/17 16:43:34 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INIT_GL_H
#define INIT_GL_H

#include "app.h"
#include <GL/gl.h>
// #include <GL/glext.h> //
#include <GL/glu.h>
#include <GL/glx.h> //
#include <SDL2/SDL.h>

PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;

void load_gl(t_app *app);
SDL_bool init_gl(t_app *app);
void gl_temp(t_app *app);
void render(t_app *app);

#endif