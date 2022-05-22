/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asset_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 15:58:50 by veilo             #+#    #+#             */
/*   Updated: 2022/05/20 15:26:35 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "assets.h"

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

unsigned int diffuse_load(t_app *app, t_3d_object *obj, char *filename) {
  t_texture_data tempdata;

  if (!(tempdata.pixels = get_bitmap_from_file(filename, &tempdata))) {
    printf("ERROR: Texture reading failed for file: %s\n", filename);
    return (0);
  }
  app->diffuses_gl[app->diffuse_count] =
      create_texture(&tempdata, texture_type_diffuse);
  if (obj != NULL)
    obj->diffuse_id = app->diffuse_count;
  app->diffuse_count++;
  free(tempdata.pixels);
  tempdata.pixels = NULL;
  return (app->diffuse_count);
}

unsigned int normalmap_load(t_app *app, t_3d_object *obj, char *filename) {
  t_texture_data tempdata;

  if (!(tempdata.pixels = get_bitmap_from_file(filename, &tempdata))) {
    printf("ERROR: Texture reading failed for file: %s\n", filename);
    return (0);
  }
  app->normalmaps_gl[app->normalmap_count] =
      create_texture(&tempdata, texture_type_normal);
  if (obj != NULL)
    obj->normalmap_id = app->normalmap_count;
  app->normalmap_count++;
  free(tempdata.pixels);
  tempdata.pixels = NULL;
  return (app->normalmap_count);
}

unsigned int specularmap_load(t_app *app, t_3d_object *obj, char *filename) {
  t_texture_data tempdata;

  if (!(tempdata.pixels = get_bitmap_from_file(filename, &tempdata))) {
    printf("ERROR: Texture reading failed for file: %s\n", filename);
    return (0);
  }
  app->specularmaps_gl[app->specularmap_count] =
      create_texture(&tempdata, texture_type_specular);
  if (obj != NULL)
    obj->specularmap_id = app->specularmap_count;
  app->specularmap_count++;
  free(tempdata.pixels);
  tempdata.pixels = NULL;
  return (app->specularmap_count);
}

void place_light_sources(t_app *app) {
  t_point_light *light;

  for (int i = 0; i < app->p_light_count - 1; i++) {
    light = app->p_lights[i];
    lm_mat4_translate(light->obj->translation, (float[3]){0.0, 6.0, 0.0},
                      light->obj->translation);
    lm_mat4_translate(light->obj->translation,
                      (float[3]){cos(M_PI * 2 / 3) * 8 - 0.1 * i, 0.0 + 0.1 * i,
                                 sin(M_PI * 2 / 3) * 8 + 0.1 * i},
                      light->obj->translation);
    memcpy(light->pos, light->obj->translation + 12, sizeof(float) * 3);
  }
}

int load_terrain(t_app *app) {
  t_3d_object *obj = NULL;
  char objects[40][40] = {
      "resources/cave/orb4.obj",         "resources/cave/arch_big_a.obj",
      "resources/cave/arch_big_b.obj",   "resources/cave/arch_big_c.obj",
      "resources/cave/arch_big_d.obj",   "resources/cave/arch_small_a.obj",
      "resources/cave/arch_small_c.obj", "resources/cave/archway.obj",
      "resources/cave/corner_b.obj",     "resources/cave/pillar_b.obj",
      "resources/cave/pillar_c.obj",     "resources/cave/rock_decor.obj",
      "resources/cave/rock_reg_a.obj",   "resources/cave/rock_reg_b.obj",
      "resources/cave/rock_reg_c.obj",   "resources/cave/rock_spiky.obj",
      "resources/cave/wall_a.obj",       "resources/cave/wall_b.obj"};

  char diffuses[40][40] = {"resources/cave/Marble045_COL_2K_3.bmp",
                           "resources/cave/T_RockArchBig_A_D.bmp",
                           "resources/cave/T_RockArchBig_B_D.bmp",
                           "resources/cave/T_RockArchBig_C_D.bmp",
                           "resources/cave/T_RockArchBig_D_D.bmp",
                           "resources/cave/T_RockArchSmall_A_D.bmp",
                           "resources/cave/T_RockArchSmall_C_D.bmp",
                           "resources/cave/T_RockWalls_D.bmp",
                           "resources/cave/T_RockWalls_D.bmp",
                           "resources/cave/T_RockPillarBig_B_D.bmp",
                           "resources/cave/T_RockPillarBig_C_D.bmp",
                           "resources/cave/T_RockDecor_B_D.bmp",
                           "resources/cave/T_RockRegular_A_D.bmp",
                           "resources/cave/T_RockRegular_B_D.bmp",
                           "resources/cave/T_RockRegular_C_D.bmp",
                           "resources/cave/T_RockSpiky_D.bmp",
                           "resources/cave/T_RockWalls_D.bmp",
                           "resources/cave/T_RockWalls_D.bmp"};

  char normals[40][40] = {"resources/cave/Marble045_NORM_2K.bmp",
                          "resources/cave/T_RockArchBig_A_N.bmp",
                          "resources/cave/T_RockArchBig_B_N.bmp",
                          "resources/cave/T_RockArchBig_C_N.bmp",
                          "resources/cave/T_RockArchBig_D_N.bmp",
                          "resources/cave/T_RockArchSmall_A_N.bmp",
                          "resources/cave/T_RockArchSmall_C_N.bmp",
                          "resources/cave/T_RockWalls_N.bmp",
                          "resources/cave/T_RockWalls_N.bmp",
                          "resources/cave/T_RockPillarBig_B_N.bmp",
                          "resources/cave/T_RockPillarBig_C_N.bmp",
                          "resources/cave/T_RockDecor_B_N.bmp",
                          "resources/cave/T_RockRegular_A_N.bmp",
                          "resources/cave/T_RockRegular_B_N.bmp",
                          "resources/cave/T_RockRegular_C_N.bmp",
                          "resources/cave/T_RockSpiky_N.bmp",
                          "resources/cave/T_RockWalls_N.bmp",
                          "resources/cave/T_RockWalls_N.bmp"};

  char orms[40][50] = {"resources/cave/Marble045_REFL_2K.bmp",
                       "resources/cave/T_RockArchBig_A_ORM.bmp",
                       "resources/cave/T_RockArchBig_B_ORM.bmp",
                       "resources/cave/T_RockArchBig_C_ORM.bmp",
                       "resources/cave/T_RockArchBig_D_ORM.bmp",
                       "resources/cave/T_RockArchSmall_A_ORM.bmp",
                       "resources/cave/T_RockArchSmall_C_ORM.bmp",
                       "resources/cave/T_RockWalls_ORM.bmp",
                       "resources/cave/T_RockWalls_ORM.bmp",
                       "resources/cave/T_RockPillarBig_B_ORM.bmp",
                       "resources/cave/T_RockPillarBig_C_ORM.bmp",
                       "resources/cave/T_RockDecor_B_ORM.bmp",
                       "resources/cave/T_RockRegular_A_ORM.bmp",
                       "resources/cave/T_RockRegular_B_ORM.bmp",
                       "resources/cave/T_RockRegular_C_ORM.bmp",
                       "resources/cave/T_RockSpiky_ORM.bmp",
                       "resources/cave/T_RockWalls_ORM.bmp",
                       "resources/cave/T_RockWalls_ORM.bmp"};

  int count = 18;
  for (int i = 0; i < count; i++) {
    if (!(obj = object_load(app, objects[i])))
      return (0);
    obj->shader = shader_type_lighting;
    obj->diffuse_id = 0;
    app->terrain_obj_count++;
    if (i == 0)
      lm_mat4_scale(obj->scale, 2, 2, 2, obj->scale);
    if (!(normalmap_load(app, obj, normals[i])))
      return (0);
    if (!(diffuse_load(app, obj, diffuses[i])))
      return (0);
    if (!(specularmap_load(app, obj, orms[i])))
      return (0);
  }
  return (1);
}

int load_default(t_app *app) {
  t_3d_object *obj = NULL;
  (void)obj;
  load_terrain(app);
  float color[3] = {255.0 / 255.0, 186.0 / 255.0, 77.0 / 255.0};
  for (int i = 0; i < 4; i++) {
    t_point_light *light;
    if (!(light = create_point_light(app, (float[3]){0, 0, 0}, color,
                                     0.52 * 0.05, "resources/sphere_hr.obj")))
      return (0);
    light->strength = 0.5;
    light->falloff = 0.9;
  }

  place_light_sources(app);
  t_point_light *light;
  if (!(light = create_point_light(
            app, (float[3]){0, 0, 0},
            (float[3]){100.0 / 255.0, 255.0 / 255.0, 180.0 / 255.0}, 2.35,
            "resources/sphere_hr.obj")))
    return (0);
  light->strength = 6.5;
  light->falloff = 2;
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
    if (!(diffuse_load(app, NULL, filepath)))
      return (NULL);
    return (filepath);
  }
  return (NULL);
}

int parse_arguments(t_app *app, int argc, char **argv) {
  if (argc > 1) {
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
  load_skybox(app);
  return (1);
}

void update_objects(t_app *app) {
  for (int i = 0; i < app->p_light_count - 1; i++) {
    update_object_rotation(app, app->p_lights[i]->obj, i);
    memcpy(app->p_lights[i]->pos, app->p_lights[i]->obj->translation + 12,
           sizeof(float) * 3);
  }
}

void update_object_rotation(t_app *app, t_3d_object *obj, int index) {
  static float r = 0.0;
  static float s = 0.0;
  static float t = 0.0;
  float f1 = 0.2;
  float f2 = 0.1;
  float f3 = 0.2;
  float fa = 3.0;
  float fr = 0.02;
  float offset = ((2 * M_PI) / 2) * index * 1;
  float height = 0.3;
  float d = 3.2;
  float ampl0[3] = {0.55 / 2.0, 0.5 / 2.0, 0.55 / 2.0};
  float ampl1[3] = {0.02, 0.02, 0.02};

  if (app->rotating) {
    float rotation[16];
    lm_mat4_create_rotmat(rotation, (float[3]){0, 1, 0},
                          -ROTATION_SPEED / 3500.0);
    lm_mat4_multiply(rotation, obj->rotation, obj->rotation);
    lm_mat4_multiply(rotation, app->objects[0]->rotation,
                     app->objects[0]->rotation);
    app->p_lights[index]->obj->translation[12] =
        ampl0[0] * cos(f1 * r + (float)index * 2) *
            sin(f1 * r + (float)index * 2 / 3) +
        -ampl0[0] * cos(f2 * r + (float)index) + ampl1[0] * cos(s * fa) +
        d * sin(t * fr + offset);
    app->p_lights[index]->obj->translation[13] =
        2 * ampl0[1] * cos(f2 * r / 5) +
        ampl0[1] * sin(f2 * r + (float)index * 3 + offset) +
        ampl0[1] * cos(f2 * r + (float)index) + ampl1[1] * cos(t * fa) + height;
    app->p_lights[index]->obj->translation[14] =
        ampl0[2] * sin(f3 * r + (float)index * 2) *
            cos(f2 * r + (float)index / 3) +
        ampl1[2] * cos(s * fa) + d * cos(t * fr + offset);
    r += 0.1;
    t += 0.1 + 0.2 * sin(((float)((float)index / 2.0) * s) / 5.0) *
                   cos(((float)((float)index / 2.0) * s) / 5.0);
    s += 0.1;
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
