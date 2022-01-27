/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_reading.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/23 16:52:18 by veilo             #+#    #+#             */
/*   Updated: 2022/01/27 19:28:31 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "object_reading.h"
#include "h_opengl.h"

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

size_t get_uv_count(char *contents) {
  char *substr = UV_PREFIX;

  return (substring_count(contents, substr));
}

size_t get_normal_count(char *contents) {
  char *substr = NORMAL_PREFIX;

  return (substring_count(contents, substr));
}

uint get_position_from_line(t_float3 *vertex, char *line) {
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

uint get_uv_from_line(t_float2 *uv, char *line) {
  char *coordinate;

  coordinate = strstr(line, " ");
  if (!coordinate)
    return (OBJ_FAILURE);
  coordinate++;
  if (coordinate) {
    uv->u = atof(coordinate);
    line = coordinate;
  }
  coordinate = strstr(line, " ");
  if (!coordinate)
    return (OBJ_FAILURE);
  coordinate++;
  if (coordinate) {
    uv->v = atof(coordinate);
    line = coordinate;
  }
  return (OBJ_SUCCESS);
}

uint get_normal_from_line(t_float3 *normal, char *line) {
  char *coordinate;

  coordinate = strstr(line, " ");
  if (!coordinate)
    return (OBJ_FAILURE);
  coordinate++;
  if (coordinate) {
    normal->x = atof(coordinate);
    line = coordinate;
  }
  coordinate = strstr(line, " ");
  if (!coordinate)
    return (OBJ_FAILURE);
  coordinate++;
  if (coordinate) {
    normal->y = atof(coordinate);
    line = coordinate;
  }
  coordinate = strstr(line, " ");
  if (!coordinate)
    return (OBJ_FAILURE);
  coordinate++;
  if (coordinate) {
    normal->z = atof(coordinate);
    line = coordinate;
  }
  return (OBJ_SUCCESS);
}

char *parse_positions(char *contents_copy_p, t_float3 *positions,
                      uint p_count) {
  char *line_token = NULL;
  line_token = strtok(contents_copy_p, "\n");

  for (uint i = 0; i < p_count; i++) {
    if (!contents_copy_p)
      break;
    else {
      if (!get_position_from_line(&positions[i], line_token)) {
        return (NULL);
      }
      line_token = strtok(NULL, "\n");
      contents_copy_p = strstr(contents_copy_p, VERTEX_PREFIX);
    }
  }
  return (contents_copy_p);
}

t_float3 *store_positions(char *contents, size_t *count_check) {
  t_float3 *positions = NULL;
  char *contents_copy_p = NULL;
  size_t p_count = 0;

  p_count = get_vertex_count(contents);
  *count_check += p_count;
  if (!(contents = strstr(contents, VERTEX_PREFIX))) {
    return (NULL);
  }
  if (!(contents_copy_p = strdup(contents))) {
    return (NULL);
  }
  if (!(positions = (t_float3 *)calloc(p_count, sizeof(t_float3))))
    return (NULL);
  if (!(parse_positions(contents_copy_p, positions, p_count))) {
    free(contents_copy_p);
    contents_copy_p = NULL;
    free(positions);
    positions = NULL;
    return (NULL);
  };
  free(contents_copy_p);
  contents_copy_p = NULL;
  return (positions);
}

char *parse_uvs(char *contents_copy_uv, t_float2 *uvs, uint uv_count) {
  char *line_token = NULL;
  line_token = strtok(contents_copy_uv, "\n");

  for (uint i = 0; i < uv_count; i++) {
    if (!contents_copy_uv)
      break;
    else {
      if (!get_uv_from_line(&uvs[i], line_token)) {
        return (NULL);
      }
      line_token = strtok(NULL, "\n");
      contents_copy_uv = strstr(contents_copy_uv, UV_PREFIX);
    }
  }
  return (contents_copy_uv);
}

t_float2 *store_uvs(char *contents, size_t count_check) {
  t_float2 *uvs = NULL;
  char *contents_copy_uv = NULL;
  size_t uv_count = 0;

  uv_count = get_uv_count(contents);
  if (count_check != uv_count)
    return (NULL);
  if (!(contents = strstr(contents, UV_PREFIX))) {
    return (NULL);
  }
  if (!(contents_copy_uv = strdup(contents))) {
    return (NULL);
  }
  if (!(uvs = (t_float2 *)calloc(uv_count, sizeof(t_float2))))
    return (NULL);
  if (!(parse_uvs(contents_copy_uv, uvs, uv_count))) {
    free(contents_copy_uv);
    contents_copy_uv = NULL;
    free(uvs);
    uvs = NULL;
    return (NULL);
  };
  free(contents_copy_uv);
  contents_copy_uv = NULL;
  return (uvs);
}

char *parse_normals(char *contents_copy_n, t_float3 *normals, uint n_count) {
  char *line_token = NULL;
  line_token = strtok(contents_copy_n, "\n");

  for (uint i = 0; i < n_count; i++) {
    if (!contents_copy_n)
      break;
    else {
      if (!get_normal_from_line(&normals[i], line_token)) {
        return (NULL);
      }
      line_token = strtok(NULL, "\n");
      contents_copy_n = strstr(contents_copy_n, NORMAL_PREFIX);
    }
  }
  return (contents_copy_n);
}

t_float3 *store_normals(char *contents, size_t count_check) {
  t_float3 *normals = NULL;
  char *contents_copy_n = NULL;
  size_t n_count = 0;

  n_count = get_normal_count(contents);
  if (count_check != n_count)
    return (NULL);
  if (!(contents = strstr(contents, NORMAL_PREFIX))) {
    return (NULL);
  }
  if (!(contents_copy_n = strdup(contents))) {
    return (NULL);
  }
  if (!(normals = (t_float3 *)calloc(n_count, sizeof(t_float3))))
    return (NULL);
  if (!(parse_normals(contents_copy_n, normals, n_count))) {
    free(contents_copy_n);
    contents_copy_n = NULL;
    free(normals);
    normals = NULL;
    return (NULL);
  };
  free(contents_copy_n);
  contents_copy_n = NULL;
  return (normals);
}

char *file_contents_get(char *filename) {
  FILE *fptr = NULL;
  size_t file_size = 0;
  char *contents = NULL;

  file_size = file_size_get(filename);
  if (!(contents = (char *)calloc(file_size, sizeof(char))))
    return (NULL);
  fptr = fopen(filename, "r");
  size_t r = fread(contents, sizeof(char), file_size, fptr);
  fclose(fptr);
  (void)r;
  return (contents);
}

float *create_vertex_data_array(t_float3 *positions, t_float3 *normals,
                                t_float2 *uvs, size_t count) {
  float *vertex_data_array;
  int offset = VERTEX_STRIDE_PUVN / 4;
  int offset_uv = 2;
  int offset_normal = offset_uv + 3;

  if (!(vertex_data_array = (float *)calloc(count, VERTEX_STRIDE_PUVN)))
    return (NULL);
  for (size_t i = 0; i < count; i++) {
    vertex_data_array[(i * offset)] = positions[i].x;
    vertex_data_array[(i * offset) + 1] = positions[i].y;
    vertex_data_array[(i * offset) + 2] = positions[i].z;

    vertex_data_array[(i * offset) + offset_uv] = uvs[i].u;
    vertex_data_array[(i * offset) + offset_uv + 1] = uvs[i].v;

    vertex_data_array[(i * offset) + offset_normal] = normals[i].x;
    vertex_data_array[(i * offset) + offset_normal + 1] = normals[i].y;
    vertex_data_array[(i * offset) + offset_normal + 2] = normals[i].z;
  }
  return (vertex_data_array);
}

t_3d_object *obj_read_from_file(char *filename) {
  char *file_contents = NULL;
  t_float3 *positions = NULL;
  t_float2 *uvs = NULL;
  t_float3 *normals = NULL;
  t_3d_object *object = NULL;
  float *vertex_data_array;
  size_t count_check = 0;
  size_t vertex_count = 0;

  if (!(file_contents = file_contents_get(filename)))
    return (NULL);
  vertex_count = get_vertex_count(file_contents);
  if (!(positions = store_positions(file_contents, &count_check))) {
    printf("failure0\n");
    free(file_contents);
    file_contents = NULL;
    return (NULL);
  }
  if (!(uvs = store_uvs(file_contents, count_check))) {
    printf("failure1\n");
    free(file_contents);
    file_contents = NULL;
    return (NULL);
  }
  if (!(normals = store_normals(file_contents, count_check))) {
    printf("failure2\n");
    free(file_contents);
    file_contents = NULL;
    return (NULL);
  }
  if (!(vertex_data_array =
            create_vertex_data_array(positions, normals, uvs, vertex_count))) {
    printf("failure3\n");
    free(file_contents);
    file_contents = NULL;
    return (NULL);
  }
  if (!(object = (t_3d_object *)calloc(1, sizeof(t_3d_object))))
    return (NULL);
  object->positions_v = positions;
  object->uvs = uvs;
  object->normals = normals;
  object->vertex_data_array = vertex_data_array;
  object->vertex_count = vertex_count;
  free(file_contents);
  file_contents = NULL;
  return (object);
  (void)vertex_data_array;
  // refactor this function clean
}