#include "editor.h"
#include <stdio.h>
#include <string.h>

#define STRING_SIZE      100

typedef enum e_command
{
	c_edit,
	c_insert,
	c_delete,
	c_print,
	c_save,
	c_commit,
	c_exit,
	c_wrong_command,
} e_command;

static e_command getCommand(char* input);
static void getCommandParam(char* input, char* param, int n);
int main(int argc, void** argv){
	int i;
	int exit_flag = 1;
	char* input;
	long unsigned int inputSize = 0;
	char filename[STRING_SIZE];
	char statno[STRING_SIZE];
	char statement[STRING_SIZE];
	char inputversion[STRING_SIZE];
	input = (char*) malloc(sizeof(char) * STRING_SIZE);

	while(exit_flag)
	{
		printf("Please enter your command:\n");
		getline(&input, &inputSize, stdin);
		input[strlen(input) - 1] = '\0';
		switch (getCommand(input))
		{
			case c_edit:
				getCommandParam(input, filename, 1);
				char* inputversion= &input[strlen(input)-2];
				Edit(filename,atoi(inputversion));
				break;
			case c_insert:
				getline(&input, &inputSize, stdin);
				input[strlen(input) - 1] = '\0';
				getCommandParam(input, statno, 0);
				getCommandParam(input, statement, 1);
				Insert(atoi(statno), statement);
				break;
			case c_delete:
				getline(&input, &inputSize, stdin);
				input[strlen(input) - 1] = '\0';
				getCommandParam(input, statno, 0);
				deleteLine(atoi(statno));
				break;
			case c_print:
				print();
				break;
			case c_save:
				save(filename);
				break;
			case c_commit:
				commit(filename);
				break;
			case c_exit:
				exit_flag = 0;
				break;
			default:
				printf("Wrong command!\n");
		}
	}

	free(input);
	printf("Program is finished successfully!\n");

	return 0;
}
static e_command getCommand(char* input)
{
	switch(input[0])
	{
		case 'E':
		case 'e':
			return c_edit;
		case 'I':
		case 'i':
			return c_insert;
		case 'D':
		case 'd':
			return c_delete;
		case 'P':
		case 'p':
			return c_print;
		case 'S':
		case 's':
			return c_save;
		case 'C':
		case 'c':
			return c_commit;
		case 'X':
		case 'x':
			return c_exit;
		default:
			return c_wrong_command;
	}
}

static void getCommandParam(char* input, char* param, int n)
{
	int i;
	char input_backup[STRING_SIZE];
	strcpy(input_backup, input);
	char* token = strtok(input_backup, " ");
	for (i = 0; token != NULL; i++)
	{
		if (i == n)
		{
			strcpy(param, token);
			break;
		}
		token = strtok(NULL, " ");
	}
}
