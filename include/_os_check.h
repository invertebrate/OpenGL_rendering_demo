/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _os_check.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 15:37:45 by veilo             #+#    #+#             */
/*   Updated: 2022/02/20 16:20:18 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifdef APPLE__
#define SCOP_APPLE
#endif
#if defined(unix) || defined(__unix__) || defined(__unix)
#define SCOP_UNIX
#endif
