/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shaders.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 15:28:01 by veilo             #+#    #+#             */
/*   Updated: 2022/02/06 19:09:44 by veilo            ###   ########.fr       */
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
                         "layout (location = 1) in vec2 aTex;\n"
                         "layout (location = 2) in vec3 aNor;\n"
                         "out vec2 texCoord;\n"
                         "void main()\n"
                         "{\n"
                         " gl_Position = vec4(aPos, 1.0);\n"
                         "texCoord = aTex;\n"
                         "}\0";
  strcpy(source, v_source);
}

void get_fragment_shader_source(char *source) {
  const char *f_source = "#version 460\n"
                         "in vec2 texCoord;\n"
                         "out vec4 FragColor;\n"
                         "uniform sampler2D ourTexture;\n"
                         "void main()\n"
                         "{\n"
                         "FragColor = texture(ourTexture, texCoord);\n"
                         //  "FragColor = vec4(texCoord, 1.0) ;\n"
                         "}\0";
  strcpy(source, f_source);
}

// #version 330 core
// out vec4 FragColor;
// in vec3 ourColor;
// in vec2 TexCoord;
// uniform sampler2D ourTexture;
// void main()
// {
// FragColor = texture(ourTexture, TexCoord);
// }

// " FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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

  // how to set uniforms
  float greenValue = 0.5;
  int vertexColorLocation = glGetUniformLocation(
      shaderProgram, "ourColor"); // get index that opengl knows
  // if (vertexColorLocation == - 1)
  // failed
  glUseProgram(shaderProgram); // set the uniform to the used shader
  glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

  app->default_shader_program = shaderProgram;

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  free((char *)vertexShaderSource);
  free((char *)fragmentShaderSource);
}