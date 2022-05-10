/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lights.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 14:59:20 by veilo             #+#    #+#             */
/*   Updated: 2022/05/10 16:28:56 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.h"
#include "assets.h"
#include "shaders.h"

void *light_creation_error(char *message) {
  printf("ERROR: Light creation failed. %s\n", message);
  return (NULL);
}

void rotate_light_obj(t_app *app, unsigned int index, int dir) {
  static float z = 0;

  lm_vec3_rotate(app->p_lights[index]->pos, (float[3]){0.0, 1.0, 0.0},
                 dir * app->delta_time * 10, app->p_lights[index]->pos);
  app->p_lights[index]->pos[1] = 3 * (sin(z) + 1) + 3;
  z += dir * app->delta_time * 10;
  lm_mat4_set_position(app->p_lights[index]->pos,
                       app->p_lights[index]->obj->translation);
  // light direction->
  //  lm_vec3_sub((float[3]){0.0, 0.0, 0.0}, app->lights[0]->pos,
  //              app->lights[0]->dir); // direction towards scene origin
  //  lm_vec3_scale(app->lights[0]->dir, -1,
  //                dir); // we live in an inverted world it seems
  (void)app, (void)index;
}

t_dir_light *create_dir_light(t_app *app, float *pos, float *dir, float *color,
                              float scale, char *mesh_path) {
  t_dir_light *light;
  t_3d_object *object;

  if (!(light = (t_dir_light *)calloc(1, sizeof(t_dir_light))))
    return (light_creation_error("Struct malloc failed.\n"));
  memcpy(light->pos, pos, sizeof(float[3]));
  memcpy(light->dir, dir, sizeof(float[3]));
  memcpy(light->color, color, sizeof(float[3]));
  if (!(object = obj_read_from_file(mesh_path))) {
    return (light_creation_error("Light object reading failed.\n"));
  }
  app->d_lights[app->d_light_count] = light;
  light->id = app->d_light_count;
  light->strength = 2.0;
  object->object_id = app->object_count;
  app->object_count++;
  app->objects[object->object_id] = object;
  object->shader = shader_type_light;
  lm_mat4_scale(object->scale, scale, scale, scale, object->scale);
  lm_mat4_translate(object->translation, pos, object->translation);
  light->obj = object;
  app->d_light_count++;
  return (light);
}

t_point_light *create_point_light(t_app *app, float *pos, float *color,
                                  float scale, char *mesh_path) {
  t_point_light *light;
  t_3d_object *object;

  if (!(light = (t_point_light *)calloc(1, sizeof(t_point_light))))
    return (light_creation_error("Struct malloc failed.\n"));
  memcpy(light->pos, pos, sizeof(float[3]));
  lm_vec3_normalize(color, light->color);
  // memcpy(light->color, color, sizeof(float[3]));
  if (!(object = obj_read_from_file(mesh_path))) {
    return (light_creation_error("Light object reading failed.\n"));
  }
  app->p_lights[app->p_light_count] = light;
  light->id = app->p_light_count;
  light->strength = 2.0;
  object->object_id = app->object_count; // questionable
  app->object_count++;
  app->objects[object->object_id] = object;
  object->shader = shader_type_light;
  lm_mat4_scale(object->scale, scale, scale, scale, object->scale);
  lm_mat4_translate(object->translation, pos, object->translation);
  light->obj = object;
  app->p_light_count++;
  return (light);
}