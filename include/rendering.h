/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 16:37:13 by veilo             #+#    #+#             */
/*   Updated: 2022/04/05 16:01:51 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDERING_H
#define RENDERING_H

#include "app.h"
#include "assets.h"
#include "h_opengl.h"

void render_frame(t_app *app);
void render_object(t_app *app, t_3d_object *object, int shadow);
void render_ground(t_app *app);
void generate_shadowmap(t_app *app);
void render_shadow_pass(t_app *app);

#endif