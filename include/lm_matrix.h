/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lm_matrix.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 17:37:02 by veilo             #+#    #+#             */
/*   Updated: 2022/03/21 16:27:51 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LM_MATRIX_H
#define LM_MATRIX_H

#include <math.h>

void lm_mat4_print(float *mat);
void lm_vec3_rotate(float *invec, float *axis, float angle, float *outvec);
void lm_mat4vec4_mul(float *invec, float *inmat, float *outvec);
float lm_vec4_dot(float *invec1, float *invec2);
void lm_vec2_add(float *v1, float *v2, float *outvec);
void lm_vec3_add(float *v1, float *v2, float *outvec);
void lm_vec2_sub(float *v1, float *v2, float *outvec);
void lm_vec3_sub(float *v1, float *v2, float *outvec);
void lm_mat4_create_rotmat(float *rotmat, float *axis, float angle);
void lm_mat4_projection(float fovx, float fovy, float near, float far,
                        float *outmat, int transpose);
void lm_mat4_identity(float *outmat);
void lm_mat4_scale(float *inmat, float sx, float sy, float sz, float *outmat);
void lm_mat4_multiply(float *inmat1, float *inmat2, float *outmat);
void lm_mat4_translate(float *inmat, float *translation, float *outmat);
void lm_mat4_add(float *mat1, float *mat2, float *outmat);

#endif