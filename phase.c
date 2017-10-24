#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct {
	char *label;
	char *mnemonic;
	char *operand;
} TOKEN;

typedef struct {
	char OP[7];
	short NUM;
}OPCODE;

OPCODE ops[] = {{"CODE" , 3}, {"THIS", 2}, {"THAT", 3}};





TOKEN token;

int main()
{
	
	char string[20] = "23434";
	char string2[2] = "aa";

	if(isalpha(string) > 0 || isalpha(string) < 0)
		printf("2 is digit");
	if(isdigit(string2))
		printf("string2 is a digit");	

	return 0;
}
