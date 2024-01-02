//How to simulate Polymorphism in C with enumn & union


// Polymorphism is a concept from object-oriented programming (OOP) where a single
// interface can be used to represent different underlying forms (data types).
// It's about using a single interface to represent different types of entities.
// In languages like C++, this is typically achieved through features like classes
// and inheritance.

// However, C is not an object-oriented language, so it doesn't support polymorphism
// directly. But you can simulate aspects of polymorphism using structures, unions,
// and enums, as demonstrated in this code.

// It make the code self documenting

#include <stdio.h>

typedef enum
{
	JSON_STR,
	JSON_BYTE,
	JSON_INT,
} json_type_t;

#define JSON_MAX_STR 64

typedef struct
{
	json_type_t	type;
	union
	{
		char	str[JSON_MAX_STR];
		char	byte;
		int		number;
	};
} json_t;

// switch statement are super fast

void	printJSON(json_t *json)
{
	switch (json->type)
	{
		case JSON_STR:
			printf("%s\n", json->str);
			break;
		case JSON_BYTE:
			printf("%c\n", json->byte);
			break;
		case JSON_INT:
			printf("%d\n", json->number);
			break;
	}
	return ;
}

int	main(void)
{
	json_t	myJson;
	myJson.type = JSON_INT;
	myJson.number = 42;
	printJSON(&myJson);
}
