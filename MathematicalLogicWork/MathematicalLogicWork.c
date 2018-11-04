#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>

#define MAXLENTH 1000
#define IDENTIFIER_LENTH 10

const enum symbol 
{ 
	LBRACE = 0, RBRACE, COMMA, SHARP, 
	TRUE, FALSE, AND, OR, NOT, 
	EXCLUSIVEOR, IMPLICATION, EQUIVALENCE, 
	IDENTIFIER = 12, DIGITSTR  
};

const wchar_t *symbolName[] = 
{
	L"LBRACE", L"RBRACE", L"COMMA", L"SHARP",
	L"TRUE", L"FALSE", L"AND", L"OR", L"NOT",
	L"EXCLUSIVEOR", L"IMPLICATION", L"EQUIVALENCE",
	L"IDENTIFIER", L"DIGITSTR"
};

const wchar_t symbolSet[] =
{
	L'(', L')', L',', L'#',
	L'1', L'0', L'∧', L'∨', L'¬', 
	L'⊕', L'→', L'↔'
};

int index = 0;

void jumpSpace(wchar_t *expression);
int getSymbol(wchar_t *expression);


void parseExpression(wchar_t *expression);
void parseConj(wchar_t *expression);
void parseLevelFiveItem(wchar_t *expression);
void parseLevelFourItem(wchar_t *expression);
void parseLevelThreeItem(wchar_t *expression);
void parseLevelTwoItem(wchar_t *expression);
void parseLevelOneItem(wchar_t *expression);
void parseFactor(wchar_t *expression);
void parseIdentifier(wchar_t *expression);

int main(int argc, char *argv[])
{
	errno_t err;
	wchar_t expression[MAXLENTH];
	FILE *inputFile;
	FILE *outputFile;
	int _symbol;

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
		expression[wcslen(expression) - 1] = L'\0';
		
		wprintf(L"%s\n", expression);
		
		do{
			_symbol = getSymbol(expression);
			wprintf(L"%s\n", symbolName[_symbol]);
		} while (_symbol != -1);

		printf("\n");
		//parseExpression(expression);
		//fputws(expression, outputFile);
		
		wmemset(expression, 0, MAXLENTH-1);
		index = 0;
	}
	fclose(inputFile);
	fclose(outputFile);
	
	
	getchar();
	return 0;
}

void jumpSpace(wchar_t *expression)
{
	while (expression[index] == L' ')
	{
		index++;
	}
}

int getSymbol(wchar_t * expression)
{
	int _symbolIndex = 0;
	wchar_t _symbol[IDENTIFIER_LENTH + 1];
	
	jumpSpace(expression);
	printf("currentdex:%d sizeofexpression:%d ",index, wcslen(expression));
	if (index >= (int)wcslen(expression))
	{
		return -1;
	}

	switch (expression[index])
	{
	case L'(':
		index++;
		return LBRACE;
	case L')':
		index++;
		return RBRACE;
	case L',':
		index++;
		return COMMA;
	case L'#':
		index++;
		return SHARP;
	case L'1':
		index++;
		return TRUE;
	case L'0':
		index++;
		return FALSE;
	case L'∧':
		index++;
		return AND;
	case L'∨':
		index++;
		return OR;
	case L'¬':
		index++;
		return NOT;
	case L'⊕':
		index++;
		return EXCLUSIVEOR;
	case L'→':
		index++;
		return IMPLICATION;
	case L'↔':
		index++;
		return EQUIVALENCE;
	default:
		if (iswalpha(expression[index]))
		{
			while (/*_symbolIndex < IDENTIFIER_LENTH && */
				   (iswalpha(expression[index])||iswdigit(expression[index])))
			{
				_symbol[_symbolIndex++] = expression[index++];
			}
			_symbol[_symbolIndex] = L'\0';
			return IDENTIFIER;
		}
		else if (iswdigit(expression[index]))
		{
			while (iswdigit(expression[index]))
			{
				_symbol[_symbolIndex++] = expression[index++];
			}
			_symbol[_symbolIndex] = L'\0';
			return DIGITSTR;
		}
		return -1;
	}

}

void parseExpression(wchar_t * expression)
{
	if (getSymbol(expression)==SHARP)
	{
		parseConj(expression);
		parseLevelFiveItem(expression);
	}
	else
	{
		parseLevelFiveItem(expression);
		parseConj(expression);
	}
}

void parseConj(wchar_t * expression)
{
	//index = 0
}

void parseLevelFiveItem(wchar_t * expression)
{
	parseLevelFourItem(expression);
	//index at ?
}

void parseLevelFourItem(wchar_t *expression)
{
	parseLevelThreeItem(expression);

}

void parseLevelThreeItem(wchar_t *expression)
{
	parseLevelTwoItem(expression);

}

void parseLevelTwoItem(wchar_t *expression)
{
	parseLevelOneItem(expression);

}

void parseLevelOneItem(wchar_t *expression)
{
	parseFactor(expression);

}

void parseFactor(wchar_t *expression)
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

void parseIdentifier(wchar_t *expression)
{

}