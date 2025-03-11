#include "minishell.h"

unsigned int hash_function(const char *key)
{
	unsigned long	hash;
	int				c;

	hash = 5381;
	while ((c = *key++))
		hash = ((hash << 5) + hash) + c;
	return (hash % HASH_SIZE);
}

