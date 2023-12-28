// Write a programe that takes a string. This string represents a graph and is
// composed of series of links between this graph's nodes. Links are separated by
// a single space. Nodes are represented by numbers, and links by two nodes
// separated by a '-'. For exemple, if there is a link between nodes 2
// and 3, it could be written as "2-3" or "3-2".

// The program will display the number of nodes comprised in the longest chain,
// followed by a '\n', given it's impossible to pass through a node more than once.

// If the number of parameters is different from 1, the program displays a '\n'.

// Examples:

// $>./g_diam "17-5 5-8 8-2 2-8 2-8 17-21 21-2 5-2 2-6 6-14 6-12 12-19 19-14 14-42" | cat -e
// 10$
// $>./g_diam "1-2 2-3 4-5 5-6 6-7 7-8 9-13 13-10 10-2 10-11 11-12 12-8 16-4 16-11 21-8 21-12 18-10 18-13 21-18" | cat -e
// 15$

// i wanted to try to solve anything with dynamic programming
// this program does manage the edge case where there is only one node ("1-1")
// Note: The program assumes that the input is always well-formatted. It stores graph
// information as two arrays of nodes representing the edges, instead of using an adjacency
// matrix or adjacency list, to focus on enhancing dynamic programming techniques.

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool parse_input(char *input, int **left, int **right, int *size);
char	**ft_split(char *str, char *charset);
int	ft_atoi(char *str);
void    free_split(char **split);

bool    test(int index_test, int pos, int linker, int *new_link, int *list, int *left, int *right)
{
    int i = 0;

    //does it have a linker
    if (left[index_test] == linker)
    {
        *new_link = right[index_test];
    }
    else if (right[index_test] == linker)
    {
        *new_link = left[index_test];
    }
    else
    {
        return false;
    }
    //index pas deja utiliser (list)
    while (i < pos)
    {
        if (list[i] == index_test)
            return false;
        i++;
    }
    i = 0;
    //le chiffre non linker de lindex pas deja utiliser ailleurs
    while (i < pos)
    {
        if (left[list[i]] == *new_link || right[list[i]] == *new_link)
            return false;
        i++;
    }
    return true;
}

void    recursive(int pos, int *solution, int size, int linker, int *list, int *left, int *right)
{
    int i = 0;
    int new_link = 0;

    //printf("%d\n", pos);
    //fflush(stdout);
    if (pos > *solution)
        *solution = pos;
    while (i < size)
    {
        if (test(i, pos, linker, &new_link, list, left, right))
        {
            list[pos] = i;
            recursive(pos+1, solution, size, new_link, list, left, right);
        }
        i++;
    }
}

void    setup(int *left, int *right, int size)
{
    int list[size];
    int solution = 0;
    int i = 0;

    //testing each starting position
    while (i < size)
    {
        if (left[i] != right[i])
        {
            list[0] = i;
            recursive(1, &solution, size, left[i], list, left, right);
            recursive(1, &solution, size, right[i], list, left, right);
        }
        i++;
    }
    //+1 to solution cause we ignore the starting node
    printf("Solution is : %d\n", solution + 1);
    fflush(stdout);
}

int main(int argc, char *argv[]) {
    int *left;
    int *right;
    int size;

    if (argc == 2)
    {
        if (!parse_input(argv[1], &left, &right, &size))
            return EXIT_FAILURE;
        setup(left, right, size);
        free(left);
        free(right);
    }
    else
        printf("\n");

    return EXIT_SUCCESS;
}

/////////////////// PARSING /////////////////////////////////////////////////
bool parse_input(char *input, int **left, int **right, int *size)
{
    int i = 0;
    char **split;

    split = ft_split(input, " -");
    if (!split)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return false;
    }

    while (split[i])
        i++;

    *size = i / 2;

    *left = (int *)malloc(sizeof(int) * (*size));
    if (!(*left))
    {
        free_split(split);
        return false;
    }

    *right = (int *)malloc(sizeof(int) * (*size));
    if (!(*right))
    {
        free(*left);
        free_split(split);
        return false;
    }

    for (int j = 0; j < *size; j++)
    {
        (*left)[j] = ft_atoi(split[j * 2]);
        (*right)[j] = ft_atoi(split[(j * 2) + 1]);
    }

    free_split(split);
    return true;
}


void    free_split(char **split)
{
    if (split)
    {
        for (int i = 0; split[i]; i++)
        {
            free(split[i]);
        }
        free(split);
    }
}

int	ft_atoi(char *str)
{
	int	i;
	int	sign;
	int	nb;

	sign = 1;
	nb = 0;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
	{
		i++;
	}
	while (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb = (nb * 10) + (str[i] - '0');
		i++;
	}
	nb *= sign;
	return (nb);
}

int	is_separator(char c, char *charset)
{
	int	i;

	if (c == '\0')
		return (1);
	i = 0;
	while (charset[i])
	{
		if (c == charset[i])
			return (1);
		i++;
	}
	return (0);
}

int	count_word(char *str, char *charset)
{
	int	i;
	int	word_count;

	i = 0;
	word_count = 0;
	while (str[i] != '\0')
	{
		if (!(is_separator(str[i], charset)) && is_separator(str[i + 1], charset))
			word_count++;
		i++;
	}
	return (word_count);
}

void	write_letter(char *word, char *str, char *charset)
{
	int	i;

	i = 0;
	while (!(is_separator(str[i], charset)))
	{
		word[i] = str[i];
		i++;
	}
	word[i] = '\0';
}

void	write_word(char **arr_word, char *str, char *charset)
{
	int		i;
	int		j;
	int		word;

	word = 0;
	i = 0;
	j = 0;
	while (str[i] != '\0')
	{
		if (is_separator(str[i], charset))
			i++;
		else
		{
			while (!(is_separator(str[i + j], charset)))
				j++;
			arr_word[word] = (char *)malloc(sizeof(char) * (j + 1));
			write_letter(arr_word[word], str + i, charset);
			i += j;
			word++;
			j = 0;
		}
	}
}

char	**ft_split(char *str, char *charset)
{
	char	**arr_word;
	int		word_count;

	word_count = count_word(str, charset);
	arr_word = (char**)malloc(sizeof(char*) * (word_count + 1));
    if (!arr_word)
        return NULL;
    for (int i = 0; i < word_count + 1; i++)
    {
        arr_word[i] = NULL;
    }
	write_word(arr_word, str, charset);
    // Check if any malloc inside write_word failed
    for (int i = 0; i < word_count; i++)
    {
        if (!arr_word[i])
        {
            free_split(arr_word);
            return NULL;
        }
    }
	return (arr_word);
}
