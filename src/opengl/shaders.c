/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shaders.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 15:28:01 by veilo             #+#    #+#             */
/*   Updated: 2022/04/17 22:10:23 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shaders.h"
#include "file_utils.h"

void check_vertex_shader_compilation(unsigned int vertex_shader) {
  int success;
  char infoLog[512];
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
    printf("ERROR vertex shader compilation unsuccessful: %s\n", infoLog);
  }
}

void check_geometry_shader_compilation(unsigned int geometry_shader) {
  int success;
  char infoLog[512];
  glGetShaderiv(geometry_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(geometry_shader, 512, NULL, infoLog);
    printf("ERROR geometry shader compilation unsuccessful: %s\n", infoLog);
  }
}

void check_frag_shader_compilation(unsigned int fragment_shader) {
  int success;
  char infoLog[512];
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
    printf("ERROR fragment shader compilation unsuccessful: %s\n", infoLog);
  }
}

void check_shader_linking(unsigned int shaderProgram, t_shader_type type) {
  int success;
  char infoLog[512];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    printf("ERROR shader linking unsuccessful for shader type %u: %s\n", type,
           infoLog);
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

void shader_error(char *msg, const char *source) {
  printf("SHADER ERROR: %s %s\n", msg, source);
}

void vf_shader_init(t_app *app, const GLchar *vsource_file,
                    const GLchar *fsource_file, t_shader_type type) {
  const GLchar *v_shader_source;
  const GLchar *f_shader_source;
  unsigned int vertexShader;
  unsigned int fragmentShader;
  unsigned int shaderProgram;

  if (!(v_shader_source = get_shader_source((char *)vsource_file))) {
    shader_error("Shader source error: ", vsource_file);
    return;
  }
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &v_shader_source, NULL);
  glCompileShader(vertexShader);
  check_vertex_shader_compilation(vertexShader);
  if (!(f_shader_source = get_shader_source((char *)fsource_file))) {
    shader_error("Shader source error: ", fsource_file);
    return;
  }
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &f_shader_source, NULL);
  glCompileShader(fragmentShader);
  check_frag_shader_compilation(fragmentShader);
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  check_shader_linking(shaderProgram, type);
  app->shaders[type] = shaderProgram;
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  sdr_delete((void *)v_shader_source);
  sdr_delete((void *)f_shader_source);
}

void vgf_shader_init(t_app *app, const GLchar *vsource_file,
                     const GLchar *gsource_file, const GLchar *fsource_file,
                     t_shader_type type) {
  const GLchar *v_shader_source;
  const GLchar *f_shader_source;
  const GLchar *g_shader_source;
  unsigned int vertex_shader;
  unsigned int fragment_shader;
  unsigned int geometry_shader;
  unsigned int shader_program;

  if (!(v_shader_source = get_shader_source((char *)vsource_file))) {
    shader_error("Shader source error: ", vsource_file);
    return;
  }
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &v_shader_source, NULL);
  glCompileShader(vertex_shader);
  check_vertex_shader_compilation(vertex_shader);

  if (!(g_shader_source = get_shader_source((char *)gsource_file))) {
    shader_error("Shader source error: ", gsource_file);
    return;
  }
  geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
  glShaderSource(geometry_shader, 1, &g_shader_source, NULL);
  glCompileShader(geometry_shader);
  check_geometry_shader_compilation(geometry_shader);

  if (!(f_shader_source = get_shader_source((char *)fsource_file))) {
    shader_error("Shader source error: ", fsource_file);
    return;
  }
  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &f_shader_source, NULL);
  glCompileShader(fragment_shader);
  check_frag_shader_compilation(fragment_shader);

  shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glAttachShader(shader_program, geometry_shader);
  glLinkProgram(shader_program);
  check_shader_linking(shader_program, type);
  app->shaders[type] = shader_program;
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  glDeleteShader(geometry_shader);
  sdr_delete((void *)v_shader_source);
  sdr_delete((void *)f_shader_source);
  sdr_delete((void *)g_shader_source);
}

void default_shader_init(t_app *app) {
  char *vsource = "shaders/v_s_default.glsl";
  char *fsource = "shaders/f_s_default.glsl";
  vf_shader_init(app, vsource, fsource, shader_type_default);
}

void lighting_shader_init(t_app *app) {
  char *vsource = "shaders/v_s_lighting.glsl";
  char *fsource = "shaders/f_s_lighting.glsl";
  vf_shader_init(app, vsource, fsource, shader_type_lighting);
}

void skybox_shader_init(t_app *app) {
  char *vsource = "shaders/v_s_skybox.glsl";
  char *fsource = "shaders/f_s_skybox.glsl";
  vf_shader_init(app, vsource, fsource, shader_type_skybox);
}

void light_shader_init(t_app *app) {
  char *vsource = "shaders/v_s_light.glsl";
  char *fsource = "shaders/f_s_light.glsl";
  vf_shader_init(app, vsource, fsource, shader_type_light);
}

void depth_shader_init(t_app *app) {
  char *vsource = "shaders/v_s_depth.glsl";
  char *fsource = "shaders/f_s_depth.glsl";
  vf_shader_init(app, vsource, fsource, shader_type_depth);
}

void cube_shadow_shader_init(t_app *app) {
  char *vsource = "shaders/v_s_cube_shadow.glsl";
  char *fsource = "shaders/f_s_cube_shadow.glsl";
  char *gsource = "shaders/g_s_cube_shadow.glsl";
  vgf_shader_init(app, vsource, gsource, fsource, shader_type_cube_shadow);
}

void debug_shader_init(t_app *app) {
  char *vsource = "shaders/v_s_debug.glsl";
  char *fsource = "shaders/f_s_debug.glsl";
  char *gsource = "shaders/g_s_debug.glsl";
  vgf_shader_init(app, vsource, gsource, fsource, shader_type_debug);
}

void initialize_shaders(t_app *app) {
  default_shader_init(app);
  lighting_shader_init(app);
  debug_shader_init(app);
  skybox_shader_init(app);
  light_shader_init(app);
  depth_shader_init(app);
  // cube_shadow_shader_init(app);
}