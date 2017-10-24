#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
	FILE *source;
	char line[500];
	source = fopen("Source.txt", "r");
	
	char *tokens;
	
	token.label = (char *) malloc(6);
	token.mnemonic = (char *) malloc(6);
	token.operand = (char *) malloc(6);
	
	tokens = line;

	while(fgets(line, 500, source) != NULL)
	{
		tokens = strtok(line, " \t");

		printf("Token: %s\n", tokens);
	}

	fclose(source);
	return 0;
}
