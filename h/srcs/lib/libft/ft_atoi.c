/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:23:49 by hjuma             #+#    #+#             */
/*   Updated: 2024/10/21 14:35:45 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int	sign;
	int	i;
	int	result;

	sign = 1;
	i = 0;
	result = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (sign * result);
}
/*
int main(void)
{
    char s[] = "   -12345";
    char s1[] = "  +45678abcd";
    char s2[] = "  -abc123";  // Invalid input, no digits after '-'

    printf("Result 1: %d\n", ft_atoi(s));    // Expected: -12345
    printf("Result 2: %d\n", ft_atoi(s1));   // Expected: 45678
    printf("Result 3: %d\n", ft_atoi(s2));   // Expected: 0
    return 0;
}
*/
