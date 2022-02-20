/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_reading.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/23 16:52:18 by veilo             #+#    #+#             */
/*   Updated: 2022/02/20 16:14:34 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "object_reading.h"
#include "file_utils.h"
#include "h_opengl.h"
#include "lm_matrix.h"

void objr_delete(void *data) {
  if (data != NULL) {
    free(data);
    data = NULL;
  }
}

void objr_delete_many(void **ptrs, size_t count) {
  for (size_t i = 0; i < count; i++) {
    objr_delete(ptrs[i]);
  }
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

size_t get_face_count(char *contents) {
  char *substr = FACE_PREFIX;

  return (substring_count(contents, substr));
}

size_t get_face_vertex_count(char *line) {
  return (substring_count(line, " "));
}

unsigned int get_position_from_line(t_float3 *position, char *line) {
  char *coordinate;

  coordinate = strstr(line, " ");
  if (!coordinate)
    return (OBJ_FAILURE);
  coordinate++;
  if (coordinate) {
    position->x = atof(coordinate);
    line = coordinate;
  }
  if (*coordinate == ' ' || !(coordinate = strstr(line, " ")))
    return (OBJ_FAILURE);
  coordinate++;
  if (coordinate) {
    position->y = atof(coordinate);
    line = coordinate;
  }
  if (*coordinate == ' ' || !(coordinate = strstr(line, " ")))
    return (OBJ_FAILURE);
  coordinate++;
  if (coordinate) {
    position->z = atof(coordinate);
    line = coordinate;
  }
  return (OBJ_SUCCESS);
}

unsigned int get_uv_from_line(t_float2 *uv, char *line) {
  char *coordinate;

  coordinate = strstr(line, " ");
  if (!coordinate)
    return (OBJ_FAILURE);
  coordinate++;
  if (coordinate) {
    uv->u = atof(coordinate);
    line = coordinate;
  }
  if (*coordinate == ' ' || !(coordinate = strstr(line, " ")))
    return (OBJ_FAILURE);
  coordinate++;
  if (coordinate) {
    uv->v = atof(coordinate);
    line = coordinate;
  }
  return (OBJ_SUCCESS);
}

unsigned int get_normal_from_line(t_float3 *normal, char *line) {
  char *coordinate;

  coordinate = strstr(line, " ");
  if (!coordinate)
    return (OBJ_FAILURE);
  coordinate++;
  if (coordinate) {
    normal->x = atof(coordinate);
    line = coordinate;
  }
  if (*coordinate == ' ' || !(coordinate = strstr(line, " ")))
    return (OBJ_FAILURE);
  coordinate++;
  if (coordinate) {
    normal->y = atof(coordinate);
    line = coordinate;
  }
  if (*coordinate == ' ' || !(coordinate = strstr(line, " ")))
    return (OBJ_FAILURE);
  coordinate++;
  if (coordinate) {
    normal->z = atof(coordinate);
    line = coordinate;
  }
  return (OBJ_SUCCESS);
}

void get_face_vertex_from_line(unsigned int *vertex, char *line) {
  int temp = 0;

  for (int i = 0; i < 3; i++) {
    if (!(temp = atoi(line)))
      return;
    else
      vertex[i] = temp;
    if (!(line = strstr(line, "/")))
      return;
    else
      line++;
  }
}

void print_face_vertices(t_face *face, size_t count) {
  for (unsigned int i = 0; i < count; i++) {
    printf("%u/%u/%u\t", face->vertices[i].x, face->vertices[i].y,
           face->vertices[i].z);
  }
  printf("\n");
}

/*
**  Reads one face from a char * line. Initializes members to 1 to avoid
**  indexing issues later.
*/
unsigned int get_face_from_line(t_face *face, char *line) {
  char *index;
  unsigned int vertex[3] = {1, 1, 1};
  size_t face_vertex_count = 0;

  face_vertex_count = get_face_vertex_count(line);
  if (face_vertex_count > 8)
    return (OBJ_FAILURE);
  for (size_t i = 0; i < face_vertex_count; i++) {
    index = strstr(line, " ");
    if (!index)
      return (OBJ_FAILURE);
    index++;
    if (index) {
      get_face_vertex_from_line(vertex, index);
      face->vertices[i].x = vertex[0];
      face->vertices[i].y = vertex[1];
      face->vertices[i].z = vertex[2];
      line = index;
    }
  }
  face->vertex_count = face_vertex_count;
  return (OBJ_SUCCESS);
}

void get_center_point(t_float3 *positions, float *point) {
  float bounds_x[2] = {FLT_MAX, -FLT_MAX};
  float bounds_y[2] = {FLT_MAX, -FLT_MAX};
  float bounds_z[2] = {FLT_MAX, -FLT_MAX};
  for (unsigned int i = 1; i < positions[0].x; i++) {
    if (positions[i].x > bounds_x[1])
      bounds_x[1] = positions[i].x;
    if (positions[i].x < bounds_x[0])
      bounds_x[0] = positions[i].x;
    if (positions[i].y > bounds_y[1])
      bounds_y[1] = positions[i].y;
    if (positions[i].y < bounds_y[0])
      bounds_y[0] = positions[i].y;
    if (positions[i].z > bounds_z[1])
      bounds_z[1] = positions[i].z;
    if (positions[i].z < bounds_z[0])
      bounds_z[0] = positions[i].z;
  }
  point[0] = (bounds_x[0] + bounds_x[1]) / 2;
  point[1] = (bounds_y[0] + bounds_y[1]) / 2;
  point[2] = (bounds_z[0] + bounds_z[1]) / 2;
}

char *parse_positions(char *contents_copy_p, t_float3 *positions,
                      unsigned int p_count) {
  char *line_token = NULL;
  line_token = strtok(contents_copy_p, "\n");

  for (unsigned int i = 0; i < p_count; i++) {
    if (!line_token)
      break;
    else {
      if (!strstr(line_token, VERTEX_PREFIX)) {
        line_token = strtok(NULL, "\n");
        i--;
        continue;
      }
      get_position_from_line(&positions[i], line_token);
      line_token = strtok(NULL, "\n");
    }
  }
  return (contents_copy_p);
}

t_float3 *store_positions(char *contents) {
  t_float3 *positions = NULL;
  char *contents_copy_p = NULL;
  size_t p_count = 0;

  p_count = get_vertex_count(contents);
  if (!(positions = (t_float3 *)calloc(p_count + 1, sizeof(t_float3))))
    return (NULL);
  positions[0].x = p_count;
  if (!(contents = strstr(contents, VERTEX_PREFIX)))
    return (positions);
  if (!(contents_copy_p = strdup(contents)))
    return (positions);
  if (!(parse_positions(contents_copy_p, positions + 1, p_count))) {
    objr_delete(contents_copy_p);
    return (positions);
  };
  objr_delete(contents_copy_p);
  return (positions);
}

char *parse_uvs(char *contents_copy_uv, t_float2 *uvs, unsigned int uv_count) {
  char *line_token = NULL;
  line_token = strtok(contents_copy_uv, "\n");

  for (unsigned int i = 0; i < uv_count; i++) {
    if (!line_token)
      break;
    else {
      if (!strstr(line_token, UV_PREFIX)) {
        line_token = strtok(NULL, "\n");
        i--;
        continue;
      }
      get_uv_from_line(&uvs[i], line_token);
      line_token = strtok(NULL, "\n");
    }
  }
  return (contents_copy_uv);
}

t_float2 *store_uvs(char *contents) {
  t_float2 *uvs = NULL;
  char *contents_copy_uv = NULL;
  size_t uv_count = 0;

  uv_count = get_uv_count(contents);
  if (!(uvs = (t_float2 *)calloc(uv_count + 1, sizeof(t_float2))))
    return (NULL);
  uvs[0].u = uv_count;
  if (!(contents = strstr(contents, UV_PREFIX))) {
    return (uvs);
  }
  if (!(contents_copy_uv = strdup(contents))) {
    return (uvs);
  }
  if (!(parse_uvs(contents_copy_uv, uvs + 1, uv_count))) {
    objr_delete(contents_copy_uv);
    return (uvs);
  };
  objr_delete(contents_copy_uv);
  return (uvs);
}

char *parse_normals(char *contents_copy_n, t_float3 *normals,
                    unsigned int n_count) {
  char *line_token = NULL;
  line_token = strtok(contents_copy_n, "\n");

  for (unsigned int i = 0; i < n_count; i++) {
    if (!line_token)
      break;
    else {
      if (!strstr(line_token, NORMAL_PREFIX)) {
        line_token = strtok(NULL, "\n");
        i--;
        continue;
      }
      get_normal_from_line(&normals[i], line_token);
      line_token = strtok(NULL, "\n");
    }
  }
  return (contents_copy_n);
}

t_float3 *store_normals(char *contents) {
  t_float3 *normals = NULL;
  char *contents_copy_n = NULL;
  size_t n_count = 0;

  n_count = get_normal_count(contents);
  if (!(normals = (t_float3 *)calloc(n_count + 1, sizeof(t_float3))))
    return (NULL);
  normals[0].x = n_count;
  if (!(contents = strstr(contents, NORMAL_PREFIX))) {
    return (normals);
  }
  if (!(contents_copy_n = strdup(contents))) {
    return (normals);
  }
  if (!(parse_normals(contents_copy_n, normals + 1, n_count))) {
    objr_delete(contents_copy_n);
    return (normals);
  };
  objr_delete(contents_copy_n);
  return (normals);
}

char *parse_faces(char *contents_copy_f, t_face *faces, unsigned int f_count) {
  unsigned int i = 0;
  char *line_token = NULL;
  line_token = strtok(contents_copy_f, "\n");
  while (i < f_count) {
    if (!line_token)
      break;
    else {
      if (!strstr(line_token, FACE_PREFIX)) {
        line_token = strtok(NULL, "\n");
        continue;
      }
    }
    get_face_from_line(faces + i, line_token);
    line_token = strtok(NULL, "\n");
    i++;
  }
  return (contents_copy_f);
}

t_face *store_faces(char *contents) {
  t_face *faces = NULL;
  char *contents_copy_f = NULL;
  size_t f_count = 0;

  f_count = get_face_count(contents);
  if (!(contents = strstr(contents, FACE_PREFIX))) {
    return (NULL);
  }
  if (!(contents_copy_f = strdup(contents))) {
    return (NULL);
  }
  if (!(faces = (t_face *)calloc(f_count + 1, sizeof(t_face))))
    return (NULL);
  if (!(parse_faces(contents_copy_f, faces, f_count))) {
    objr_delete(contents_copy_f);
    objr_delete(faces);
    return (NULL);
  }
  bzero(&faces[f_count], sizeof(t_face));
  objr_delete(contents_copy_f);
  return (faces);
}

size_t get_triangle_count(t_face *faces) {
  unsigned int i = 0;
  size_t triangle_count = 0;

  while (faces[i].vertex_count > 2) {
    triangle_count += faces[i].vertex_count - 2;
    i++;
  }
  return (triangle_count);
}

static void clamp_face_indices(t_face *faces, t_float3 *positions,
                               t_float2 *uvs, t_float3 *normals) {
  unsigned int i = 0;

  while (faces[i].vertex_count > 2) {
    for (unsigned int k = 0; k < faces[i].vertex_count; k++) {
      if ((float)faces[i].vertices[k].x - positions[0].x > 0.001)
        faces[i].vertices[k].x = floor(positions[0].x);
      if ((float)faces[i].vertices[k].y - uvs[0].u > 0.001)
        faces[i].vertices[k].y = floor(uvs[0].u);
      if ((float)faces[i].vertices[k].z - normals[0].x > 0.001)
        faces[i].vertices[k].z = floor(normals[0].x);
    }
    i++;
  }
}

/*
**  Triangulates n-gons into a triangle fan.
*/
t_uint3 *triangulate_faces(t_face *faces, size_t *triangle_count) {
  t_uint3 *tvertices;
  unsigned int face_index = 0;
  unsigned int tvertex_index = 0;
  unsigned int v_count = 0;
  unsigned int i = 0;
  *triangle_count = get_triangle_count(faces);
  if (!(tvertices = (t_uint3 *)calloc(*triangle_count, sizeof(t_uint3) * 3)))
    return (NULL);
  while (1) {
    if ((v_count = faces[face_index].vertex_count) > 2) {
      i = 0;
      while (i < v_count - 2) {
        tvertices[tvertex_index].x = faces[face_index].vertices[0].x;
        tvertices[tvertex_index].y = faces[face_index].vertices[0].y;
        tvertices[tvertex_index].z = faces[face_index].vertices[0].z;
        tvertices[tvertex_index + 1].x = faces[face_index].vertices[i + 1].x;
        tvertices[tvertex_index + 1].y = faces[face_index].vertices[i + 1].y;
        tvertices[tvertex_index + 1].z = faces[face_index].vertices[i + 1].z;
        tvertices[tvertex_index + 2].x = faces[face_index].vertices[i + 2].x;
        tvertices[tvertex_index + 2].y = faces[face_index].vertices[i + 2].y;
        tvertices[tvertex_index + 2].z = faces[face_index].vertices[i + 2].z;
        i++;
        tvertex_index += 3;
      }
    } else {
      break;
    }
    face_index++;
  }
  // for (unsigned int i = 0; i < *triangle_count * 3; i += 3) {
  //   printf("trianges: %u/%u/%u %u/%u/%u %u/%u/%u\n", triangles[i].x,
  //          triangles[i].y, triangles[i].z, triangles[i + 1].x,
  //          triangles[i + 1].y, triangles[i + 1].z, triangles[i + 2].x,
  //          triangles[i + 2].y, triangles[i + 2].z);
  // }
  return (tvertices);
}

float *create_vertex_data_array(t_float3 *positions, t_float3 *normals,
                                t_float2 *uvs, t_uint3 *tvertices,
                                size_t triangle_count) {
  float *vertex_data_array;
  int offset = VERTEX_STRIDE_PUVN / sizeof(float);
  int offset_uv = 3;
  int offset_normal = offset_uv + 2;

  if (!(vertex_data_array =
            (float *)calloc(triangle_count * 3, VERTEX_STRIDE_PUVN))) {
  }
  if (positions == NULL || normals == NULL || uvs == NULL ||
      tvertices == NULL) {
    objr_delete_many((void *[4]){tvertices, positions, normals, uvs}, 4);
    return (NULL);
  }
  for (size_t i = 0; i < triangle_count * 3; i++) {
    vertex_data_array[(i * offset)] = positions[tvertices[i].x].x;
    vertex_data_array[(i * offset) + 1] = positions[tvertices[i].x].y;
    vertex_data_array[(i * offset) + 2] = positions[tvertices[i].x].z;
    vertex_data_array[(i * offset) + offset_uv] = uvs[tvertices[i].y].u;
    vertex_data_array[(i * offset) + offset_uv + 1] = uvs[tvertices[i].y].v;
    vertex_data_array[(i * offset) + offset_normal] = normals[tvertices[i].z].x;
    vertex_data_array[(i * offset) + offset_normal + 1] =
        normals[tvertices[i].z].y;
    vertex_data_array[(i * offset) + offset_normal + 2] =
        normals[tvertices[i].z].z;
  }
  objr_delete_many((void *[4]){tvertices, positions, normals, uvs}, 4);
  return (vertex_data_array);
}

void *object_creation_error(char *filename, char *file_contents,
                            char *error_msg) {

  printf("ERROR: Object reading failed %s, stage; %s failed.", filename,
         error_msg);
  objr_delete(file_contents);
  return (NULL);
}

float calculate_scale(t_float3 *positions) {
  float scale = 0.01;
  for (unsigned int i = 1; i < (unsigned int)positions[0].x + 1; i++) {
    if (fabs(positions[i].x) > scale)
      scale = positions[i].x;
    if (fabs(positions[i].y) > scale)
      scale = positions[i].y;
    if (fabs(positions[i].z) > scale)
      scale = positions[i].z;
  }
  if (scale < 1.0) {
    for (unsigned int i = 0; i < (unsigned int)positions[0].x + 1; i++) {
      if (fabs(positions[i].x) < scale)
        scale = fabs(positions[i].x);
      if (fabs(positions[i].y) < scale)
        scale = fabs(positions[i].y);
      if (fabs(positions[i].z) < scale)
        scale = fabs(positions[i].z);
    }
  }
  return (1 / scale);
}

t_3d_object *obj_read_from_file(char *filename) {
  char *file_contents = NULL;
  t_float3 *positions = NULL;
  t_float2 *uvs = NULL;
  t_float3 *normals = NULL;
  float *vertex_data_array = NULL;
  t_face *faces = NULL;
  t_uint3 *tvertices = NULL;
  size_t triangle_count = 0;
  t_3d_object *object = NULL;
  size_t vertex_count = 0;
  size_t file_size = 0;
  float center_point[3];
  float scale_factor = 1;

  if (!(file_contents = (char *)file_contents_get(filename, &file_size, 0)))
    return (NULL);
  vertex_count = get_vertex_count(file_contents);
  positions = store_positions(file_contents);
  get_center_point(positions, center_point);
  scale_factor = calculate_scale(positions);
  uvs = store_uvs(file_contents);
  normals = store_normals(file_contents);
  if (!(faces = store_faces(file_contents)))
    return (object_creation_error(filename, file_contents, "Face reading"));
  clamp_face_indices(faces, positions, uvs, normals);
  if (!(tvertices = triangulate_faces(faces, &triangle_count))) {
    return (
        object_creation_error(filename, file_contents, "Face triangulation"));
  }
  if (!(vertex_data_array = create_vertex_data_array(
            positions, normals, uvs, tvertices, triangle_count)))
    return (object_creation_error(filename, file_contents,
                                  "Vertex data array creation"));
  if (!(object = (t_3d_object *)calloc(1, sizeof(t_3d_object))))
    return (object_creation_error(filename, file_contents,
                                  "Object memory allocation"));
  object->vertex_data_array = vertex_data_array;
  object->vertex_count = vertex_count;
  object->triangle_count = triangle_count;
  object->scale_factor = scale_factor;
  memcpy(object->center_point, center_point, sizeof(float) * 3);
  lm_mat4_identity(object->model_matrix);
  lm_mat4_identity(object->translation);
  lm_mat4_identity(object->rotation);
  lm_mat4_identity(object->scale);
  object->shader = shader_type_default;
  objr_delete(file_contents);
  return (object);
}

// TODO: FIX READING FALSE POSITIVE WHEN A LINE HAS ONLY 2 NUMBERS BUT 2 SPACES
// CHANGE TO ACCOMODATE DIFFERENT COUNTS OF V VT VN AND THE ABSENCE<-
// VERTEX ATTRIBUTE ARRAY CREATION MAKES WRONG TRIANGLES //does it?
//->REWORK THAT
// VERTEX ARRAY FUNCTIONS MALFUNCTION WITH NON BLENDER
// EXPORTED DATA (probably weird face indices)