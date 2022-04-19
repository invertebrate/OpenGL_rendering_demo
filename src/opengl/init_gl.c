/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_gl.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 15:36:42 by veilo             #+#    #+#             */
/*   Updated: 2022/04/19 15:48:19 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "h_opengl.h"

#ifdef CHECK_UNIX
void load_gl_functions() {
  glCreateProgram = (PFNGLCREATEPROGRAMPROC)glXGetProcAddress(
      (unsigned char *)"glCreateProgram");
  glUseProgram =
      (PFNGLUSEPROGRAMPROC)glXGetProcAddress((unsigned char *)"glUseProgram");
  glCreateShader = (PFNGLCREATESHADERPROC)glXGetProcAddress(
      (unsigned char *)"glCreateShader");
  glShaderSource = (PFNGLSHADERSOURCEPROC)glXGetProcAddress(
      (unsigned char *)"glShaderSource");
  glCompileShader = (PFNGLCOMPILESHADERPROC)glXGetProcAddress(
      (unsigned char *)"glCompileShader");
  glGetShaderiv =
      (PFNGLGETSHADERIVPROC)glXGetProcAddress((unsigned char *)"glGetShaderiv");
  glGetProgramiv = (PFNGLGETPROGRAMIVPROC)glXGetProcAddress(
      (unsigned char *)"glGetProgramiv");
  glAttachShader = (PFNGLATTACHSHADERPROC)glXGetProcAddress(
      (unsigned char *)"glAttachShader");
  glLinkProgram =
      (PFNGLLINKPROGRAMPROC)glXGetProcAddress((unsigned char *)"glLinkProgram");
  glGenBuffers =
      (PFNGLGENBUFFERSPROC)glXGetProcAddress((unsigned char *)"glGenBuffers");
  glBindBuffer =
      (PFNGLBINDBUFFERPROC)glXGetProcAddress((unsigned char *)"glBindBuffer");
  glBufferData =
      (PFNGLBUFFERDATAPROC)glXGetProcAddress((unsigned char *)"glBufferData");
  glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)glXGetProcAddress(
      (unsigned char *)"glGenVertexArrays");
  glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)glXGetProcAddress(
      (unsigned char *)"glGetAttribLocation");
  glDisableVertexAttribArray =
      (PFNGLDISABLEVERTEXATTRIBARRAYPROC)glXGetProcAddress(
          (unsigned char *)"glDisableVertexAttribArray");
  glEnableVertexAttribArray =
      (PFNGLENABLEVERTEXATTRIBARRAYPROC)glXGetProcAddress(
          (unsigned char *)"glEnableVertexAttribArray");
  glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)glXGetProcAddress(
      (unsigned char *)"glVertexAttribPointer");
  glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glXGetProcAddress(
      (unsigned char *)"glBindVertexArray");
  glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)glXGetProcAddress(
      (unsigned char *)"glGetShaderInfoLog");
  glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)glXGetProcAddress(
      (unsigned char *)"glGetProgramInfoLog");
  glDeleteShader = (PFNGLDELETESHADERPROC)glXGetProcAddress(
      (unsigned char *)"glDeleteShader");
  glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)glXGetProcAddress(
      (unsigned char *)"glGetUniformLocation");
  glUniform4f =
      (PFNGLUNIFORM4FPROC)glXGetProcAddress((unsigned char *)"glUniform4f");
  glUniform3f =
      (PFNGLUNIFORM3FPROC)glXGetProcAddress((unsigned char *)"glUniform3f");
  glUniform1f =
      (PFNGLUNIFORM1FPROC)glXGetProcAddress((unsigned char *)"glUniform1f");
  glUniform1i =
      (PFNGLUNIFORM1IPROC)glXGetProcAddress((unsigned char *)"glUniform1i");
  glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)glXGetProcAddress(
      (unsigned char *)"glUniformMatrix4fv");
  glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)glXGetProcAddress(
      ((unsigned char *)"glGenerateMipmap"));
  glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)glXGetProcAddress(
      (unsigned char *)"glGenFramebuffers");
  glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)glXGetProcAddress(
      (unsigned char *)"glBindFramebuffer");
  glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)glXGetProcAddress(
      (unsigned char *)"glFramebufferTexture2D");
  glFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC)glXGetProcAddress(
      (unsigned char *)"glFramebufferTexture");
}
#endif
#ifdef CHECK_APPLE
void load_gl_functions() {}
#endif

void init_gl_properties() {
  glEnable(GL_DEPTH_TEST);

  //   glEnable(GL_CULL_FACE);
  glClearColor(CLEAR_R, CLEAR_G, CLEAR_B, CLEAR_A);
}
