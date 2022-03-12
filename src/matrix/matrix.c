/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 17:36:16 by veilo             #+#    #+#             */
/*   Updated: 2022/03/12 14:39:01 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lm_matrix.h"
#include <stdio.h>
#include <string.h>

void lm_mat4_print(float *mat) {
  printf("mat4:\n");
  for (int i = 0; i < 4; i++) {
    printf("%f\t%f\t%f\t%f\n", mat[i + 4 * 0], mat[i + 4 * 1], mat[i + 4 * 2],
           mat[i + 4 * 3]);
  }
  printf(":endmat4\n");
}

void lm_mat4_identity(float *outmat) {
  memset(outmat, 0, sizeof(float) * 16);
  outmat[0] = 1.0;
  outmat[5] = 1.0;
  outmat[10] = 1.0;
  outmat[15] = 1.0;
}

void lm_mat4_scale(float *inmat, float sx, float sy, float sz, float *outmat) {
  outmat[0] = inmat[0] * sx;
  outmat[5] = inmat[5] * sy;
  outmat[10] = inmat[10] * sz;
}

void lm_mat4_projection(float fovx, float fovy, float near, float far,
                        float *outmat) {
  memset(outmat, 0, sizeof(float) * 16);
  float scalex = 1 / tan(fovx * 0.5 * M_PI / 180);
  float scaley = 1 / tan(fovy * 0.5 * M_PI / 180);
  outmat[0] = scalex;
  outmat[5] = scaley;
  outmat[10] = -(far / (far - near));
  outmat[11] = -((far * near) / (far - near));
  outmat[14] = -1;
}

void lm_vec3_normalize(float *invec, float *outvec) {
  float length = 0;

  length =
      sqrt(invec[0] * invec[0] + invec[1] * invec[1] + invec[2] * invec[2]);
  outvec[0] = invec[0] / length;
  outvec[1] = invec[1] / length;
  outvec[2] = invec[2] / length;
}

void lm_mat4_create_rotmat(float *rotmat, float *axis, float angle) {

  lm_vec3_normalize(axis, axis);
  rotmat[0] = cos(angle) + (axis[0] * axis[0]) * (1.0 - cos(angle)); // yz
  rotmat[1] = axis[1] * axis[0] * (1.0 - cos(angle)) + (axis[2] * sin(angle));
  rotmat[2] = axis[2] * axis[0] * (1.0 - cos(angle)) - (axis[1] * sin(angle));
  rotmat[3] = 0;
  rotmat[4] = axis[0] * axis[1] * (1.0 - cos(angle)) - (axis[2] * sin(angle));
  rotmat[5] = cos(angle) + (axis[1] * axis[1]) * (1.0 - cos(angle)); // xz
  rotmat[6] = axis[2] * axis[1] * (1.0 - cos(angle)) + (axis[0] * sin(angle));
  rotmat[7] = 0;
  rotmat[8] = axis[0] * axis[2] * (1.0 - cos(angle)) + (axis[1] * sin(angle));
  rotmat[9] = axis[1] * axis[2] * (1.0 - cos(angle)) - (axis[0] * sin(angle));
  rotmat[10] = cos(angle) + (axis[2] * axis[2]) * (1.0 - cos(angle)); // yx
  rotmat[11] = 0;
  rotmat[12] = 0;
  rotmat[13] = 0;
  rotmat[14] = 0;
  rotmat[15] = 1.0;
}

float lm_vec4_dot(float *invec1, float *invec2) {
  return (invec1[0] * invec2[0] + invec1[1] * invec2[1] +
          invec1[2] * invec2[2] + invec1[3] * invec2[3]);
}

void lm_mat4vec4_mul(float *invec, float *inmat, float *outvec) {
  float res[4];

  for (int i = 0; i < 4; i++) {
    res[i] = lm_vec4_dot(invec, (float[4]){inmat[0 + i], inmat[4 + i],
                                           inmat[8 + i], inmat[12 + i]});
  }
  memcpy(outvec, res, sizeof(res));
}

void lm_vec3_rotate(float *invec, float *axis, float angle, float *outvec) {
  float tempvec[4] = {0, 0, 0, 1};
  float rotmat[16];
  lm_vec3_normalize(axis, axis);
  memcpy(tempvec, invec, sizeof(float) * 3);
  lm_mat4_create_rotmat(rotmat, axis, angle);
  lm_mat4vec4_mul(tempvec, rotmat, tempvec);
  memcpy(outvec, tempvec, sizeof(float) * 3);
}

void lm_mat4_get_column(float *inmat, float *outvec, int index) {
  for (unsigned int i = 0; i < 4; i++) {
    outvec[i] = inmat[4 * index + i];
  }
}

void lm_mat4_get_row(float *inmat, float *outvec, int index) {
  for (unsigned int i = 0; i < 4; i++) {
    outvec[i] = inmat[i * 4 + index];
  }
}

void lm_mat4_multiply(float *inmat1, float *inmat2, float *outmat) {
  float col[4] = {0, 0, 0, 0};
  float row[4] = {0, 0, 0, 0};
  float res[16];

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      lm_mat4_get_row(inmat1, row, j);
      lm_mat4_get_column(inmat2, col, i);
      res[i * 4 + j] = lm_vec4_dot(row, col);
      // printf("outmat[%d] = row: %d column: %d\n", i * 4 + j, j, i);
    }
  }
  // int a = 0;
  // int b = 0;

  // for (int i = 0; i < 16; i++) {
  //   a = (i / 4) * 4;
  //   b = (i % 4);
  //   res[i] = (inmat1[a] * inmat2[b]) + (inmat1[a + 1] * inmat2[b + 4]) +
  //            (inmat1[a + 2] * inmat2[b + 8]) + (inmat1[a + 3] * inmat2[b +
  //            12]);
  // }
  memcpy(outmat, res, sizeof(res));
}

void lm_mat4_translate(float *inmat, float *translation, float *outmat) {
  outmat[12] = inmat[12] + translation[0];
  outmat[13] = inmat[12] + translation[1];
  outmat[14] = inmat[12] + translation[2];
}

void lm_mat4_add(float *mat1, float *mat2, float *outmat) {
  for (int i = 0; i < 16; i++) {
    outmat[i] = mat1[i] + mat2[i];
  }
}