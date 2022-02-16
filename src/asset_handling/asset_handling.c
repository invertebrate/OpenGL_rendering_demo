/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asset_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 15:58:50 by veilo             #+#    #+#             */
/*   Updated: 2022/02/16 21:23:46 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asset_handling.h"

void *object_load(t_app *app, char *filename) {
  t_3d_object *object = NULL;

  if (!(object = obj_read_from_file(filename))) {
    printf("ERROR: Object reading failed for file: %s\n", filename);
    return (NULL);
  }
  app->objects[app->object_count] = object;
  object->object_id = app->object_count;
  object->texture_id = 0;
  app->object_count++;
  return (object);
}

GLuint texture_load(t_app *app, char *filename) {
  t_texture_data tempdata;

  if (!(tempdata.pixels = get_bitmap_from_file(filename, &tempdata))) {
    printf("ERROR: Texture reading failed for file: %s\n", filename);
    return (0);
  }
  memcpy(app->texture_data[app->texture_count], &tempdata, sizeof(tempdata));
  app->textures_gl[app->texture_count] =
      create_texture(app->texture_data[app->texture_count]);
  app->texture_count++;
  free(tempdata.pixels);
  tempdata.pixels = NULL;
  return (app->texture_count);
}

int load_42_demo(t_app *app) {
  t_3d_object *obj = NULL;

  if (!(obj = object_load(app, "resources/42.obj")))
    return (0);
  center_model(obj);
  obj->shader = shader_type_42_demo;
  lm_mat4_translate(obj->translation, (float[3]){0, 0, -5}, obj->translation);
  lm_mat4_scale(obj->scale, obj->scale_factor * 4, obj->scale_factor * 4,
                obj->scale_factor * 4, obj->scale);
  obj->texture_id = 0;
  if (!(obj = object_load(app, "resources/teapot.obj")))
    return (0);
  center_model(obj);
  obj->shader = shader_type_42_demo;
  lm_mat4_translate(obj->translation, (float[3]){0, 0, -5}, obj->translation);
  lm_mat4_scale(obj->scale, obj->scale_factor * 4, obj->scale_factor * 4,
                obj->scale_factor * 4, obj->scale);
  obj->texture_id = 0;
  if (!(obj = object_load(app, "resources/cube.obj")))
    return (0);
  center_model(obj);
  obj->shader = shader_type_42_demo;
  lm_mat4_translate(obj->translation, (float[3]){0, 0, -2}, obj->translation);
  obj->texture_id = 0;
  if (!(texture_load(app, "resources/test.bmp")))
    return (0);
  if (!(texture_load(app, "resources/warning.bmp")))
    return (0);
  if (!(texture_load(app, "resources/cat.bmp")))
    return (0);
  return (1);
}

int load_default(t_app *app) {
  t_3d_object *obj = NULL;

  if (!(obj = object_load(app, "resources/monster.obj")))
    return (0);
  if (!(texture_load(app, "resources/monster_01/monster01_diffuse.bmp")))
    return (0);
  obj->shader = shader_type_default;
  center_model(obj);
  lm_mat4_translate(obj->translation, (float[3]){0, 0, -5}, obj->translation);
  lm_mat4_scale(obj->scale, obj->scale_factor * 4, obj->scale_factor * 4,
                obj->scale_factor * 4, obj->scale);
  if (!(obj = object_load(app, "resources/monster02.obj")))
    return (0);
  if (!(texture_load(app, "resources/monster_02/monster02_diffuse.bmp")))
    return (0);
  if (!(texture_load(app, "resources/test.bmp")))
    return (0);
  if (!(texture_load(app, "resources/warning.bmp")))
    return (0);
  if (!(texture_load(app, "resources/cat.bmp")))
    return (0);
  obj->shader = shader_type_default;
  center_model(obj);
  lm_mat4_translate(obj->translation, (float[3]){0, 0, -5}, obj->translation);
  lm_mat4_scale(obj->scale, obj->scale_factor * 4, obj->scale_factor * 4,
                obj->scale_factor * 4, obj->scale);
  return (1);
}

char *parse_asset(t_app *app, char *asset) {
  char *filepath = NULL;

  if (strncmp(asset, "o:", 2) == 0) {
    filepath = asset + 2;
    if (!(object_load(app, filepath)))
      return (NULL);
    return (filepath);
  } else if (strncmp(asset, "t:", 2) == 0) {
    filepath = asset + 2;
    if (!(texture_load(app, filepath)))
      return (NULL);
    return (filepath);
  }
  return (NULL);
}

int parse_arguments(t_app *app, int argc, char **argv) {
  if (argc == 2 && strcmp(argv[1], "42_demo") == 0) {
    if (!(load_42_demo(app)))
      return (0);
  } else if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      if (!(parse_asset(app, argv[i])))
        return (0);
    }
  } else {
    if (!(load_default(app)))
      return (0);
  }
  return (1);
}

int assets_init(t_app *app, int argc, char **argv) {
  if (!(parse_arguments(app, argc, argv))) {
    printf("ERROR: Invalid arguments. Asset initialization failed.\nCorrect "
           "arguments:\n42_demo\n[no "
           "argument]\no:[object_path]\nt:[texture_path]\n");
    return (0);
  }
  vaos_create(app);
  initialize_shaders(app);
  return (1);
}

void update_blending(t_app *app) {
  if (app->blending == SDL_TRUE) {
    app->demo_blend_value += 0.01 * app->blend_dir;
    if (app->demo_blend_value > 1.0) {
      app->demo_blend_value = 1.0;
      app->blending = SDL_FALSE;
      app->blend_dir = -1;
    }
    if (app->demo_blend_value < 0.0) {
      app->demo_blend_value = 0.0;
      app->blending = SDL_FALSE;
      app->blend_dir = 1;
    }
  }
}

void update_objects(t_app *app) {
  static float tim = 0;
  if (app->rotating)
    tim += 0.0065;
  if (app->object_count > 0)
    for (uint i = 0; i < app->object_count; i++) {
      update_object_rotation(app, app->objects[i], tim);
    }
  update_blending(app);
}

void update_object_rotation(t_app *app, t_3d_object *obj, float tim) {
  if (app->rotating)
    lm_mat4_create_rotmat(obj->rotation, (float[3]){0, 1, 0},
                          tim * (3.14159 / 2));
}

void center_model(t_3d_object *obj) {
  obj->model_matrix[12] -= obj->center_point[0];
  obj->model_matrix[13] -= obj->center_point[1];
  obj->model_matrix[14] -= obj->center_point[2];
}

void cycle_textures(t_app *app) {
  if (app->texture_count == 0 || app->object_count == 0)
    return;
  else {
    app->objects[app->active_object]->texture_id++;
    app->objects[app->active_object]->texture_id =
        app->objects[app->active_object]->texture_id % app->texture_count;
  }
}

void cycle_objects(t_app *app) {
  if (app->object_count == 0)
    return;
  else {
    app->active_object++;
    app->active_object = app->active_object % app->object_count;
  }
}