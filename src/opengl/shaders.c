/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shaders.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 15:28:01 by veilo             #+#    #+#             */
/*   Updated: 2022/01/21 16:41:58 by veilo            ###   ########.fr       */
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
  const char *v_source = "#version 460\n"
                         "layout (location = 0) in vec3 aPos;\n"
                         "void main()\n"
                         "{\n"
                         " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                         "}\0";
  strcpy(source, v_source);
}

void get_fragment_shader_source(char *source) {
  const char *f_source = "#version 460\n"
                         "out vec4 FragColor;\n"
                         "void main()\n"
                         "{\n"
                         " FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                         "}\0";
  strcpy(source, f_source);
}

void shaders_init(t_app *app) {
  const char *const vertexShaderSource = (const char *const)malloc(512);
  get_vertex_shader_source((char *)vertexShaderSource);
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  check_vertex_shader_compilation(vertexShader);
  const char *const fragmentShaderSource = (const char *const)malloc(512);
  get_fragment_shader_source((char *)fragmentShaderSource);
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  check_frag_shader_compilation(fragmentShader);
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  check_shader_linking(shaderProgram);

  app->default_shader_program = shaderProgram;

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  free((char *)vertexShaderSource);
  free((char *)fragmentShaderSource);
}