/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asset_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 15:58:50 by veilo             #+#    #+#             */
/*   Updated: 2022/02/13 15:56:40 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asset_handling.h"

void objects_load(t_app *app) {
  t_3d_object *test = NULL;

  if (!(test = obj_read_from_file("resources_orig/teapot.obj"))) {
    printf("ERROR: Object reading failed for file: %s\n", "text.txt");
    return;
  }
  app->objects[app->object_count] = test;
  test->object_id = app->object_count;
  test->texture_id = app->object_count;
  app->object_count++;
}

void textures_load(t_app *app) {
  t_texture_data tempdata;

  tempdata.pixels = get_bitmap_from_file("resources/test.bmp", &tempdata);
  memcpy(app->texture_data[0], &tempdata, sizeof(tempdata));
  app->textures_gl[0] = create_texture(app->texture_data[0]);
}