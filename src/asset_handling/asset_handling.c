/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asset_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 15:58:50 by veilo             #+#    #+#             */
/*   Updated: 2022/02/16 15:20:47 by veilo            ###   ########.fr       */
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
  if (app->active_object == NULL)
    app->active_object = object;
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
  return (app->texture_count);
}