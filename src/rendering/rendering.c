/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 16:36:43 by veilo             #+#    #+#             */
/*   Updated: 2022/01/21 16:36:54 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering.h"

void objects_render(t_app *app) {
  glBindVertexArray(app->VAO);
  glUseProgram(app->default_shader_program);
  // glDrawArrays(GL_TRIANGLES, 0, 3);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // draws all triangles with line
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void render(t_app *app) {
  glClear(GL_COLOR_BUFFER_BIT);
  objects_render(app);
  SDL_GL_SwapWindow(app->window);
  (void)app;
}

// Renderer::Renderer(SDL_Window* window) {
//   sdl_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//   sdl_window = window;
//   SDL_SetRenderDrawColor(sdl_renderer, 200, 100, 150, 255);
// }

// Renderer::~Renderer() {
//   SDL_DestroyRenderer(sdl_renderer);
// }

// void Renderer::render() {
//   SDL_RenderPresent(this->sdl_renderer);
// }

// SDL_Texture* Renderer::texture_load(const char* file) {
//   SDL_Texture* new_texture = NULL;
//   SDL_Surface* loaded_surface = IMG_Load(file);
//   if (loaded_surface == NULL)
//     printf("unable to load the image %s! SDL_omage Error: %s\n", file,
//            IMG_GetError());
//   else {
//     new_texture = SDL_CreateTextureFromSurface(sdl_renderer, loaded_surface);
//     if (new_texture == NULL)
//       printf(
//           "unable to create the texture from %s! SDL "
//           "Error: %s\n",
//           file, SDL_GetError());
//   }
//   SDL_FreeSurface(loaded_surface);
//   return (new_texture);
// }