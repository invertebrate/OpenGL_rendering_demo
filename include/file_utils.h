/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 16:29:25 by veilo             #+#    #+#             */
/*   Updated: 2022/02/20 17:29:22 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t file_size_get(char *filename);
size_t substring_count(char *contents, char *substr);
void *file_contents_get(char *filename, size_t *file_size,
                        unsigned int end_padding);

#endif