/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asset_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 15:58:50 by veilo             #+#    #+#             */
/*   Updated: 2022/02/13 17:38:42 by veilo            ###   ########.fr       */
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
  object->texture_id = app->object_count;
  app->object_count++;
  return (object);
}

void *texture_load(t_app *app, char *filename) {
  t_texture_data tempdata;

  if (!(tempdata.pixels = get_bitmap_from_file(filename, &tempdata))) {
    printf("ERROR: Texture reading failed for file: %s\n", filename);
    return (NULL);
  }
  memcpy(app->texture_data[0], &tempdata, sizeof(tempdata));
  app->textures_gl[0] = create_texture(app->texture_data[0]);
  return (tempdata.pixels);
}