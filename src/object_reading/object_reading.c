/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_reading.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/23 16:52:18 by veilo             #+#    #+#             */
/*   Updated: 2022/01/23 18:54:51 by veilo            ###   ########.fr       */
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
  char *substr = "\nv ";

  return (substring_count(contents, substr));
}

t_3d_object *obj_read_from_file(char *filename) {
  FILE *fptr;
  char *file_contents;
  size_t file_size = 0;

  file_size = file_size_get(filename);
  file_contents = (char *)malloc(sizeof(char) * file_size);
  fptr = fopen(filename, "r");
  size_t r = fread(file_contents, sizeof(char), file_size, fptr);
  // each line is v f f f -> %s %s %s %s
  // "\nv " count to get vertex count
  // use ret = strstr(contents, "\nv ") to get vertex count
  printf("file contents in string[]:\n[%s]\n", file_contents);
  fclose(fptr);
  size_t count = get_vertex_count(file_contents);
  printf("vertex count: %lu\n", count);
  (void)r;
  return (NULL);
}