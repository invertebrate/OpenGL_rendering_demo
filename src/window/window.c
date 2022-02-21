/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 15:40:23 by veilo             #+#    #+#             */
/*   Updated: 2022/02/21 14:37:29 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"

void window_init(t_app *app) {
  SDL_Window *window;

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  window =
      SDL_CreateWindow("SCOP", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       1000, 1000, SDL_WINDOW_OPENGL);
  app->main_context = SDL_GL_CreateContext(window);
  app->window = window;
  if (SDL_GL_SetSwapInterval(1) < 0) {
    printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
  }
}