/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asset_handling.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 15:59:36 by veilo             #+#    #+#             */
/*   Updated: 2022/02/13 17:37:43 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASSET_HANDLING
#define ASSET_HANDLING

#include "app.h"
#include "texture_gl.h"

void *object_load(t_app *app, char *filename);
void *texture_load(t_app *app, char *filename);

#endif