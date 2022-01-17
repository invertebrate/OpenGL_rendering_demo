/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_gl.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 15:36:42 by veilo             #+#    #+#             */
/*   Updated: 2022/01/17 17:51:43 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "init_gl.h"

void load_gl(t_app *app) {
  // void(*)() glXGetProcAddress(	const GLubyte * procName);
  // In an initialization routine
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
  (void)app;
}

SDL_bool init_gl(t_app *app) {
  // Success flag
  // Generate program
  GLuint gProgramID = 0;
  GLint gVertexPos2DLocation = -1;
  GLuint gVBO = 0;
  GLuint gIBO = 0;
  gProgramID = glCreateProgram();
  printf("gluint %u\n", gProgramID);
  app->program_id = gProgramID;
  app->program_id_count++;
  // Create vertex shader
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  // Get vertex source
  const GLchar *vertexShaderSource[] = {
      "#version 140\nin vec2 LVertexPos2D; void main() { gl_Position = vec4( "
      "LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); }"};
  // Set vertex source
  glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
  // Compile vertex source
  glCompileShader(vertexShader);
  // Check vertex shader for errors
  GLint vShaderCompiled = GL_FALSE;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
  // printf("compiled %d\n", vShaderCompiled);
  if (vShaderCompiled != GL_TRUE) {
    printf("Unable to compile vertex shader %d!\n", vertexShader);
    // printShaderLog(vertexShader);
    // success = false;
  } else {
    // Attach vertex shader to program
    glAttachShader(gProgramID, vertexShader);
    // Create fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // // Get fragment source
    const GLchar *fragmentShaderSource[] = {
        "#version 140\nout vec4 LFragment; void main() { LFragment = vec4( "
        "1.0, 1.0, 1.0, 1.0 ); }"};
    // Set fragment source
    glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
    // Compile fragment source
    glCompileShader(fragmentShader);
    // Check fragment shader for errors
    GLint fShaderCompiled = GL_FALSE;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
    if (fShaderCompiled != GL_TRUE) {
      printf("Unable to compile fragment shader %d!\n", fragmentShader);
      // printShaderLog(fragmentShader);
      // success = false;
    } else {
      // Attach fragment shader to program
      glAttachShader(gProgramID, fragmentShader);
      // Link program
      glLinkProgram(gProgramID);
      // Check for errors
      GLint programSuccess = GL_TRUE;
      glGetProgramiv(gProgramID, GL_LINK_STATUS, &programSuccess);
      if (programSuccess != GL_TRUE) {
        printf("Error linking program %d!\n", gProgramID);
        // printProgramLog(gProgramID);
        // success = false;
      } else {
        // Get vertex attribute location
        gVertexPos2DLocation = glGetAttribLocation(gProgramID, "LVertexPos2D");
        if (gVertexPos2DLocation == -1) {
          printf("LVertexPos2D is not a valid glsl program variable!\n");
          // success = false;
        }
        // If the program linked successfully we then get the attribute from the
        //     shader program so we can send it vertex data.
        else {
          // Initialize clear color
          app->gVertexPos2DLocation = gVertexPos2DLocation;
          // glClearColor(0.f, 0.f, 0.f, 1.f);
          // VBO data
          GLfloat vertexData[] = {-0.5f, -0.5f, 0.5f,  -0.5f,
                                  0.5f,  0.5f,  -0.5f, 0.5f};
          // IBO data
          GLuint indexData[] = {0, 1, 2, 3};
          // Create VBO
          glGenBuffers(1, &gVBO);
          glBindBuffer(GL_ARRAY_BUFFER, gVBO);
          glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData,
                       GL_STATIC_DRAW);
          // Create IBO
          glGenBuffers(1, &gIBO);
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
          glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData,
                       GL_STATIC_DRAW);
          app->gIBO = gIBO;
          app->gVBO = gVBO;
          return SDL_TRUE;
        }
      }
    }
  }
  return SDL_FALSE;
}

void render(t_app *app) {
  // Clear color buffer
  glClearColor(0.0f, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  // Render quad
  // Bind program
  glUseProgram(app->program_id);
  // Enable vertex position
  glEnableVertexAttribArray(app->gVertexPos2DLocation);
  // Set vertex data
  glBindBuffer(GL_ARRAY_BUFFER, app->gVBO);
  glVertexAttribPointer(app->gVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE,
                        2 * sizeof(GLfloat), NULL);
  // Set index data and render
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, app->gIBO);
  glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);
  // Disable vertex position
  glDisableVertexAttribArray(app->gVertexPos2DLocation);
  // Unbind program
  glUseProgram(0);
  // Clear color buffer

  SDL_GL_SwapWindow(app->window);
  (void)app;
}

void gl_temp(t_app *app) {
  // Create context
  // app->gl_context = SDL_GL_CreateContext(app->window);
  // if (app->gl_context == NULL) {
  //   printf("OpenGL context could not be created! SDL Error: %s\n",
  //          SDL_GetError());
  // }
  // Use Vsync
  if (SDL_GL_SetSwapInterval(1) < 0) {
    printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
  }
  // Initialize OpenGL
  if (!init_gl(app)) {
    printf("Unable to initialize OpenGL!\n");
  }
}