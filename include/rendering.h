/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 16:37:13 by veilo             #+#    #+#             */
/*   Updated: 2022/05/05 18:58:55 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDERING_H
#define RENDERING_H

#include "app.h"
#include "assets.h"
#include "h_opengl.h"

void render_frame(t_app *app);
void draw_scene(t_app *app);
void render_object(t_app *app, t_3d_object *object, t_shader_type shader);
void render_ground(t_app *app, t_shader_type shader);
void generate_shadowmap(t_app *app);
void generate_shadow_cubemap(t_app *app);
void generate_depth_cubemap(t_app *app);
void render_shadow_pass(t_app *app);

#endif