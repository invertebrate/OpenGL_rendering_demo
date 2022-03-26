/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asset_handling.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 15:59:36 by veilo             #+#    #+#             */
/*   Updated: 2022/03/26 15:33:18 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASSET_HANDLING
#define ASSET_HANDLING

#include "app.h"
#include "lm_matrix.h"
#include "texture_gl.h"

void *object_load(t_app *app, char *filename);
unsigned int diffuse_load(t_app *app, char *filename);
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