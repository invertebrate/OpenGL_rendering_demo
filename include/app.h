/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 15:47:28 by veilo             #+#    #+#             */
/*   Updated: 2022/01/16 16:54:29 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APP_H
#define APP_H

typedef struct s_object3D t_object3D;

typedef struct s_app {
  SDL_Window *window;
  SDL_GLContext main_context;
  t_object3D *objects;
  uint32_t custom_event_type;
  void *(**custom_event_handles)(void *);
  uint32_t custom_event_count;
  SDL_bool is_running;
} t_app;

#endif