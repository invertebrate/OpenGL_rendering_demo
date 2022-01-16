/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 15:47:20 by veilo             #+#    #+#             */
/*   Updated: 2022/01/16 15:47:21 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "events.h"


void *custom_event_dummy(void * dummy)
{
  int x = 5;
  x += 10;
  (void)dummy;
  return (NULL);
}

void custom_event_handles_register(t_app *app)
{
		// auto pair = std::make_pair(e_event_code_gamewon, game_won);
		// custom_events.insert(pair);
		// pair = std::make_pair(e_event_code_gamelost, game_lost);
		// custom_events.insert(pair);
    app->custom_event_handles[app->custom_event_count - 1] = custom_event_dummy;
    app->custom_event_count++;
}

void custom_event_handle(SDL_Event *event)
{
		// auto handle = custom_events.find((e_event_code)event->user.code);
		// handle->second(event->user.data1, event->user.data2);
    ((void *(*)(void *))event->user.data1)(event->user.data2);
}


void custom_event_add(t_app *app, e_event_code event_code, void *data)
{
		if (app->custom_event_type != ((Uint32)-1))
		{
				SDL_Event event;
				SDL_memset(&event, 0, sizeof(event));
				event.type = app->custom_event_type;
				event.user.code = event_code;
				event.user.data1 = (void*)(app->custom_event_handles[event_code]);
				event.user.data2 = data;
				SDL_PushEvent(&event);
		}
}

void dummy_event_add(t_app *app)
{
  custom_event_add(app, e_event_code_dummy, NULL);
}