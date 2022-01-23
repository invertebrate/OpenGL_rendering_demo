/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 15:47:28 by veilo             #+#    #+#             */
/*   Updated: 2022/01/23 17:41:29 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APP_H
#define APP_H

#include <GL/gl.h>
#include <SDL2/SDL.h>

#define MAX_PROGRAM_IDS 256

typedef struct s_object3D t_object3D;

typedef struct s_app {
  SDL_Window *window;
  SDL_GLContext main_context;
  t_object3D *objects;
  uint32_t custom_event_type;
  void *(**custom_event_handles)(void *);
  uint32_t custom_event_count;
  SDL_bool is_running;

  SDL_GLContext gl_context;

  uint32_t program_id_count;
  GLuint program_id;
  GLuint VAO;
  uint default_shader_program;
} t_app;

#endif