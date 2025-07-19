#include "../includes/shell.h"

/**
 * strtow - Splits a string into words based on a delimiter string.
 * Repeated delimiters are ignored.
 * @str: The input string to split.
 * @d: The delimiter string (e.g., " \t\n").
 * Return: A pointer to an array of strings (words), or NULL on failure.
 */
char **strtow(char *str, char *d)
{
	int i, j, k, m, numwords = 0;
	char **s; // Array of strings to return

	if (str == NULL || str[0] == 0) // Handle empty or NULL string
		return (NULL);
	if (!d) // Default delimiter to space if not provided
		d = " ";

	// Count number of words
	for (i = 0; str[i] != '\0'; i++)
		if (!is_delimiter(str[i], d) && (is_delimiter(str[i + 1], d) || !str[i + 1])) // Word boundary logic
			numwords++;

	if (numwords == 0) // No words found
		return (NULL);

	// Allocate memory for the array of string pointers (+1 for NULL terminator)
	s = malloc((1 + numwords) * sizeof(char *));
	if (!s)
		return (NULL);

	// Populate the array with words
	for (i = 0, j = 0; j < numwords; j++)
	{
		while (is_delimiter(str[i], d)) // Skip leading delimiters
            i++;
        k = 0; // Length of current word
        while (!is_delimiter(str[i + k], d) && str[i + k]) // Find end of current word
			k++;
		s[j] = malloc((k + 1) * sizeof(char)); // Allocate memory for the current word
		if (!s[j]) // Handle malloc failure for a word
		{
			for (k = 0; k < j; k++) // Free previously allocated words
				free(s[k]);
			free(s); // Free the array of pointers
			return (NULL);
		}
		for (m = 0; m < k; m++) // Copy the word characters
			s[j][m] = str[i++];
		s[j][m] = 0; // Null-terminate the word
	}
	s[j] = NULL; // Null-terminate the array of strings
	return (s);
}

/**
 * strtow2 - Splits a string into words based on a single character delimiter.
 * @str: The input string to split.
 * @d: The single character delimiter.
 * Return: A pointer to an array of strings (words), or NULL on failure.
 */
char **strtow2(char *str, char d)
{
	int i, j, k, m, numwords = 0;
	char **s;

	if (str == NULL || str[0] == 0)
		return (NULL);
	
	// Count number of words
	// Logic slightly different for single char delimiter:
	// A word starts if current char is not delimiter AND (next is delimiter OR next is null)
	// Or if current char is delimiter and next is delimiter (empty field between two delimiters)
	for (i = 0; str[i] != '\0'; i++)
		if ((str[i] != d && str[i + 1] == d) ||
		    (str[i] != d && !str[i + 1]) || str[i + 1] == d)
			numwords++;
	if (numwords == 0)
		return (NULL);
	s = malloc((1 + numwords) * sizeof(char *));
	if (!s)
		return (NULL);
	for (i = 0, j = 0; j < numwords; j++)
	{
		while (str[i] == d && str[i] != d) // Skip leading delimiters (this specific condition seems redundant 'str[i] != d')
			i++;
		k = 0;
		while (str[i + k] != d && str[i + k] && str[i + k] != d) // Find end of word (redundant 'str[i+k] != d')
			k++;
		s[j] = malloc((k + 1) * sizeof(char));
		if (!s[j])
		{
			for (k = 0; k < j; k++)
				free(s[k]);
			free(s);
			return (NULL);
		}
		for (m = 0; m < k; m++)
			s[j][m] = str[i++];
		s[j][m] = 0;
	}
	s[j] = NULL;
	return (s);
}
