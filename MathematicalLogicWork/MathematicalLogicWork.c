#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>

#pragma region MACRO


#define MAXLENTH 1000 //单行最长长度
#define MAXPARANUM 10 //函数最大参数数量
#define IDENTIFIER_LENGTH 10 //标识符最大长度
#define VARIABLE_NUMBER 10 //命题变元最大数量
#define FUNCTION_NUMBER 10 //函数最大数量
#define TRUTHTABLE_LENGTH 1024 //真值表最大长度

#pragma endregion

#pragma region GLOBAL

int layer = 0; //当前所处的语法树深度
int startIndex = 0; //getSymbol运行前index所处的位置
int index = 0; //当前词法分析到的位置
wchar_t currentSymbol[IDENTIFIER_LENGTH + 1]; //目前分析得到的标识符内容

struct function //函数列表
{
	wchar_t name[IDENTIFIER_LENGTH + 1];
	int paraNumber;
	wchar_t paras[MAXPARANUM];
	wchar_t truthTable[TRUTHTABLE_LENGTH];
	
	//计算函数时，先获取其参数的值，然后调用
	/*
	按照参数顺序计算值，然后查找真值表，返回值
	*/

}functionList[FUNCTION_NUMBER];
int funIndex = 0;

struct variable //命题变元列表
{
	wchar_t name[IDENTIFIER_LENGTH + 1];
	int truthValue;
	//计算公式时，首先赋值，然后计算值，计算值时先查表
}variableList[VARIABLE_NUMBER];
int varIndex = 0;


const enum symbol //词法符号
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

#pragma region SYMBOL

int searchVarList(wchar_t *name);
int searchFunList(wchar_t *name);

#pragma endregion



#pragma region TODO
/*
TODO:
生成运算栈
计算运算栈时，对每一个逻辑联结词，包括自定义联结词
按照layer从大到小，按照出现的先后顺序从先到后计算运算符


TODO:
运行前需要对所有函数检查是否定义


*/

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
		
		parseExpression(expression);
		wprintf(L"%s\n", expression);
		
		funIndex--;
		wprintf(L"Print Function Table:\n");
		while (funIndex>=0)
		{
			wprintf(L"Num.%d function : %s\n",funIndex,functionList[funIndex].name);
			wprintf(L"Number of parameters : %d\n",functionList[funIndex].paraNumber);
			wprintf(L"Truth Table : %s\n",functionList[funIndex].truthTable);
			funIndex--;
		}

		varIndex--;
		wprintf(L"Print Variable Table:\n");
		while (varIndex >= 0)
		{
			wprintf(L"Num.%d variable : %s\n", varIndex, variableList[varIndex].name);
			varIndex--;
		}

		//归零四大天王
		wmemset(expression, 0, MAXLENTH-1);
		index = 0;
		funIndex = 0;
		varIndex = 0;
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
	wmemset(currentSymbol, 0, IDENTIFIER_LENGTH + 1);
	index = startIndex;
}

int getSymbol(wchar_t * expression)
{
	int _symbolIndex = 0;
	wmemset(currentSymbol, 0, IDENTIFIER_LENGTH + 1);
	startIndex = index;
	
	jumpSpace(expression);
	//printf("currentdex:%d sizeofexpression:%d ",index, wcslen(expression));
	
	if (index >= (int)wcslen(expression))
	{
		return -2;
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
		currentSymbol[_symbolIndex++] = expression[index++];
		currentSymbol[_symbolIndex] = L'\0';
		return TRUE;
	case L'0':
		currentSymbol[_symbolIndex++] = expression[index++];
		currentSymbol[_symbolIndex] = L'\0';
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
			while ((iswalpha(expression[index])||iswdigit(expression[index])))
			{
				currentSymbol[_symbolIndex++] = expression[index++];
			}
			currentSymbol[_symbolIndex] = L'\0';
			return IDENTIFIER;
		}
		else if (iswdigit(expression[index]))
		{
			while (iswdigit(expression[index]))
			{
				currentSymbol[_symbolIndex++] = expression[index++];
			}
			currentSymbol[_symbolIndex] = L'\0';
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
	int _truthTableIndex = 0;
	int _searchResult = -1;
	int _sym = -1;

	if (getSymbol(expression) == SHARP)
	{
		while (getSymbol(expression) != -2)//读取所有的联结词定义直到达到文件末尾
		{
			jumpBackward();
			if (getSymbol(expression) == IDENTIFIER)
			{
				_searchResult = searchFunList(currentSymbol);
				if (_searchResult == -1)//新建一个联结词
				{
					wcsncpy(functionList[funIndex].name, currentSymbol, IDENTIFIER_LENGTH + 1);
					_sym = getSymbol(expression);
					if (_sym == DIGITSTR || _sym == TRUE || _sym == FALSE)
					{
						functionList[funIndex].paraNumber = _wtoi(currentSymbol);
						while (getSymbol(expression) != IDENTIFIER)
						{
							jumpBackward();
							if (getSymbol(expression) == -2){break;}
							else { jumpBackward(); }

							if (getSymbol(expression) == TRUE)
							{
								putchar('1');
								functionList[funIndex].truthTable[_truthTableIndex++] = L'1';
								continue;
							}
							else
							{
								jumpBackward();
								if (getSymbol(expression) == FALSE)
								{
									putchar('0');
									functionList[funIndex].truthTable[_truthTableIndex++] = L'0';
								}
								continue;
							}
						}
						putchar('\n');
						jumpBackward();
					}
					functionList[funIndex].truthTable[_truthTableIndex] = L'\0';
					funIndex++;
				}
				else//检查是否一致并补全真值表
				{
					_sym = getSymbol(expression);
					if (_sym == DIGITSTR || _sym == TRUE || _sym == FALSE)
					{
						if (functionList[_searchResult].paraNumber == _wtoi(currentSymbol))
						{
							while (getSymbol(expression) != IDENTIFIER)
							{
								jumpBackward();
								if (getSymbol(expression) == -2){break;}
								else{ jumpBackward(); }

								if (getSymbol(expression) == TRUE)
								{
									putchar('1');
									functionList[_searchResult].truthTable[_truthTableIndex++] = L'1';
									continue;
								}
								else
								{
									jumpBackward();
									if (getSymbol(expression) == FALSE)
									{
										putchar('0');
										functionList[_searchResult].truthTable[_truthTableIndex++] = L'0';
									}
									continue;
								}
							}
							putchar('\n');
							jumpBackward();
						}
						else{/*这里要报错*/}
					}
					functionList[_searchResult].truthTable[_truthTableIndex] = L'\0';
				}
				_truthTableIndex = 0;
				_searchResult = -1;
			}
		}
	}
	else
	{
		jumpBackward();
	}
	printf("parseConj : Current out layer : %d\n", layer);
	layer--;
}

void parseLevelFiveItem(wchar_t * expression)
{
	layer++;
	printf("parseLevelFiveItem : Current in layer : %d\n", layer);
	parseLevelFourItem(expression);
	while (getSymbol(expression) == EQUIVALENCE)
	{	
		parseLevelFourItem(expression);
		printf("current operation : EQUIVALENCE at layer %d\n",layer);
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
		printf("current operation : IMPLICATION at layer %d\n", layer);
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
		printf("current operation : EXCLUSIVEOR at layer %d\n", layer);
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
		printf("current operation : OR at layer %d\n", layer);
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
		printf("current operation : AND at layer %d\n", layer);
	}
	jumpBackward();
	printf("parseLevelOneItem : Current out layer : %d\n", layer);
	layer--;
}

void parseFactor(wchar_t *expression)
{
	wchar_t tempIdentifier[IDENTIFIER_LENGTH + 1];
	int _paraNumber = 0;
	int _tempFunIndex = 0;
	int _searchFunResult = -1;
	
	layer++;
	printf("parseFactor : Current in layer : %d\n", layer);

	switch (getSymbol(expression))
	{
	case NOT:
		parseFactor(expression);
		printf("current operation : NOT at layer %d\n", layer);
		break;
	case LBRACE:
		parseLevelFiveItem(expression);
		if (getSymbol(expression) == RBRACE){}
		break;
	case TRUE:
		break;
	case FALSE:
		break;
	case IDENTIFIER:
		wcsncpy(tempIdentifier,currentSymbol, IDENTIFIER_LENGTH + 1);
		if (getSymbol(expression) == LBRACE)//自定义联结词
		{
			_searchFunResult = searchFunList(tempIdentifier);
			if (_searchFunResult == -1)//当符号表中不存在时，则新建一个联结词
			{
				wcsncpy(functionList[funIndex].name, tempIdentifier, IDENTIFIER_LENGTH + 1);
				if (getSymbol(expression) == RBRACE)//该联结词参数数为0
				{
					functionList[funIndex].paraNumber = _paraNumber;
					funIndex++;
					break;
				}
				else //该联结词参数不为零
				{
					//预读了，退一位
					jumpBackward();
					//如果参数是函数，那么需要给自己占位，需要修改funIndex，等到运行完后再返回到原始位置
					_tempFunIndex = funIndex;
					
					funIndex++;
					parseLevelFiveItem(expression);
					_paraNumber++;

					while (getSymbol(expression) == COMMA)
					{
						funIndex++;
						parseLevelFiveItem(expression);
						_paraNumber++;
					}
					//退出时应该预读了一个 RBRACE，退一位
					jumpBackward();
					if (getSymbol(expression) == RBRACE)
					{
						wprintf(L"current operation : %s at layer %d\n", tempIdentifier, layer);
						functionList[_tempFunIndex].paraNumber = _paraNumber;
						break;
					}
				}
			}
			else//当符号表中存在时，检查与之前的是否一致
			{
				if (getSymbol(expression) == RBRACE)
				{
					if (functionList[_searchFunResult].paraNumber == 0){break;}
					else {/*报错*/break; }
				}
				else
				{
					//预读了，退一位
					jumpBackward();
					//此时不需要给自己占位
					parseLevelFiveItem(expression);
					_paraNumber++;

					while (getSymbol(expression) == COMMA)
					{
						parseLevelFiveItem(expression);
						_paraNumber++;
					}
					//退出时应该预读了一个 RBRACE，退一位
					jumpBackward();
					if (getSymbol(expression) == RBRACE)
					{
						wprintf(L"current operation : %s at layer %d\n", tempIdentifier, layer);
						functionList[_searchFunResult].paraNumber = _paraNumber;
						break;
					}
				}
			}
		}
		else//命题变元
		{
			//预读了，所以要退一位
			jumpBackward();
			//首先查找是否定义了这个变元
			if (searchVarList(tempIdentifier) == -1)
			{
				//新建变元
				wcsncpy(variableList[varIndex].name, tempIdentifier, IDENTIFIER_LENGTH + 1);
				varIndex++;
			}
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

int searchVarList(wchar_t *name)
{
	int i = 0;
	for (; i < varIndex; i++)
	{
		if (wcsncmp(variableList[i].name, name, wcslen(name)) == 0)
		{
			return i;
		}
	}
	return -1;
}

int searchFunList(wchar_t * name)
{
	int i = 0;
	for (; i < funIndex; i++)
	{
		if (wcsncmp(functionList[i].name, name, wcslen(name)) == 0)
		{
			return i;
		}
	}
	return -1;
}
