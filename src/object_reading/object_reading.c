/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_reading.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/23 16:52:18 by veilo             #+#    #+#             */
/*   Updated: 2022/01/24 17:44:21 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "object_reading.h"

size_t file_size_get(char *filename) {
  FILE *fptr;
  size_t size = 0;

  fptr = fopen(filename, "r");
  fseek(fptr, 0, SEEK_END);
  size = ftell(fptr);
  fclose(fptr);
  return (size);
}

size_t substring_count(char *contents, char *substr) {
  char *temp;
  size_t sublen;
  size_t count = 0;

  sublen = strlen(substr);
  while (1) {
    temp = strstr(contents, substr);
    if (temp != NULL) {
      contents = temp + sublen;
      count++;
    } else
      break;
  }
  return (count);
}

size_t get_vertex_count(char *contents) {
  char *substr = VERTEX_PREFIX;

  return (substring_count(contents, substr));
}

uint get_vertex_from_line(t_float3 *vertex, char *line) {
  char *coordinate;

  coordinate = strstr(line, " ");
  if (!coordinate)
    return (OBJ_FAILURE);
  coordinate++;
  if (coordinate) {
    vertex->x = atof(coordinate);
    line = coordinate;
  }
  coordinate = strstr(line, " ");
  if (!coordinate)
    return (OBJ_FAILURE);
  coordinate++;
  if (coordinate) {
    vertex->y = atof(coordinate);
    line = coordinate;
  }
  coordinate = strstr(line, " ");
  if (!coordinate)
    return (OBJ_FAILURE);
  coordinate++;
  if (coordinate) {
    vertex->z = atof(coordinate);
    line = coordinate;
  }
  return (OBJ_SUCCESS);
}

t_float3 *store_vertices(char *contents, uint v_count) {
  t_float3 *vertices;
  char *startptr;
  char *line_token;

  vertices = (t_float3 *)malloc(sizeof(t_float3) * v_count);
  startptr = strstr(contents, VERTEX_PREFIX);
  line_token = strtok(startptr, "\n");
  for (uint i = 0; i < v_count; i++) {
    if (!startptr)
      break;
    else {
      if (!get_vertex_from_line(&vertices[i], line_token)) {
        free(vertices);
        return (NULL);
      }
      line_token = strtok(NULL, "\n");
      startptr = strstr(contents, VERTEX_PREFIX);
    }
  }
  return (vertices);
}

t_3d_object *obj_read_from_file(char *filename) {
  FILE *fptr = NULL;
  char *file_contents = NULL;
  size_t count = 0;
  size_t file_size = 0;
  t_float3 *vertices = NULL;
  t_float2 *uvs = NULL;
  t_3d_object *object = malloc(sizeof(t_3d_object));

  file_size = file_size_get(filename);
  file_contents = (char *)malloc(sizeof(char) * file_size);
  fptr = fopen(filename, "r");
  size_t r = fread(file_contents, sizeof(char), file_size, fptr);
  fclose(fptr);
  count = get_vertex_count(file_contents);
  vertices = store_vertices(file_contents, count);
  if (vertices == NULL)
    return (NULL);
  printf("vertex count: %lu\n", count);
  for (uint i = 0; i < count; i++) {
    printf("vertex %d: %f %f %f\n", i, vertices[i].x, vertices[i].y,
           vertices[i].z);
  }
  uvs = store_uvs(file_contents, count);
  (void)r;
  return (object);
}