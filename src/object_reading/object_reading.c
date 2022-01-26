/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_reading.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/23 16:52:18 by veilo             #+#    #+#             */
/*   Updated: 2022/01/26 18:19:36 by veilo            ###   ########.fr       */
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
  char *line_token;
  char *contents_copy_v = NULL;

  contents_copy_v = strdup(strstr(contents, VERTEX_PREFIX));
  vertices = (t_float3 *)malloc(sizeof(t_float3) * v_count);
  line_token = strtok(contents_copy_v, "\n");
  for (uint i = 0; i < v_count; i++) {
    if (!contents)
      break;
    else {
      if (!get_vertex_from_line(&vertices[i], line_token)) {
        free(vertices);
        vertices = NULL;
        free(contents_copy_v);
        contents_copy_v = NULL;
        return (NULL);
      }
      line_token = strtok(NULL, "\n");
      contents_copy_v = strstr(contents_copy_v, VERTEX_PREFIX);
    }
  }
  free(contents_copy_v);
  contents_copy_v = NULL;
  return (vertices);
}

uint get_uv_from_line(t_float2 *uv, char *line) {
  char *coordinate;

  coordinate = strstr(line, " ");
  if (!coordinate)
    return (OBJ_FAILURE);
  coordinate++;
  if (coordinate) {
    printf("u: %s\n", coordinate);
    uv->u = atof(coordinate);
    line = coordinate;
  }
  coordinate = strstr(line, " ");
  if (!coordinate)
    return (OBJ_FAILURE);
  coordinate++;
  if (coordinate) {
    printf("v: %s\n", coordinate);
    uv->v = atof(coordinate);
    line = coordinate;
  }
  return (OBJ_SUCCESS);
}

t_float2 *store_uvs(char *contents, uint v_count) {
  t_float2 *uvs;
  char *line_token = NULL;
  char *contents_copy_uv = NULL;

  printf("contents passed to uv: %s\n", contents);
  contents_copy_uv = strdup(strstr(contents, UV_PREFIX));
  uvs = (t_float2 *)malloc(sizeof(t_float2) * v_count);
  line_token = strtok(contents_copy_uv, "\n");
  for (uint i = 0; i < v_count; i++) {
    if (!contents_copy_uv)
      break;
    else {
      printf("linetoken in else to uv: %s\n", line_token);
      if (!get_uv_from_line(&uvs[i], line_token)) {
        printf("uv from line failed\n");
        free(uvs);
        uvs = NULL;
        free(contents_copy_uv);
        contents_copy_uv = NULL;
        return (NULL);
      }
      line_token = strtok(NULL, "\n");
      contents_copy_uv = strstr(contents_copy_uv, UV_PREFIX);
    }
  }
  free(contents_copy_uv);
  contents_copy_uv = NULL;
  return (uvs);
}

char *file_contents_get(char *filename) {
  FILE *fptr = NULL;
  size_t file_size = 0;
  char *contents = NULL;

  file_size = file_size_get(filename);
  contents = (char *)malloc(sizeof(char) * file_size);
  fptr = fopen(filename, "r");
  size_t r = fread(contents, sizeof(char), file_size, fptr);
  fclose(fptr);
  (void)r;
  return (contents);
}

t_3d_object *obj_read_from_file(char *filename) {
  char *file_contents = NULL;
  size_t count = 0;
  t_float3 *vertices = NULL;
  t_float2 *uvs = NULL;
  t_3d_object *object = malloc(sizeof(t_3d_object));

  file_contents = file_contents_get(filename);
  count = get_vertex_count(file_contents);
  vertices = store_vertices(file_contents, count);
  uvs = store_uvs(file_contents, count);
  if (vertices == NULL || uvs == NULL)
    return (NULL);
  printf("vertex count: %lu\n", count);
  for (uint i = 0; i < count; i++) {
    printf("vertex %d: %f %f %f\n", i, vertices[i].x, vertices[i].y,
           vertices[i].z);
  }
  for (uint i = 0; i < count; i++) {
    printf("uv %d: %f %f\n", i, uvs[i].u, uvs[i].v);
  }
  (void)vertices;
  return (object);
}