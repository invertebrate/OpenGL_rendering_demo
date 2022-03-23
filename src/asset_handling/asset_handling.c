/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asset_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 15:58:50 by veilo             #+#    #+#             */
/*   Updated: 2022/03/23 16:53:43 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asset_handling.h"

void obj_delete(t_3d_object *obj) {
  if (!obj)
    return;
  if (obj->vertex_data_array) {
    free(obj->vertex_data_array);
    obj->vertex_data_array = NULL;
  }
  if (obj->triangles) {
    free(obj->triangles);
    obj->triangles = NULL;
  }
  free(obj);
  obj = NULL;
}

void *object_load(t_app *app, char *filename) {
  t_3d_object *object = NULL;

  if (!(object = obj_read_from_file(filename))) {
    printf("ERROR: Object reading failed for file: %s\n", filename);
    return (NULL);
  }
  app->objects[app->object_count] = object;
  object->object_id = app->object_count;
  object->diffuse_id = 0;
  object->shader = shader_type_default;
  app->object_count++;
  return (object);
}

unsigned int diffuse_load(t_app *app, char *filename) {
  t_texture_data tempdata;

  if (!(tempdata.pixels = get_bitmap_from_file(filename, &tempdata))) {
    printf("ERROR: Texture reading failed for file: %s\n", filename);
    return (0);
  }
  app->diffuses_gl[app->diffuse_count] = create_texture(&tempdata);
  app->diffuse_count++;
  free(tempdata.pixels);
  tempdata.pixels = NULL;
  return (app->diffuse_count);
}

unsigned int normalmap_load(t_app *app, char *filename) {
  t_texture_data tempdata;

  if (!(tempdata.pixels = get_bitmap_from_file(filename, &tempdata))) {
    printf("ERROR: Texture reading failed for file: %s\n", filename);
    return (0);
  }
  app->normalmaps_gl[app->normalmap_count] = create_texture(&tempdata);
  app->normalmap_count++;
  free(tempdata.pixels);
  tempdata.pixels = NULL;
  return (app->normalmap_count);
}

unsigned int specularmap_load(t_app *app, char *filename) {
  t_texture_data tempdata;

  if (!(tempdata.pixels = get_bitmap_from_file(filename, &tempdata))) {
    printf("ERROR: Texture reading failed for file: %s\n", filename);
    return (0);
  }
  app->specularmaps_gl[app->specularmap_count] = create_texture(&tempdata);
  app->specularmap_count++;
  free(tempdata.pixels);
  tempdata.pixels = NULL;
  return (app->specularmap_count);
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
  obj->diffuse_id = 0;
  if (!(obj = object_load(app, "resources/teapot.obj")))
    return (0);
  center_model(obj);
  obj->shader = shader_type_42_demo;
  lm_mat4_translate(obj->translation, (float[3]){0, 0, -5}, obj->translation);
  lm_mat4_scale(obj->scale, obj->scale_factor * 4, obj->scale_factor * 4,
                obj->scale_factor * 4, obj->scale);
  obj->diffuse_id = 0;
  if (!(obj = object_load(app, "resources/cube.obj")))
    return (0);
  center_model(obj);
  obj->shader = shader_type_lighting;
  lm_mat4_translate(obj->translation, (float[3]){0, 0, -2}, obj->translation);
  obj->diffuse_id = 0;
  if (!(diffuse_load(app, "resources/test.bmp")))
    return (0);
  if (!(diffuse_load(app, "resources/warning.bmp")))
    return (0);
  if (!(diffuse_load(app, "resources/cat.bmp")))
    return (0);
  return (1);
}

int load_default(t_app *app) {
  t_3d_object *obj = NULL;

  if (!(obj = object_load(app, "resources/mutant/bear.obj")))
    return (0);
  obj->shader = shader_type_lighting;
  obj->diffuse_id = 0;
  center_model(obj);
  lm_mat4_translate(obj->translation, (float[3]){0, 0, -2}, obj->translation);
  lm_mat4_scale(obj->scale, obj->scale_factor * 2, obj->scale_factor * 2,
                obj->scale_factor * 2, obj->scale);

  if (!(normalmap_load(app, "resources/mutant/bear_normal.bmp")))
    return (0);
  if (!(diffuse_load(app, "resources/mutant/bear_diffuse.bmp")))
    return (0);
  if (!(specularmap_load(app, "resources/mutant/bear_specular.bmp")))
    return (0);
  return (1);
}

char *parse_asset(t_app *app, char *asset) {
  char *filepath = NULL;
  t_3d_object *object = NULL;

  if (strncmp(asset, "o:", 2) == 0) {
    filepath = asset + 2;
    if (!(object = object_load(app, filepath)))
      return (NULL);
    center_model(object);
    lm_mat4_translate(object->translation, (float[3]){0, 0, -1},
                      object->translation);
    lm_mat4_scale(object->scale, object->scale_factor, object->scale_factor,
                  object->scale_factor, object->scale);
    object->shader = shader_type_lighting;
    object->diffuse_id = 0;
    return (filepath);
  } else if (strncmp(asset, "t:", 2) == 0) {
    filepath = asset + 2;
    if (!(diffuse_load(app, filepath)))
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
      parse_asset(app, argv[i]);
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

void update_objects(t_app *app) {
  if (app->object_count > 0)
    for (unsigned int i = 0; i < app->object_count; i++) {
      update_object_rotation(app, app->objects[i],
                             (float)app->rotating * app->delta_time);
    }
}

void update_object_rotation(t_app *app, t_3d_object *obj, float angle) {
  if (app->rotating) {
    float rotation[16];
    lm_mat4_create_rotmat(rotation, (float[3]){0, 1, 0},
                          angle * ROTATION_SPEED);
    lm_mat4_multiply(rotation, obj->rotation, obj->rotation);
  }
}

void center_model(t_3d_object *obj) {
  obj->model_matrix[12] -= obj->center_point[0];
  obj->model_matrix[13] -= obj->center_point[1];
  obj->model_matrix[14] -= obj->center_point[2];
}

void cycle_textures(t_app *app) {
  if (app->diffuse_count == 0 || app->object_count == 0)
    return;
  else {
    app->objects[app->active_object]->diffuse_id++;
    app->objects[app->active_object]->diffuse_id =
        app->objects[app->active_object]->diffuse_id % app->diffuse_count;
  }
}

void cycle_normalmaps(t_app *app) {
  if (app->normalmap_count == 0 || app->object_count == 0)
    return;
  else {
    app->objects[app->active_object]->normalmap_id++;
    app->objects[app->active_object]->normalmap_id =
        app->objects[app->active_object]->normalmap_id % app->normalmap_count;
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