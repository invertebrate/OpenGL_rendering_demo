/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _os_check.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <veilo@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 15:37:45 by veilo             #+#    #+#             */
/*   Updated: 2022/04/19 15:46:46 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifdef __APPLE__
#define CHECK_APPLE
#endif
#if defined(unix) || defined(__unix__) || defined(__unix)
#define CHECK_UNIX
#endif
