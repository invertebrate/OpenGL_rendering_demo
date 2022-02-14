/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shaders.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 15:28:01 by veilo             #+#    #+#             */
/*   Updated: 2022/02/14 15:09:06 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shaders.h"

void check_vertex_shader_compilation(unsigned int vertexShader) {
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    printf("ERROR vertex shader compilation unsuccessful: %s\n", infoLog);
  }
}

void check_frag_shader_compilation(unsigned int fragmentShader) {
  int success;
  char infoLog[512];
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    printf("ERROR fragment shader compilation unsuccessful: %s\n", infoLog);
  }
}

void check_shader_linking(unsigned int shaderProgram) {
  int success;
  char infoLog[512];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    printf("ERROR shader linking unsuccessful: %s\n", infoLog);
  }
}

void get_vertex_shader_source(char *source) {
  const char *v_source =
      "#version 460\n"
      "layout (location = 0) in vec3 aPos;\n"
      "layout (location = 1) in vec2 aTex;\n"
      "layout (location = 2) in vec3 aNor;\n"
      "uniform mat4 transform;\n"
      "uniform mat4 scale;\n"
      "out vec2 texCoord;\n"
      "out vec3 norm;\n"
      "mat4 test;\n"
      "void main()\n"
      "{\n"
      "test = scale * transform;\n"
      "gl_Position = test * vec4(aPos, 1.0);\n"
      //  "texCoord = vec2(aPos.x, aPos.y);\n" //z axis projection
      // "texCoord = vec2(aPos.x, aPos.y);\n"
      "norm = aNor;\n"
      "texCoord = aTex;\n"
      "}\0";
  strcpy(source, v_source);
}

void get_fragment_shader_source(char *source) {
  const char *f_source = "#version 460\n"
                         "in vec2 texCoord;\n"
                         "in vec3 norm;\n"
                         "out vec4 FragColor;\n"
                         "uniform sampler2D ourTexture;\n"
                         "void main()\n"
                         "{\n"
                         "FragColor = texture(ourTexture, texCoord);\n"
                         //  "FragColor = vec4(norm, 0.4);\n"
                         "}\0";
  strcpy(source, f_source);
}

void default_shader_init(t_app *app) {
  const char *const vertexShaderSource = (const char *const)malloc(512);
  const char *const fragmentShaderSource = (const char *const)malloc(512);
  unsigned int vertexShader;
  unsigned int fragmentShader;
  unsigned int shaderProgram;

  get_vertex_shader_source((char *)vertexShaderSource);
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  check_vertex_shader_compilation(vertexShader);
  get_fragment_shader_source((char *)fragmentShaderSource);
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  check_frag_shader_compilation(fragmentShader);
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  check_shader_linking(shaderProgram);
  app->default_shader_program = shaderProgram;
  glUseProgram(shaderProgram);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  free((char *)vertexShaderSource);
  free((char *)fragmentShaderSource);
}

void shaders_init(t_app *app) {
  default_shader_init(app);
  (void)app;
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // put into gl init
}