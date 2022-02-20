/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _os_check.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 15:37:45 by veilo             #+#    #+#             */
/*   Updated: 2022/02/20 17:28:03 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifdef __APPLE__
#define SCOP_APPLE
#endif
#if defined(unix) || defined(__unix__) || defined(__unix)
#define SCOP_UNIX
#endif
