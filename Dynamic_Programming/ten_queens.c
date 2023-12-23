#include <unistd.h>

// Write a function that displays all the possibilities to place ten queens on a 10x10 chessboard 
// without them being able to reach each other in a single move.
// Recursion must be used.
// The return value of your function must be the number of solutions displayed.

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

//condition to advance in the resolution
int	ft_ten_queens_puzzle_test(int list[10], int x, int y)
{
	int	i;

	i = 0;
	while (i < x)
	{
		if (y == list[i] || list[i] + i == y + x || list[i] - i == y - x)
			return (0);
		i++;
	}
	return (1);
}

//resolution
void	ft_ten_queens_puzzle_recursive(int list[10], int *nb_solution, int pos)
{
	int	i;
	int	j;

	//array end at 9, so if its 10, its full
	if (pos == 10)
	{
		*nb_solution += 1;
		j = 0;
		while (j < 10)
			ft_putchar(list[j++] + '0');
		ft_putchar('\n');
	}
	else
	{
		i = 0;
		while (i < 10)
		{
			if (ft_ten_queens_puzzle_test(list, pos, i))
			{
				list[pos] = i;
				ft_ten_queens_puzzle_recursive(list, nb_solution, pos + 1);
			}
			i++;
		}
	}
}

//setup
int	ft_ten_queens_puzzle(void)
{
	int	list[10];
	int	nb_solution;

	nb_solution = 0;
	ft_ten_queens_puzzle_recursive(list, &nb_solution, 0);
	return (nb_solution);
}

#include <stdio.h>
int main(void)
{
	int i;
	i = ft_ten_queens_puzzle();
	printf("%d", i);
	return (0);
}