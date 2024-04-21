/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   h_opengl.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 15:38:38 by veilo             #+#    #+#             */
/*   Updated: 2022/05/06 16:19:05 by veilo            ###   ########.fr       */
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
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLUNIFORM4FPROC glUniform4f;
extern PFNGLUNIFORM3FPROC glUniform3f;
extern PFNGLUNIFORM1FPROC glUniform1f;
extern PFNGLUNIFORM1IPROC glUniform1i;
extern PFNGLUNIFORM3FVPROC glUniform3fv;
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
extern PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
extern PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
extern PFNGLFRAMEBUFFERTEXTUREPROC glFramebufferTexture;
extern PFNGLDRAWBUFFERSPROC glDrawBuffers;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;

#endif
void load_gl_functions();
SDL_bool vaos_create(t_app *app);
void render_frame(t_app *app);
void init_gl_properties();

#endif