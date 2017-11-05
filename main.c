#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include "expres.h"


int main() 
{
	char * type[] = {"*","/","\\","+","-","=","<>","<","<=",">",">=","(",
	                 ")","num","str","bool","$", "neterm"};
	TStack stack;
	SInit(&stack);
	
	int i = 0;
	for (i = 0; i <= ex_dollar; i++) {
		SPush(&stack, i);
		printf ("%s ", type[stack.topPtr->type]);
	}
	printf ("\n");
	
	while (!SEmpty(&stack)) {
		printf ("%s ", type[stack.topPtr->type]);
		printf ("(A:%s) ", type[SActive(&stack)]);
		SPop(&stack);		
	}


	printf ("\n");

	return 0;
}

