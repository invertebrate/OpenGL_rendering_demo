/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   h_opengl.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 15:38:38 by veilo             #+#    #+#             */
/*   Updated: 2022/05/02 17:33:00 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef H_OPENGL_H
#define H_OPENGL_H

#include "app.h"
#include "shaders.h"
#ifdef CHECK_APPLE
#include <OpenGL/gl3.h>
#endif
#ifdef CHECK_UNIX
#include <GL/gl.h>
#include <GL/glx.h>
#endif
#include <SDL2/SDL.h>

#define TU_DIFFUSE_GL GL_TEXTURE0
#define TU_NORMALMAP_GL GL_TEXTURE1
#define TU_SPECULARMAP_GL GL_TEXTURE2
#define TU_SHADOWMAP_GL GL_TEXTURE3
#define TU_SHADOW_CUBEMAP_GL GL_TEXTURE4
#define TU_COUNT 16
#define VERTEX_STRIDE_PUVNTB 4 * (3 + 2 + 3 + 3 + 3)
#define CLEAR_R 0.2
#define CLEAR_G 0.3
#define CLEAR_B 0.3
#define CLEAR_A 1.0

#ifdef CHECK_UNIX
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
PFNGLUNIFORM1IPROC glUniform1i;
PFNGLUNIFORM3FVPROC glUniform3fv;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
PFNGLFRAMEBUFFERTEXTUREPROC glFramebufferTexture;
PFNGLDRAWBUFFERSPROC glDrawBuffers;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;

#endif
void load_gl_functions();
SDL_bool vaos_create(t_app *app);
void render_frame(t_app *app);
void init_gl_properties();

#endif