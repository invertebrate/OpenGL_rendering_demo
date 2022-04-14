/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 15:46:59 by veilo             #+#    #+#             */
/*   Updated: 2022/04/14 16:22:57 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WINDOW_H
#define WINDOW_H

#include "app.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define W_ASPECT 16.0 / 9.0
#define WINDOW_WIDTH 1500.0 * (W_ASPECT)
#define WINDOW_HEIGHT 1500.0

void window_init(t_app *app);

#endif