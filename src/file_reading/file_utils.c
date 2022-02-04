/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 16:28:59 by veilo             #+#    #+#             */
/*   Updated: 2022/02/02 16:40:52 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "file_utils.h"

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

/**
**  Reads the file into void*.
**/
void *file_contents_get(char *filename, size_t *file_size) {
  FILE *fptr = NULL;
  char *contents = NULL;

  *file_size = file_size_get(filename);
  if (!(contents = (char *)calloc(*file_size + 1, sizeof(char))))
    return (NULL);
  fptr = fopen(filename, "r");
  size_t r = fread(contents, sizeof(char), *file_size, fptr);
  fclose(fptr);
  (void)r;
  return (contents);
}
