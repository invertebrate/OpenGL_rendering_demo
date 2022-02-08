/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 17:36:16 by veilo             #+#    #+#             */
/*   Updated: 2022/02/08 17:44:26 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lm_matrix.h"
#include <string.h>

void lm_mat4_create_rotmat(float *rotmat, float *axis, float angle) {
  rotmat[0] = cos(angle) + (axis[0] * axis[0]) * (1 - cos(angle));
  rotmat[1] = axis[1] * axis[0] * (1 - cos(angle)) + axis[2] * sin(angle);
  rotmat[2] = axis[2] * axis[0] * (1 - cos(angle)) - axis[1] * sin(angle);
  rotmat[3] = 0;

  rotmat[4] = axis[0] * axis[1] * (1 - cos(angle)) - axis[2] * sin(angle);
  rotmat[5] = cos(angle) + (axis[1] * axis[1]) * (1 - cos(angle));
  rotmat[6] = axis[2] * axis[1] * (1 - cos(angle)) + axis[0] * sin(angle);
  rotmat[7] = 0;

  rotmat[8] = axis[0] * axis[2] * (1 - cos(angle)) + axis[1] * sin(angle);
  rotmat[9] = axis[1] * axis[2] * (1 - cos(angle)) - axis[0] * sin(angle);
  rotmat[10] = cos(angle) + (axis[2] * axis[2]) * (1 - cos(angle));
  rotmat[11] = 0;

  rotmat[12] = 0;
  rotmat[13] = 0;
  rotmat[14] = 0;
  rotmat[15] = 1;
}

float lm_vec4_dot(float *invec1, float *invec2) {
  return (invec1[0] * invec2[0] + invec1[1] * invec2[1] +
          invec1[2] * invec2[2] + invec1[3] * invec2[3]);
}

void lm_mat4vec4_mul(float *invec, float *inmat, float *outvec) {
  outvec[0] = lm_vec4_dot(invec, inmat);
  outvec[1] = lm_vec4_dot(invec, inmat + 4);
  outvec[2] = lm_vec4_dot(invec, inmat + 8);
  outvec[3] = lm_vec4_dot(invec, inmat + 12);
}

void lm_vec3_rotate(float *invec, float *axis, float angle, float *outvec,
                    float *rotmat) {
  float tempvec[4] = {0, 0, 0, 1};

  memcpy(tempvec, invec, sizeof(float) * 3);
  lm_mat4_create_rotmat(rotmat, axis, angle);
  lm_mat4vec4_mul(tempvec, rotmat, tempvec);
  memcpy(outvec, tempvec, sizeof(float) * 3);
}