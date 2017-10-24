#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUMELEM(x) (sizeof(x) / sizeof(x[0]))


void split(char *, char *, char *, char *, int *); //Splits Line into words
int getcmd(char *, int *); //Returns Value for giving command
void loadFile(char *,char *); //Load file (Not Implemented yet)
void exe(char *); //Execute laoded file
void db(); //Opens in debug mode
void dmp(char *, char *); //Dump file
void assemble(char *); //Assemble SIC files

void err(); //show Error 
void hlp(); //Show help

FILE *source, *intermediate, *opcode, *symbolTable; //FILES

typedef struct {
	char *label;
	char *mnemonic;
	char *operand;
} TOKEN;

TOKEN tok;

typedef struct {
	char OP[7];
	short FORM;
} OPCODE;

OPCODE Ops[] = {{"LDA", 3},   {"LDX", 3},    {"LDL", 3},    {"STA", 3},
                {"STX", 3},   {"STL", 3},    {"ADD", 3},    {"SUB", 3},
                {"MUL", 3},   {"DIV", 3},    {"COMP", 3},   {"TIX", 3},
                {"JEQ", 3},   {"JGT", 3},    {"JLT", 3},    {"J", 3},
                {"AND", 3},   {"OR", 3},     {"JSUB", 3},   {"SUB", 3},
                {"LDCH", 3},  {"STCH", 3},   {"ADDF", 3},   {"SUBF", 3},
                {"MULF", 3},  {"DIVF", 3},   {"LDB", 3},    {"LDS", 3},
                {"LDF", 3},   {"LDT", 3},    {"STB", 3},    {"STS", 3},
                {"STF", 3},   {"STT", 3},    {"COMPF", 3},  {"     ", 0},
                {"ADDR", 2},  {"SUBR", 2},   {"MULR", 2},   {"DIVR", 2},
                {"COMPR", 2}, {"SHIFTL", 2}, {"SHIFTR", 2}, {"RMO", 2},
                {"SVC", 2},   {"CLEAR", 2},  {"TIXR", 2},   {"      ", 0},
                {"FLOAT", 1}, {"FIX", 1},    {"NORM", 1},   {"      ", 0},
                {"LPS", 3},   {"STI", 3},    {"RD", 3},     {"WD", 3},
                {"TD", 3},    {"      ", 0}, {"STSW", 3},   {"SSK", 3},
                {"SIO", 1},   {"HIO", 1},    {"TIO", 1},    {"      ", 0}};

int main()
{
	typedef enum {ERROR = -1,LOAD,EXECUTE,DEBUG,DUMP,DIRECTORY,ASSEMBLE,HELP,EXIT} COMMAND; //Each command get enumed
	char line[80], cmd[50], p1[50], p2[50];
	char buff[255];
	int len, n;
	while(true) //Loops Until user enters "Exit"
	{
	printf("Hail Master$ ");
	fgets(line,80, stdin); //Gets input from user

	len = strlen(line) - 1;
	if(line[len] == '\n')
		line[len] = '\0';

	split(line,cmd,p1,p2,&n); //Split Line into Words [Command] [Parameter1] [Parameter2]
	
	switch(getcmd(cmd, &n)) //Gets the value, and executes
	{
		case LOAD:
			loadFile(p1,buff);
			break;
		case EXECUTE:
			exe(buff);
			break;
		case DEBUG:
			db();
			break;
		case DUMP:
			dmp(p1,p2);
			break;
		case DIRECTORY:
			system("ls");
			break;
		case ASSEMBLE:
			assemble(p1);
			break;
		case HELP:
			hlp();
			break;
		case EXIT:
			exit(0);
			break;
		case ERROR:
			err();
			break;
		default:
			printf("'%s': not found, type 'help'\n", cmd);
			break;
	}
	}


	//Testing
	printf("\n%d things on this line\n", n);
	printf("Command: %s\n", cmd);
	printf("Parameter 1: %s\n", p1);
	printf("parameter 2: %s\n", p2);


	return 0;
}

void split(char *str, char *c, char *p1, char *p2, int *n) //Splits Line into words
{
	c[0] = p1[0] = p2[0] = '\0'; //Start Everything on NULL
	//Initialized
	int addinx = 0, count = 1;
	bool cmd = false, param1 = false, param2 = false;
	
	//Loops the whole line
	for(int indx = 0; indx < 80; indx++)
	{
		if(str[indx] == '\n' || str[indx] == '\0'){break;} //Exits loop if Next character is "New line"
		
		if(str[indx] == ' ') //Check if Character is a space
		{
			if(c[0] != '\0'){cmd = true;} // If command array is not empty Change bool to true
			if(p1[0] != '\0'){param1 = true;}// If Parameter array is not empty change bool to true 
			if(p2[0] != '\0'){param2 = true;}// if parameter2 array is not emty change bool to true
			addinx = 0;//Reset Index Used for [Command] [P1][p2]
			continue;//Continues with the loop
		}

		else if(str[indx] != ' ') //If current line index is not space -> add to The Current Array
		{
			if(str[indx + 1] == ' ' && str[indx + 2] != ' ') //Check at space if next is not space add to count
				count++;
			if(cmd == false) // Goes into Command array if False
			{
				c[addinx] = str[indx];
				addinx++;
				c[addinx] = '\0';
				continue;
			}
			if(param1 == false) // Goes into param1  array if false
			{
				p1[addinx] = str[indx];
				addinx++;
				p1[addinx] = '\0';
				continue;
			}
			if(param2 == false) // Goes into param2 array if false
			{
				p2[addinx] = str[indx];
				addinx++;
				p2[addinx] = '\0';
				continue;
			}
		}		
	}
	*n = count; //Set N to count, count being the number of words counted + 1 -.-
}

int getcmd(char *c, int *n) //Return a value to the corresponding Command
{
	if(strcmp(c, "load") == 0){
		if(*n - 1 >= 2 || *n - 1 < 1) return -1; //Check for correct number of paramete
		return 0;
	}
	else if(strcmp(c, "execute") == 0)return 1;
	else if(strcmp(c, "debug") == 0)return 2;
	else if(strcmp(c, "dump") == 0){
		if(*n - 1 >= 3 || *n - 1 <= 1) return -1; //Checks for correct number of parameter
		return 3;
	}
	else if(strcmp(c, "dir") == 0 || strcmp(c, "directory")==0) return 4;
	else if(strcmp(c, "assemble") == 0){
		if(*n - 1 >= 2 || *n - 1 < 1) return -1; //Checks for correct numbe of parameters
		return 5;
	}
	else if(strcmp(c, "help") == 0) return 6;
	else if(strcmp(c, "exit") == 0){
		if(*n >1) return -1;
		return 7;
	}
	return -2;
}

void assemble(char *p1)
{

}

void loadFile(char *p1, char *buff) //Loads a files ITS ASSEMBLE :-(
{
	typedef enum { NOERROR, INVALID_MNEMONIC } ERRORS;
//	FILE *source, *intermediate, *opcode, *symbolTable; // Creats the Files pointer
	char line[500]; // String for each line in the Files "p1"
	char *token; 	// Created for STRTOK
	int startingAddress, addressCounter;
	//Opening READ only FILES
	source = fopen("Source.txt", "r");
	opcode = fopen("opcode.txt","r");
	//Opening Write Files
	intermediate = fopen("intermediate.txt","w");
	symbolTable = fopen("symbolTable.txt","w");
	if(source == NULL || opcode == NULL || intermediate == NULL || symbolTable == NULL) printf("One of the Files did not opened\n"); //Checking If Files Opened Correctly

	//token = strtok(line, " \t");
	
	//=============================
	//Allocating memory
	tok.label = (char *) malloc(6);
	tok.mnemonic = (char *) malloc(6);
	tok.operand = (char *) malloc(6);
	//=============================
	
	char *pastToken; //Keep track of past tokens
	token = line;	
	//Starts Reading from "p1"
	while(fgets(line, 500, source))
	{
		bool labelFound = true;
		
		if(pastToken != NULL)
			printf("pastToken: %s\n", pastToken);
		if(line[0] == '.')
			fprintf("%s\n", line);
		token = strtok(line, " \t\r\n\v\f");
		
		line[strcspn(line, "\n")] = '\0';

		if(line[0] != '\t')
		{
			
			strcpy(tok.label, token); //Sets label
			token = strtok(NULL, " \t\r\n\v\f"); // gets next word on the line

			strcpy(tok.mnemonic, token); //sets mnemonic
			token = strtok(NULL, " \t\r\n\v\f");

			strcpy(tok.operand, token); //sets operand

			if(strcmp(tok.mnemonic, "START") == 0) //Checks if "START" is present on the line
			{
				printf("tok.operand %s\n", tok.operand);
				startingAddress = atoi(tok.operand);
				printf("startinAddress %d\n", startingAddress);
				addressCounter = startingAddress;
				printf("addressCounter %d\n", addressCounter);
			}
			if(strcmp(tok.mnemonic, "START") == 0|| strcmp(tok.mnemonic, "STL") == 0)
			{
				addressCounter = startingAddress;
			}
			else addressCounter += 3;
			fprintf(intermediate, "%d\t %s\t %s\t %s  \n", addressCounter, tok.label, tok.mnemonic, tok.operand);
		}
		else
		{ 
			if(rsubFound) continue;

			strcpy(tok.mnemonic, token);
			token = strtok(NULL, " \t\r\n\v\f");

			strcpy(tok.operand, token);
			
			//Write to File
			fprintf(intermediate, "%d\t\t %s\t %s  \n", addressCounter, tok.mnemonic, tok.operand);
		}
		pastToken = tok.label;
	}

	//printf(" LABEL: %s\n MNEMONIC: %s\n OPERAND: %s\n", tok.label, tok.mnemonic, tok.operand);
	printf("Succesfully went throught the file\n");

	fclose(intermediate);
	fclose(source);
	fclose(opcode);
	fclose(symbolTable);

}
void exe(char *buff) //Executes Loaded File
{
	printf("Executing File\n");
	printf("%s\n",buff);
}
void db() //open in debug mode
{
	printf("Opening in debug Mode\n");
}
void dmp(char *p1, char *p2) //Dump files
{
	printf("Dumping\n");
}

void err() //Shows Error
{
	printf("Error Try Again.\n");
}
void hlp() //Shows Available commads
{
	printf("load [filename]\nexecute\ndebug\ndump [start] [end]\nassemble [filename]\n");
}
