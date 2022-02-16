/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lm_matrix.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 17:37:02 by veilo             #+#    #+#             */
/*   Updated: 2022/02/16 15:29:26 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LM_MATRIX_H
#define LM_MATRIX_H

#include <math.h>

void lm_vec3_rotate(float *invec, float *axis, float angle, float *outvec,
                    float *rotmat);
void lm_mat4vec4_mul(float *invec, float *inmat, float *outvec);
float lm_vec4_dot(float *invec1, float *invec2);
void lm_mat4_create_rotmat(float *rotmat, float *axis, float angle);
void lm_mat4_projection(float fovx, float fovy, float near, float far,
                        float *outmat);
void lm_mat4_identity(float *outmat);
void lm_mat4_scale(float *inmat, float sx, float sy, float sz, float *outmat);
void lm_mat4_multiply(float *inmat1, float *inmat2, float *outmat);
void lm_mat4_translate(float *inmat, float *translation, float *outmat);

#endif