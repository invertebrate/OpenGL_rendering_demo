/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_reading.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/23 16:52:18 by veilo             #+#    #+#             */
/*   Updated: 2022/01/31 18:38:11 by veilo            ###   ########.fr       */
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

size_t get_face_count(char *contents) {
  char *substr = FACE_PREFIX;

  return (substring_count(contents, substr));
}

size_t get_face_vertex_count(char *line) {
  return (substring_count(line, " "));
}

uint get_position_from_line(t_float3 *position, char *line) {
  char *coordinate;

  coordinate = strstr(line, " ");
  if (!coordinate)
    return (OBJ_FAILURE);
  coordinate++;
  if (coordinate) {
    position->x = atof(coordinate);
    line = coordinate;
  }
  coordinate = strstr(line, " ");
  if (!coordinate)
    return (OBJ_FAILURE);
  coordinate++;
  if (coordinate) {
    position->y = atof(coordinate);
    line = coordinate;
  }
  coordinate = strstr(line, " ");
  if (!coordinate)
    return (OBJ_FAILURE);
  coordinate++;
  if (coordinate) {
    position->z = atof(coordinate);
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

SDL_bool get_face_vertex_from_line(uint *vertex, char *line) {
  int temp = 0;

  bzero(vertex, sizeof(uint) * 3);
  for (int i = 0; i < 3; i++) {
    if (!(temp = atoi(line)))
      return (SDL_FALSE);
    else
      vertex[i] = temp;
    if (!(line = strstr(line, "/")))
      return (SDL_FALSE);
    else
      line++;
  }
  return (SDL_TRUE);
}

void print_face_vertices(t_face *face, size_t count) {
  for (uint i = 0; i < count; i++) {
    printf("%u/%u/%u\t", face->vertices[i].x, face->vertices[i].y,
           face->vertices[i].z);
  }
  printf("\n");
}

uint get_face_from_line(t_face *face, char *line) {
  char *index;
  uint temp[3] = {0, 0, 0};
  size_t face_vertex_count = 0;

  face_vertex_count = get_face_vertex_count(line);
  if (face_vertex_count > 8)
    return (OBJ_FAILURE);
  bzero(face, sizeof(t_face));
  for (size_t i = 0; i < face_vertex_count; i++) {
    index = strstr(line, " ");
    if (!index)
      return (OBJ_FAILURE);
    index++;
    if (index) {
      get_face_vertex_from_line(temp, index);
      face->vertices[i].x = temp[0];
      face->vertices[i].y = temp[1];
      face->vertices[i].z = temp[2];
      line = index;
    }
  }
  face->vertex_count = face_vertex_count;
  print_face_vertices(face, face_vertex_count);
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
      if (!get_position_from_line(&positions[i], line_token))
        return (NULL);
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
  if (!(contents = strstr(contents, VERTEX_PREFIX)))
    return (NULL);
  if (!(contents_copy_p = strdup(contents)))
    return (NULL);
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

char *parse_faces(char *contents_copy_f, t_face *faces, uint f_count) {
  char *line_token = NULL;
  line_token = strtok(contents_copy_f, "\n");

  for (uint i = 0; i < f_count; i++) {
    if (!contents_copy_f)
      break;
    else {
      if (!get_face_from_line(&faces[i], line_token))
        return (NULL);
      line_token = strtok(NULL, "\n");
      contents_copy_f = strstr(contents_copy_f, FACE_PREFIX);
    }
  }
  return (contents_copy_f);
}

// check if face is triangle or quad
// if triangle, store triangle
// if quad, split into triangles by:
// 1, 2, 3 form one triangle
// 2, 3 ,1 form the other

t_face *store_faces(char *contents) {
  t_face *faces = NULL;
  char *contents_copy_f = NULL;
  size_t f_count = 0;
  // size_t t_count = 0;

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
    free(contents_copy_f);
    contents_copy_f = NULL;
    free(faces);
    faces = NULL;
    return (NULL);
  };
  bzero(&faces[f_count], sizeof(t_face));
  free(contents_copy_f);
  contents_copy_f = NULL;
  return (faces);
}

size_t get_triangle_count(t_face *faces) {
  uint i = 0;
  size_t triangle_count = 0;

  while (faces[i].vertex_count > 2) {
    triangle_count += faces[i].vertex_count - 2;
    i++;
  }
  return (triangle_count);
}

/*
**  Triangulates n-gons into a triangle fan.
*/
// IDEA: triangulate into uint triplets that later determine the vertex
// attribute array
uint *triangulate_faces(t_face *faces) {
  uint *triangles;
  size_t triangle_count = 0;
  uint face_index = 0;
  uint triangle_index = 0;
  uint v_count = 0;
  uint i = 0;

  triangle_count = get_triangle_count(faces);
  if (!(triangles = (uint *)calloc(triangle_count, sizeof(uint) * 3)))
    return (NULL);
  while (1) {
    if ((v_count = faces[face_index].vertex_count) > 2) {
      i = 0;
      while (i < v_count - 2) {
        triangles[triangle_index] = faces[face_index].vertices[0].x;
        triangles[triangle_index + 1] = faces[face_index].vertices[(i + 1)].x;
        triangles[triangle_index + 2] = faces[face_index].vertices[(i + 2)].x;
        i += 1;
        triangle_index += 3;
      }
    } else {
      break;
    }
    face_index++;
  }
  return (triangles);
}

void objr_delete(void *data) {
  if (data != NULL) {
    free(data);
    data = NULL;
  }
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

// SDL_bool check_index_bounds(t_float3 *normals, t_float2 *uvs, size_t count,
//                             t_face *faces) {

//                             }

void reorder_attributes(t_float3 *normals, t_float2 *uvs, size_t count,
                        t_face *faces) {
  t_float3 *normals_copy = NULL;
  t_float2 *uvs_copy = NULL;

  normals_copy = (t_float3 *)calloc(count, sizeof(t_float3));
  memcpy(normals_copy, normals, sizeof(t_float3) * count);
  uvs_copy = (t_float2 *)calloc(count, sizeof(t_float2));
  memcpy(uvs_copy, uvs, sizeof(t_float2) * count);
  // if (!(check_index_bounds(normals, uvs, count, faces))) {
  // };
  // for (uint i = 0; faces[i].vertex_count > 2; i++) {
  //   for (uint k = 0; k < faces[i].vertex_count; k++) {
  //     uvs[faces[i].vertices[k].x % count - 1].u =
  //         uvs_copy[faces[i].vertices[k].y % count - 1].u;
  //     uvs[faces[i].vertices[k].x % count - 1].v =
  //         uvs_copy[faces[i].vertices[k].y % count - 1].v;
  //     normals[faces[i].vertices[k].x % count - 1].x =
  //         normals_copy[faces[i].vertices[k].z % count - 1].x;
  //     normals[faces[i].vertices[k].x % count - 1].y =
  //         normals_copy[faces[i].vertices[k].z % count - 1].y;
  //     normals[faces[i].vertices[k].x % count - 1].z =
  //         normals_copy[faces[i].vertices[k].z % count - 1].z;
  //   }
  // }
  (void)faces;
}

float *create_vertex_data_array(t_float3 *positions, t_float3 *normals,
                                t_float2 *uvs, size_t count, t_face *faces) {
  float *vertex_data_array;
  int offset = VERTEX_STRIDE_PUVN / 4;
  int offset_uv = 2;
  int offset_normal = offset_uv + 3;

  // reorder_attributes(normals, uvs, count, faces);
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
  objr_delete(positions);
  objr_delete(normals);
  objr_delete(uvs);
  return (vertex_data_array);
}

void *object_creation_error(char *filename, char *file_contents,
                            char *error_msg) {

  printf("ERROR: Object reading failed %s, stage; %s failed.", filename,
         error_msg);
  objr_delete(file_contents);
  return (NULL);
}

t_3d_object *obj_read_from_file(char *filename) {
  char *file_contents = NULL;
  t_float3 *positions = NULL;
  t_float2 *uvs = NULL;
  t_float3 *normals = NULL;
  float *vertex_data_array = NULL;
  t_face *faces = NULL;
  t_3d_object *object = NULL;
  size_t count_check = 0;
  size_t vertex_count = 0;

  if (!(file_contents = file_contents_get(filename)))
    return (NULL);
  vertex_count = get_vertex_count(file_contents);
  if (!(positions = store_positions(file_contents, &count_check)))
    return (object_creation_error(filename, file_contents,
                                  "Vertex Position reading"));
  if (!(uvs = store_uvs(file_contents, count_check)))
    return (
        object_creation_error(filename, file_contents, "Vertex UV reading"));
  if (!(normals = store_normals(file_contents, count_check)))
    return (object_creation_error(filename, file_contents,
                                  "Vertex Normal reading"));
  // change uv and normal array order
  if (!(vertex_data_array = create_vertex_data_array(positions, normals, uvs,
                                                     vertex_count, faces)))
    return (object_creation_error(filename, file_contents,
                                  "Vertex data array creation"));
  if (!(faces = store_faces(file_contents)))
    return (object_creation_error(filename, file_contents, "Face reading"));
  if (!(object = (t_3d_object *)calloc(1, sizeof(t_3d_object))))
    return (object_creation_error(filename, file_contents,
                                  "Object memory allocation"));
  object->vertex_data_array = vertex_data_array;
  object->vertex_count = vertex_count;
  object->triangles = triangulate_faces(faces);
  free(file_contents);
  file_contents = NULL;
  return (object);
}
// TODO: change triangulation to triangulate into uint triplets
// change create vertex attribute array to have all vertices in order according
// to the triplet array
//->results in index array being trivial 1.2.3.4.5...
//->and VAA to have the different permutations of the vertices (normal, uv etc)
// determined by the obj file face descriptions