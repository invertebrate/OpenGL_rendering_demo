/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 17:36:16 by veilo             #+#    #+#             */
/*   Updated: 2022/04/15 15:31:45 by veilo            ###   ########.fr       */
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

void lm_vec3_print(float *vec) {
  printf("vec3:\n");
  printf("%f\t%f\t%f\n", vec[0], vec[1], vec[2]);
  printf(":endvec3\n");
}

void lm_vec4_print(float *vec) {
  printf("vec4:\n");
  printf("%f\t%f\t%f\t%f\n", vec[0], vec[1], vec[2], vec[3]);
  printf(":endvec4\n");
}

void lm_mat4_identity(float *outmat) {
  memset(outmat, 0, sizeof(float) * 16);
  outmat[0] = 1.0;
  outmat[5] = 1.0;
  outmat[10] = 1.0;
  outmat[15] = 1.0;
}

void lm_vec3_sub(float *v1, float *v2, float *outvec) {
  float res[3] = {0, 0, 0};

  for (int i = 0; i < 3; i++)
    res[i] = v1[i] - v2[i];
  memcpy(outvec, res, sizeof(res));
}

void lm_vec2_sub(float *v1, float *v2, float *outvec) {
  float res[2] = {0, 0};

  for (int i = 0; i < 2; i++)
    res[i] = v1[i] - v2[i];
  memcpy(outvec, res, sizeof(res));
}

void lm_vec3_add(float *v1, float *v2, float *outvec) {
  float res[3] = {0, 0, 0};

  for (int i = 0; i < 3; i++)
    res[i] = v1[i] + v2[i];
  memcpy(outvec, res, sizeof(res));
}

void lm_vec2_add(float *v1, float *v2, float *outvec) {
  float res[2] = {0, 0};

  for (int i = 0; i < 2; i++)
    res[i] = v1[i] + v2[i];
  memcpy(outvec, res, sizeof(res));
}

void lm_mat4_scale(float *inmat, float sx, float sy, float sz, float *outmat) {
  outmat[0] = inmat[0] * sx;
  outmat[5] = inmat[5] * sy;
  outmat[10] = inmat[10] * sz;
}

void lm_mat4_transpose(float *inmat, float *outmat) {
  float temp[16];

  memcpy(temp, inmat, sizeof(temp));
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      outmat[i * 4 + j] = temp[j * 4 + i];
    }
  }
}

void lm_mat4_projection(float fovx, float fovy, float near, float far,
                        float *outmat, int transpose) {
  memset(outmat, 0, sizeof(float) * 16);
  float scalex = 1 / tan(fovx * 0.5 * M_PI / 180);
  float scaley = 1 / tan(fovy * 0.5 * M_PI / 180);
  outmat[0] = scalex;
  outmat[5] = scaley;
  outmat[10] = -(far / (far - near));
  outmat[11] = -((far * near) / (far - near));
  outmat[14] = -1;
  if (transpose) {
    lm_mat4_transpose(outmat, outmat);
  }
}

void lm_mat4_ortho(float far, float near, float top, float bottom, float left,
                   float right, float *outmat, int transpose) {
  memset(outmat, 0, sizeof(float) * 16);
  outmat[0] = 2 / (right - left);
  outmat[4 + 1] = 2 / (top - bottom);
  outmat[8 + 2] = -2 / (far - near);
  outmat[12 + 3] = 1;
  outmat[12 + 0] = -(right + left) / (right - left);
  outmat[12 + 1] = -(top + bottom) / (top - bottom);
  outmat[12 + 2] = -(far + near) / (far - near);
  if (transpose) {
    lm_mat4_transpose(outmat, outmat);
  }
}

void lm_vec3_scale(float *invec, float scale, float *outvec) {
  for (int i = 0; i < 3; i++) {
    outvec[i] = scale * invec[i];
  }
}

void lm_vec4_scale(float *invec, float scale, float *outvec) {
  for (int i = 0; i < 4; i++) {
    outvec[i] = scale * invec[i];
  }
}

void lm_vec3_normalize(float *invec, float *outvec) {
  float length = 0;

  length =
      sqrt(invec[0] * invec[0] + invec[1] * invec[1] + invec[2] * invec[2]);
  if (length < __FLT_EPSILON__)
    length = 1;
  outvec[0] = invec[0] / length;
  outvec[1] = invec[1] / length;
  outvec[2] = invec[2] / length;
}

void lm_mat4_create_rotmat(float *rotmat, float *axis, float angle) {

  lm_vec3_normalize(axis, axis);
  rotmat[0] = cos(angle) + (axis[0] * axis[0]) * (1.0 - cos(angle));
  rotmat[1] = axis[1] * axis[0] * (1.0 - cos(angle)) + (axis[2] * sin(angle));
  rotmat[2] = axis[2] * axis[0] * (1.0 - cos(angle)) - (axis[1] * sin(angle));
  rotmat[3] = 0;
  rotmat[4] = axis[0] * axis[1] * (1.0 - cos(angle)) - (axis[2] * sin(angle));
  rotmat[5] = cos(angle) + (axis[1] * axis[1]) * (1.0 - cos(angle));
  rotmat[6] = axis[2] * axis[1] * (1.0 - cos(angle)) + (axis[0] * sin(angle));
  rotmat[7] = 0;
  rotmat[8] = axis[0] * axis[2] * (1.0 - cos(angle)) + (axis[1] * sin(angle));
  rotmat[9] = axis[1] * axis[2] * (1.0 - cos(angle)) - (axis[0] * sin(angle));
  rotmat[10] = cos(angle) + (axis[2] * axis[2]) * (1.0 - cos(angle));
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

float lm_vec3_dot(float *invec1, float *invec2) {
  return (invec1[0] * invec2[0] + invec1[1] * invec2[1] +
          invec1[2] * invec2[2]);
}

void lm_mat4vec4_mul(float *inmat, float *invec, float *outvec) {
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
  lm_mat4vec4_mul(rotmat, tempvec, tempvec);
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
    }
  }
  memcpy(outmat, res, sizeof(res));
}

void lm_mat4_set_position(float *pos, float *outmat) {
  outmat[12] = pos[0];
  outmat[13] = pos[1];
  outmat[14] = pos[2];
}

void lm_mat4_translate(float *inmat, float *translation, float *outmat) {
  outmat[12] = inmat[12] + translation[0];
  outmat[13] = inmat[13] + translation[1];
  outmat[14] = inmat[14] + translation[2];
}

void lm_mat4_add(float *mat1, float *mat2, float *outmat) {
  float res[16];

  for (int i = 0; i < 16; i++) {
    res[i] = mat1[i] + mat2[i];
  }
  memcpy(outmat, res, sizeof(res));
}

void lm_mat4_lookat(float *pos, float *dir, float *right, float *up,
                    float *outmat) {
  float temp[16];

  lm_mat4_identity(outmat);
  lm_mat4_identity(temp);
  for (int i = 0; i < 3; i++) {
    outmat[i * 4 + 0] = right[i];
    outmat[i * 4 + 1] = up[i];
    outmat[i * 4 + 2] = dir[i];
  }
  memcpy(temp + 12, pos, sizeof(float) * 3);
  lm_mat4_multiply(outmat, temp, outmat);
}

void lm_vec3_cross(float *invec1, float *invec2, float *outvec) {
  outvec[2] = invec1[0] * invec2[1] - invec1[1] * invec2[0];
  outvec[1] = -(invec1[0] * invec2[2] - invec1[2] * invec2[0]);
  outvec[0] = invec1[1] * invec2[2] - invec1[2] * invec2[1];
}

void lm_mat4_topleftmat3(float *inmat, float *outmat) {
  float tempmat[16];

  memcpy(tempmat, inmat, sizeof(tempmat));
  for (int i = 0; i < 3; i++) {
    tempmat[i + 12] = 0;
  }
  tempmat[15] = 1;
  memcpy(outmat, tempmat, sizeof(tempmat));
}

float lm_vec3_length(float *invec) { return (sqrt(lm_vec3_dot(invec, invec))); }

float lm_vec3_angle(float *invec1, float *invec2) {
  return (acos(lm_vec3_dot(invec1, invec2) /
               (lm_vec3_length(invec1) * lm_vec3_length(invec2))));
}

/*
**  Finds a vector perpendicular to invec on the plane defined by invec and
**  guide.
*/
void lm_vec3_find_perp(float *invec, float *guide, float *outvec) {
  float temp[3] = {0.0, 0.0, 0.0};
  float rotated_guide[3] = {0.0, 0.0, 0.0};

  lm_vec3_cross(invec, guide, temp);
  lm_vec3_normalize(temp, temp);
  lm_vec3_rotate(guide, temp, -(lm_vec3_angle(invec, guide) - (M_PI / 2)),
                 rotated_guide);
  lm_vec3_normalize(rotated_guide, rotated_guide);
  memcpy(outvec, rotated_guide, sizeof(rotated_guide));
}
