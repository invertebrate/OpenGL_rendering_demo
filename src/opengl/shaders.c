/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shaders.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 15:28:01 by veilo             #+#    #+#             */
/*   Updated: 2022/03/30 15:51:56 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shaders.h"
#include "file_utils.h"

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

const GLchar *get_shader_source(char *filename) {
  const GLchar *source;
  size_t filesize = 0;

  source = (const GLchar *)file_contents_get(filename, &filesize, 1);
  return (source);
}

void sdr_delete(void *ptr) {
  if (ptr != NULL) {
    free(ptr);
    ptr = NULL;
  }
}

void shader_init(t_app *app, const GLchar *vsource_file,
                 const GLchar *fsource_file, t_shader_type type) {
  const GLchar *v_shader_source;
  const GLchar *f_shader_source;
  unsigned int vertexShader;
  unsigned int fragmentShader;
  unsigned int shaderProgram;

  v_shader_source = get_shader_source((char *)vsource_file);
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &v_shader_source, NULL);
  glCompileShader(vertexShader);
  check_vertex_shader_compilation(vertexShader);
  f_shader_source = get_shader_source((char *)fsource_file);
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &f_shader_source, NULL);
  glCompileShader(fragmentShader);
  check_frag_shader_compilation(fragmentShader);
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  check_shader_linking(shaderProgram);
  app->shaders[type] = shaderProgram;
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  sdr_delete((void *)v_shader_source);
  sdr_delete((void *)f_shader_source);
}

void default_shader_init(t_app *app) {
  char *vsource = "shaders/v_shader_default.glsl";
  char *fsource = "shaders/f_shader_default.glsl";
  shader_init(app, vsource, fsource, shader_type_default);
}

void lighting_shader_init(t_app *app) {
  char *vsource = "shaders/v_shader_lighting.glsl";
  char *fsource = "shaders/f_shader_lighting.glsl";
  shader_init(app, vsource, fsource, shader_type_lighting);
}

void skybox_shader_init(t_app *app) {
  char *vsource = "shaders/v_shader_skybox.glsl";
  char *fsource = "shaders/f_shader_skybox.glsl";
  shader_init(app, vsource, fsource, shader_type_skybox);
}

void light_shader_init(t_app *app) {
  char *vsource = "shaders/v_shader_light.glsl";
  char *fsource = "shaders/f_shader_light.glsl";
  shader_init(app, vsource, fsource, shader_type_light);
}

void initialize_shaders(t_app *app) {
  default_shader_init(app);
  lighting_shader_init(app);
  skybox_shader_init(app);
  light_shader_init(app);
}