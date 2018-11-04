#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>

#define MAXLENTH 1000

const enum symbol 
{ 
	LBRACE = 0, RBRACE, COMMA, SHARP, 
	TRUE, FALSE, AND, OR, NOT, EXCLUSIVEOR, IMPLICATION,
	EQUIVALENCE
};

int index = 0;

void jumpSpace(char *expression);
int getSymbol(char *expression);


void parseExpression(char *expression);
void parseConj(char *expression);
void parseLevelFiveItem(char *expression);
void parseLevelFourItem(char *expression);
void parseLevelThreeItem(char *expression);
void parseLevelTwoItem(char *expression);
void parseLevelOneItem(char *expression);
void parseFactor(char *expression);
void parseIdentifier(char *expression);

int main(int argc, char *argv[])
{
	errno_t err;
	wchar_t expression[MAXLENTH];
	FILE *inputFile;
	FILE *outputFile;
	
	if (argc == 3)
	{
		printf("%s %s\n", argv[1], argv[2]);
		err = fopen_s(&inputFile, argv[1], "r,ccs=UNICODE");
		if (err!=0)
		{
			printf("NO INPUT FILE:%s\n",argv[1]);
			return 1;
		}
		err = fopen_s(&outputFile, argv[2], "w,ccs=UNICODE");
		if (err!=0)
		{
			printf("NO OUTPUT FILE:%s\n", argv[2]);
			return 1;
		}
	}
	else
	{
		printf("PARAMETER NUMBER ERROR:%d", argc - 1);
		return 1;
	}
	
	
	while (!feof(inputFile))
	{
		fgetws(expression, MAXLENTH - 1, inputFile);

		wprintf(L"%s", expression);
		
		fputws(expression, outputFile);
		
		wmemset(expression, 0, MAXLENTH);
	}
	fclose(inputFile);
	fclose(outputFile);
	//parseExpression(expression);
	
	getchar();
	return 0;
}

void jumpSpace(char *expression)
{
	while (expression[index] == ' ')
	{
		index++;
	}
}

int getSymbol(char * expression)
{
	int symbolIndex = 0;
	
	

	switch (expression[index])
	{
	case 'a':
		break;
		
	}

	return 0;
}

void parseExpression(char * expression)
{
	jumpSpace(expression);
	if (expression[index] == '#')
	{
		parseConj(expression);
		parseLevelFiveItem(expression);
	}
}

void parseConj(char * expression)
{
	//index = 0
}

void parseLevelFiveItem(char * expression)
{
	parseLevelFourItem(expression);
	//index at ?
}

void parseLevelFourItem(char *expression)
{
	parseLevelThreeItem(expression);

}

void parseLevelThreeItem(char *expression)
{
	parseLevelTwoItem(expression);

}

void parseLevelTwoItem(char *expression)
{
	parseLevelOneItem(expression);

}

void parseLevelOneItem(char *expression)
{
	parseFactor(expression);

}

void parseFactor(char *expression)
{
	switch (expression[index])
	{
	case '¬':
		break;

	case '(':
		break;

	case '0':

		break;
	
	case '1':
		
		break;

	default:
		parseIdentifier(expression);
		jumpSpace(expression);
		
		break;
	}
}

void parseIdentifier(char *expression)
{

}