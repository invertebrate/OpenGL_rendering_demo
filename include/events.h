/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 15:47:16 by veilo             #+#    #+#             */
/*   Updated: 2022/02/20 16:37:02 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTS_H
#define EVENTS_H

#include "app.h"
#include <SDL2/SDL.h>
#include <stdio.h>

#define MAX_CUSTOM_EVENTS 128

void events_handle(t_app *app, SDL_Event *event);

#endif