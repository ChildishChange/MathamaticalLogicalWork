#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>

#pragma region MACRO

#define MAXLENTH 1000
#define IDENTIFIER_LENGTH 10

#pragma endregion

#pragma region GLOBAL

int startIndex;
int index = 0;

int currentSymbol;

int layer = 0;

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

#pragma endregion


#pragma region FUNCTION

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

#pragma endregion




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
		
		/*
		do{
			_symbol = getSymbol(expression);
			wprintf(L"%s\n", symbolName[_symbol]);
		} while (_symbol != -1);

		printf("\n");
		*/
		parseExpression(expression);
		//fputws(expression, outputFile);
		wprintf(L"%s\n", expression);
		while (index > 0)
		{
			putchar(' ');
			index--;
		}
		printf("1\n");

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
void jumpBackward()
{
	index = startIndex;
}

int getSymbol(wchar_t * expression)
{
	int _symbolIndex = 0;
	wchar_t _symbol[IDENTIFIER_LENGTH + 1];
	
	startIndex = index;

	jumpSpace(expression);
	
	//printf("currentdex:%d sizeofexpression:%d ",index, wcslen(expression));
	
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
	layer++;
	printf("parseExpression : Current in layer : %d\n", layer);
	if (getSymbol(expression)==SHARP)
	{
		jumpBackward();
		parseConj(expression);
	}
	else
	{
		jumpBackward();
		parseLevelFiveItem(expression);
		parseConj(expression);
	}
	printf("parseExpression : Current out layer : %d\n", layer);
	layer--;
}

void parseConj(wchar_t * expression)
{
	layer++;
	printf("parseConj : Current in layer : %d\n", layer);
	//#
	if (getSymbol(expression) == SHARP)
	{
		//标识符
		while (getSymbol(expression) != -1)
		{
			jumpBackward();
			getSymbol(expression) == IDENTIFIER;
			getSymbol(expression) == DIGITSTR;

			while (getSymbol(expression) != IDENTIFIER)
			{
				jumpBackward();
				if (getSymbol(expression) == TRUE)
				{
					putchar('1');
				}
				else
				{
					jumpBackward();
					if (getSymbol(expression) == FALSE)
					{
						putchar('0');

					}
				}
			}
			putchar('\n');
			jumpBackward();
		}
	}
	else
	{
		jumpBackward();
	}
	printf("parseConj : Current out layer : %d\n", layer);
	layer--;
	//index = 0
}

void parseLevelFiveItem(wchar_t * expression)
{
	layer++;
	printf("parseLevelFiveItem : Current in layer : %d\n", layer);
	parseLevelFourItem(expression);
	while (getSymbol(expression) == EQUIVALENCE)
	{
		parseLevelFourItem(expression);
	}
	jumpBackward();
	printf("parseLevelFiveItem : Current out layer : %d\n", layer);
	layer--;
}

void parseLevelFourItem(wchar_t *expression)
{
	layer++;
	printf("parseLevelFourItem : Current in layer : %d\n", layer);
	parseLevelThreeItem(expression);
	while (getSymbol(expression) == IMPLICATION)
	{
		parseLevelThreeItem(expression);
	}
	jumpBackward();
	printf("parseLevelFourItem : Current out layer : %d\n", layer);
	layer--;
}

void parseLevelThreeItem(wchar_t *expression)
{
	layer++;
	printf("parseLevelThreeItem : Current in layer : %d\n", layer);
	parseLevelTwoItem(expression);
	while (getSymbol(expression) == EXCLUSIVEOR)
	{
		parseLevelTwoItem(expression);
	}
	jumpBackward();
	printf("parseLevelThreeItem : Current out layer : %d\n", layer);
	layer--;
}

void parseLevelTwoItem(wchar_t *expression)
{
	layer++;
	printf("parseLevelTwoItem : Current in layer : %d\n", layer);
	parseLevelOneItem(expression);
	while (getSymbol(expression) == OR)
	{
		parseLevelOneItem(expression);
	}
	jumpBackward();
	printf("parseLevelTwoItem : Current out layer : %d\n", layer);
	layer--;
}

void parseLevelOneItem(wchar_t *expression)
{
	layer++;
	printf("parseLevelOneItem : Current in layer : %d\n", layer);
	parseFactor(expression);
	while (getSymbol(expression)==AND)
	{
		parseFactor(expression);
	}
	jumpBackward();
	printf("parseLevelOneItem : Current out layer : %d\n", layer);
	layer--;
}

void parseFactor(wchar_t *expression)
{
	layer++;
	printf("parseFactor : Current in layer : %d\n", layer);
	switch (getSymbol(expression))
	{
	case NOT:
		parseFactor(expression);
		break;

	case LBRACE:
		parseLevelFiveItem(expression);
		if (getSymbol(expression) == RBRACE)
		{

		}
		break;

	case TRUE:
		break;

	case FALSE:
		break;

	case IDENTIFIER:
		//function
		if (getSymbol(expression) == LBRACE)
		{
			//function without para
			if (getSymbol(expression) == RBRACE)
			{
				break;
			}
			//function with para
			else
			{
				//预读了，退一位
				jumpBackward();
				parseLevelFiveItem(expression);
				while (getSymbol(expression) == COMMA)
				{
					parseLevelFiveItem(expression);
				}
				//退出时应该预读了一个 RBRACE，退一位
				jumpBackward();
				if (getSymbol(expression) == RBRACE)
				{
					break;
				}
			}
		}
		//only identifier
		else
		{
			//预读了，所以要退一位
			jumpBackward();
			break;
		}
		break;

	default:		
		jumpSpace(expression);
		break;

	}
	printf("parseFactor : Current out layer : %d\n", layer);
	layer--;
}

