/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lm_matrix.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 17:37:02 by veilo             #+#    #+#             */
/*   Updated: 2022/02/08 17:45:36 by veilo            ###   ########.fr       */
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

#endif