/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 15:47:16 by veilo             #+#    #+#             */
/*   Updated: 2022/02/16 21:24:21 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTS_H
#define EVENTS_H

#include "app.h"
#include <SDL2/SDL.h>
#include <stdio.h>

#define MAX_CUSTOM_EVENTS 128

typedef enum e_event_code { e_event_code_dummy = 0 } e_event_code;

void custom_event_add(t_app *app, e_event_code event_code, void *data);
void custom_event_handle(SDL_Event *event);
void custom_event_handles_register(t_app *app);
void events_handle(t_app *app, SDL_Event *event);

#endif