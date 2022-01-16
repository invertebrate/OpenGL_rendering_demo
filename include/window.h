/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 15:46:59 by veilo             #+#    #+#             */
/*   Updated: 2022/01/16 15:47:01 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// class Window {
//   /*SDL_Texture		*frame;
//   t_framebuffer	*framebuffer;
//   TTF_Font		*main_font;
//   TTF_Font		*small_font;
//   TTF_Font		*title_font;
//   int32_t			width;
//   int32_t			height;
//   int32_t			pitch;

//   uint32_t		window_id;
//   t_bool			is_hidden;
//   t_bool			is_fullscreen;*/
//  private:
//   SDL_Window* sdl_window;

//  public:
//   Window(const char* name, int32_t width, int32_t height) {
//     sdl_window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED,
//                                   SDL_WINDOWPOS_CENTERED, width, height, 0);
//     printf("window created %p\n", sdl_window);
//   }
//   ~Window() {
//     SDL_DestroyWindow(sdl_window);
//     printf("window destroyed \n");
//   }
//   SDL_Window* sdl_window_get() {
//     printf("window get %p\n", sdl_window);
//     return (sdl_window);
//   }

//  private:
// };

#endif