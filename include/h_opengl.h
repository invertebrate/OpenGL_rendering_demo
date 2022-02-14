/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   h_opengl.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 15:38:38 by veilo             #+#    #+#             */
/*   Updated: 2022/02/14 16:37:02 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef H_OPENGL_H
#define H_OPENGL_H

#include "app.h"
#include <GL/gl.h>
// #include <GL/glext.h> //
#include "shaders.h"
#include <GL/glu.h>
#include <GL/glx.h>
#include <SDL2/SDL.h>

#define VERTEX_STRIDE_PUVN 4 * (3 + 2 + 3)
#define CLEAR_R 0.2
#define CLEAR_G 0.3
#define CLEAR_B 0.3
#define CLEAR_A 1.0

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
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLUNIFORM4FPROC glUniform4f;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
// glUniformf: the function expects a float as its value.
// glUniformi: the function expects an int as its value.
// glUniformui: the function expects an unsigned int as its value.
// glUniform3f: the function expects 3 floats as its value.
// glUniformfv: the function expects a float vector/array as its value.

void load_gl_functions();
SDL_bool vaos_create(t_app *app);
void gl_temp(t_app *app);
void render_frame(t_app *app);
void init_gl_properties();

#endif