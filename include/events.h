/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 15:47:16 by veilo             #+#    #+#             */
/*   Updated: 2022/01/16 15:47:16 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef EVENTS_H
#define EVENTS_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include "app.h"


#define MAX_CUSTOM_EVENTS 128

typedef enum e_event_code { e_event_code_dummy = 0 } e_event_code;

void custom_event_add(t_app* app, e_event_code event_code, void* data);
void custom_event_handle(SDL_Event* event);
void custom_event_handles_register(t_app* app);

#endif