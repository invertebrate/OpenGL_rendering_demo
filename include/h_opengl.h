/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   h_opengl.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 15:38:38 by veilo             #+#    #+#             */
/*   Updated: 2022/03/09 17:32:29 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef H_OPENGL_H
#define H_OPENGL_H

#include "app.h"
#include "shaders.h"
#ifdef SCOP_APPLE
#include <OpenGL/gl3.h>
#endif
#ifdef SCOP_UNIX
#include <GL/gl.h>
#include <GL/glx.h>
#endif
#include <SDL2/SDL.h>

#define VERTEX_STRIDE_PUVN 4 * (3 + 2 + 3)
#define CLEAR_R 0.2
#define CLEAR_G 0.3
#define CLEAR_B 0.3
#define CLEAR_A 1.0

#ifdef SCOP_UNIX
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
PFNGLUNIFORM3FPROC glUniform3f;
PFNGLUNIFORM1FPROC glUniform1f;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
#endif
void load_gl_functions();
SDL_bool vaos_create(t_app *app);
void gl_temp(t_app *app);
void render_frame(t_app *app);
void init_gl_properties();

#endif