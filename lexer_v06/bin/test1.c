/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_program_name.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <pollivie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 12:30:27 by pollivie          #+#    #+#             */
/*   Updated: 2023/09/12 12:30:31 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>

int	main(int argc, char *argv[])
{
	int	arglen;

	arglen = 0;
	if (argc == 1)
	{
		while (argv[0][arglen])
			++arglen;
		write(1, argv[0], arglen);
		write(1, "\n", 1);
	}
	return (0);
}
