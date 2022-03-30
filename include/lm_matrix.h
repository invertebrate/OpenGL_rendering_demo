/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lm_matrix.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 17:37:02 by veilo             #+#    #+#             */
/*   Updated: 2022/03/30 16:20:10 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LM_MATRIX_H
#define LM_MATRIX_H

#include <math.h>

#define VECTOR_X                                                               \
  (float[3]) { 1.0, 0.0, 0.0 }
#define VECTOR_Y                                                               \
  (float[3]) { 0.0, 1.0, 0.0 }
#define VECTOR_Z                                                               \
  (float[3]) { 0.0, 0.0, 1.0 }

void lm_mat4_print(float *mat);
void lm_vec3_print(float *vec);
void lm_vec4_print(float *vec);
void lm_vec3_rotate(float *invec, float *axis, float angle, float *outvec);
void lm_mat4vec4_mul(float *inmat, float *invec, float *outvec);
float lm_vec4_dot(float *invec1, float *invec2);
float lm_vec3_dot(float *invec1, float *invec2);
void lm_vec2_add(float *v1, float *v2, float *outvec);
void lm_vec3_add(float *v1, float *v2, float *outvec);
void lm_vec2_sub(float *v1, float *v2, float *outvec);
void lm_vec3_sub(float *v1, float *v2, float *outvec);
void lm_vec3_scale(float *invec, float scale, float *outvec);
void lm_vec4_scale(float *invec, float scale, float *outvec);
void lm_mat4_create_rotmat(float *rotmat, float *axis, float angle);
void lm_mat4_projection(float fovx, float fovy, float near, float far,
                        float *outmat, int transpose);
void lm_mat4_identity(float *outmat);
void lm_mat4_set_position(float *pos, float *outmat);
void lm_vec3_normalize(float *invec, float *outvec);
void lm_mat4_scale(float *inmat, float sx, float sy, float sz, float *outmat);
void lm_mat4_multiply(float *inmat1, float *inmat2, float *outmat);
void lm_mat4_translate(float *inmat, float *translation, float *outmat);
void lm_mat4_add(float *mat1, float *mat2, float *outmat);
void lm_mat4_lookat(float *pos, float *dir, float *right, float *up,
                    float *outmat);
void lm_mat4_transpose(float *inmat, float *outmat);
void lm_vec3_cross(float *invec1, float *invec2, float *outvec);
void lm_mat4_topleftmat3(float *inmat, float *outmat);
void lm_vec3_find_perp(float *invec, float *guide, float *outvec);
float lm_vec3_length(float *invec);
float lm_vec3_angle(float *invec1, float *invec2);
#endif