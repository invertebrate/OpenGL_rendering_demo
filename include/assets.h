/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assets.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 15:59:36 by veilo             #+#    #+#             */
/*   Updated: 2022/04/20 17:02:12 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASSET_HANDLING
#define ASSET_HANDLING

#include "app.h"
#include "lm_matrix.h"
#include "texture_gl.h"

typedef struct s_dir_light {
  float pos[3];
  float dir[3];
  float color[3];
  float view[16];
  float strength;
  t_3d_object *obj;
  unsigned int id;
} t_dir_light;

typedef struct s_point_light {
  float pos[3];
  float dir[3];
  float color[3];
  float view[16 * 6];
  float strength;
  t_3d_object *obj;
  unsigned int id;
} t_point_light;

typedef enum e_texture_type {
  texture_type_diffuse = 0,
  texture_type_normal = 1,
  texture_type_specular = 2
} t_texture_type;

void rotate_light_obj(t_app *app, unsigned int index, int dir);
t_dir_light *create_dir_light(t_app *app, float *pos, float *dir, float *color,
                              float scale, char *mesh_path);
t_point_light *create_point_light(t_app *app, float *pos, float *dir,
                                  float *color, float scale, char *mesh_path);
void *object_load(t_app *app, char *filename);
unsigned int diffuse_load(t_app *app, t_3d_object *obj, char *filename);
int assets_init(t_app *app, int argc, char **argv);
int parse_arguments(t_app *app, int argc, char **argv);
char *parse_asset(t_app *app, char *asset);
int load_default(t_app *app);
int load_42_demo(t_app *app);
void update_objects(t_app *app);
void update_blending(t_app *app);
void cycle_objects(t_app *app);
void cycle_textures(t_app *app);
void cycle_normalmaps(t_app *app);
void center_model(t_3d_object *obj);
void update_object_rotation(t_app *app, t_3d_object *obj, float tim);
void obj_delete(t_3d_object *obj);
void load_skybox(t_app *app);

#endif