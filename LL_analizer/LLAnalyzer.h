#pragma once
#include "Scaner.h"
#include <vector>
#include <conio.h>

using namespace std;

struct TOneSymb{
	bool isTerm;
	int type;
	LEX lexem;
	TOneSymb(bool f, int typeLex,LEX lexeM = "Нетерминал"){
		isTerm = f;
		type = typeLex;
		strcpy_s(lexem, lexeM);
	}
};

class LLAnalyzer
{
	vector<TOneSymb > stack;
	Scaner *scaner;
	int currentType;

	public:
		LLAnalyzer(){scaner = new Scaner();}
		~LLAnalyzer();

		void print_error(){
			int curPos = 0;
			int line = 0;
			int pos = 0;
			scaner->getData(curPos, line, pos);
			printf("Обнаружена ошибка в строке = %d позиция = %d ожилось: %s, найден: %s.", 
				line, pos,stack.back().lexem,scaner->getLexem());
			_getch();
			exit(1);
		}

		void analyze(IM text)
		{
			int curType = 0;
			int curPos = 0;
			int line = 0;
			int pos = 0;
			stack.clear();
			stack.push_back(TOneSymb(true,TEnd,"конец файла"));
			stack.push_back(TOneSymb(false, TOper_programm,"программа"));

			currentType = scaner->scan(text);
			
			while (true){
				if (stack.back().isTerm){//в магазине терминал
					if (stack.back().type == currentType){
						if (stack.back().type == TEnd)
							break;
						else{
							stack.pop_back();
							currentType = scaner->scan(text);
						}
					} else {
						print_error();
						break;
					}
				} else {//В магазине нетерминал
					switch (stack.back().type)
					{
					case TOper_programm:{
						if (currentType == TClass){
							stack.pop_back();
							stack.push_back(TOneSymb(true, TCloseBrace, "}"));
							stack.push_back(TOneSymb(false, TOper_class_descr,"описание класса"));
							stack.push_back(TOneSymb(true, TOpenBrace, "{"));
							stack.push_back(TOneSymb(true, TMain, "Main"));
							stack.push_back(TOneSymb(true, TClass, "class"));
						} else print_error();
						break;
					}
					case TOper_class_descr:{
					    stack.pop_back();
						if (currentType == TClass){
							stack.push_back(TOneSymb(false, TOper_class_descr,"описание класса"));
							stack.push_back(TOneSymb(true, TCloseBrace, "}"));
							stack.push_back(TOneSymb(false, TOper_class_descr,"описание класса"));
							stack.push_back(TOneSymb(true, TOpenBrace, "{"));
							stack.push_back(TOneSymb(true, TIdentifier, "идентификатор"));
							stack.push_back(TOneSymb(true, TClass, "class"));
						}else if (currentType == TInt || currentType == TFloat || currentType == TIdentifier) {
							stack.push_back(TOneSymb(false, TOper_class_descr,"описние класса"));
							stack.push_back(TOneSymb(false, TOper_var_descr,"описание переменных"));
						} else if (currentType == TFunctionMain){
							stack.push_back(TOneSymb(false, TOper_class_descr, "описние класса"));
							stack.push_back(TOneSymb(false, TOper_main_function, "функция main"));
						}
						break;
					}
					case TOper_var_descr:{
						if (currentType == TInt || currentType == TFloat || currentType == TIdentifier){
							stack.pop_back();
							stack.push_back(TOneSymb(true, TSemicolons, ";"));
							stack.push_back(TOneSymb(false, TOper_var_list,"опсание переменных"));
							stack.push_back(TOneSymb(false, TOper_type,"тип"));
						} else print_error();
						break;
					}
					case TOper_type:{
						if (currentType == TInt){
							stack.pop_back();
							stack.push_back(TOneSymb(true, TInt, "int"));
						} else if (currentType == TFloat){
							stack.pop_back();
							stack.push_back(TOneSymb(true, TFloat,"float"));
						}
						else if (currentType == TIdentifier){
							stack.pop_back();
							stack.push_back(TOneSymb(true, TIdentifier,"идентификатор"));
						} else print_error();
					    break;
					}
					case TOper_assigment:{
						if (currentType == TIdentifier){
							scaner->getData(curPos, line, pos);
							curType = scaner->scan(text);
							if (curType == TAssigment){
								curType = scaner->scan(text);
								scaner->setData(curPos, line, pos);
								stack.pop_back();
								if (curType == TNew){
									stack.push_back(TOneSymb(true, TCloseParen, ")"));
									stack.push_back(TOneSymb(true, TOpenParen, "("));
									stack.push_back(TOneSymb(true, TIdentifier, "идентификатор"));
									stack.push_back(TOneSymb(true, TNew, "new"));
									stack.push_back(TOneSymb(true, TAssigment, " = "));
									stack.push_back(TOneSymb(false, TOper_operand, "операнд"));
								} else{
									stack.push_back(TOneSymb(false, TOper_expr1, "выражение"));
									stack.push_back(TOneSymb(true, TAssigment, " = "));
									stack.push_back(TOneSymb(false, TOper_operand, "операнд"));
								}
							} else print_error();
						}else print_error();
						break;
					}
					case TOper_var_list:{
						if (currentType == TIdentifier){
							stack.pop_back();
							stack.push_back(TOneSymb(false, TOper_right_list,"список переменных"));
							stack.push_back(TOneSymb(false, TOper_elem_list,"переменная"));
						} else print_error();
						break;
					}
					case TOper_right_list:{
						stack.pop_back();
						if (currentType == TComma){
							stack.push_back(TOneSymb(false, TOper_var_list,"список перемнных"));
							stack.push_back(TOneSymb(true, TComma, ","));
					    }
						break;
					}
					case TOper_elem_list:{//здесь есть конфликт, непонятно как код сработает
						if (currentType == TIdentifier){
							scaner->getData(curPos, line, pos);
							curType = scaner->scan(text);
							if (curType == TAssigment){
								curType = scaner->scan(text);
								scaner->setData(curPos, line, pos);
								stack.pop_back();
								if (curType == TNew){
									stack.push_back(TOneSymb(true, TCloseParen,")"));
									stack.push_back(TOneSymb(true, TOpenParen, "("));
									stack.push_back(TOneSymb(true, TIdentifier,"идентификатор"));
									stack.push_back(TOneSymb(true, TNew, "new"));
									stack.push_back(TOneSymb(true, TAssigment, "="));
									stack.push_back(TOneSymb(true, TIdentifier, "идентификатор"));
								} else {
									stack.push_back(TOneSymb(false, TOper_expr1,"выражение"));
									stack.push_back(TOneSymb(true, TAssigment, "="));
									stack.push_back(TOneSymb(true, TIdentifier,"идентификатор"));
								}
							} else if (curType == TOpenSqBracket){
								scaner->setData(curPos, line, pos);
								stack.pop_back();
								stack.push_back(TOneSymb(false,TOper_mass_size,"размерность массива"));
								stack.push_back(TOneSymb(true, TIdentifier,"идентификатор"));
							} else {
								scaner->setData(curPos, line, pos);
								stack.pop_back();
								stack.push_back(TOneSymb(true, TIdentifier));
							}
						}
						break;
					}
					case TOper_mass_size:{
						if (currentType == TOpenSqBracket){
							stack.pop_back();
							stack.push_back(TOneSymb(false, TOper_right_size_mass,"размер массива"));
							stack.push_back(TOneSymb(true, TCloseSqBracket,"]"));
							stack.push_back(TOneSymb(true, TIntConst,"целая константа"));
							stack.push_back(TOneSymb(true,TOpenSqBracket,"["));
						} else print_error();
						break;
					}
					case TOper_right_size_mass:{
						stack.pop_back();
						if (currentType == TOpenSqBracket){
							stack.push_back(TOneSymb(false, TOper_mass_size,"размер массива"));
							stack.push_back(TOneSymb(true, TCloseSqBracket, "]"));
							stack.push_back(TOneSymb(true, TIntConst,"целая константа"));
							stack.push_back(TOneSymb(false, TOpenSqBracket, "["));
						}
						break;
					}
					case TOper_main_function:{
						if (currentType == TFunctionMain){
							stack.pop_back();
							stack.push_back(TOneSymb(true, TCloseBrace,"}"));
							stack.push_back(TOneSymb(false, TOper_composite_oper,"составной оператор"));
							stack.push_back(TOneSymb(true, TOpenBrace, "{"));
							stack.push_back(TOneSymb(true, TCloseParen, ")"));
							stack.push_back(TOneSymb(true, TOpenParen, "("));
							stack.push_back(TOneSymb(true, TFunctionMain, "main"));
						} else print_error();
						break;
					}
					case TOper_composite_oper:{
					    stack.pop_back();
						if (currentType == TInt || currentType == TFloat){
							stack.push_back(TOneSymb(false, TOper_composite_oper,"составной оператор"));
							stack.push_back(TOneSymb(false, TOper_var_descr,"описание переменных"));
						} else if (currentType == TSwitch || currentType == TBreak || currentType == TOpenBrace || currentType == TSemicolons){
							stack.push_back(TOneSymb(false, TOper_composite_oper,"составной оператор"));
							stack.push_back(TOneSymb(false, TOper_operator, "оператор"));
						} else if (currentType == TIdentifier){
							scaner->getData(curPos, line, pos);
							curType = scaner->scan(text);
							scaner->setData(curPos, line, pos);
							if (curType == TAssigment){
								stack.push_back(TOneSymb(false, TOper_composite_oper,"составной оператор"));
								stack.push_back(TOneSymb(false, TOper_operator, "оператор"));
							} else {
								stack.push_back(TOneSymb(false, TOper_composite_oper,"составной оператор"));
								stack.push_back(TOneSymb(false, TOper_var_descr,"список переменных"));
							}
						}
						break;
					}
					case TOper_operator:{
						if (currentType == TSwitch){
							stack.pop_back();
							stack.push_back(TOneSymb(false, TOper_switch,"switch"));
						} else if (currentType == TIdentifier){
							stack.pop_back();
							stack.push_back(TOneSymb(false, TOper_assigment,"оператор присваивания"));
						} else if (currentType == TBreak){
							stack.pop_back();
							stack.push_back(TOneSymb(true, TSemicolons, ";"));
							stack.push_back(TOneSymb(true, TBreak,"break"));
						} else if (currentType == TOpenBrace){
							stack.pop_back();
							stack.push_back(TOneSymb(true, TCloseBrace, "}"));
							stack.push_back(TOneSymb(false, TOper_composite_oper, "составной оператор"));
							stack.push_back(TOneSymb(true, TOpenBrace, "{"));
						} else if (currentType == TSemicolons){
							stack.pop_back();
							stack.push_back(TOneSymb(true, TSemicolons, ";"));
						} else print_error();
						break;
					}
					case TOper_operand:{
						if (currentType == TIdentifier){
							stack.pop_back();
							stack.push_back(TOneSymb(false, TOper_right_operand,"операнд"));
							stack.push_back(TOneSymb(false, TOper_appeal_to_mass,"обращение к массиву"));
							stack.push_back(TOneSymb(true, TIdentifier,"идентификатор"));
						} else print_error();
						break;
					}
					case TOper_right_operand:{
						stack.pop_back();
						if (currentType == TPoint){
							stack.push_back(TOneSymb(false, TOper_operand,"операнд"));
							stack.push_back(TOneSymb(true, TPoint,"."));
						}
						break;
					}
					case TOper_appeal_to_mass:{
						stack.pop_back();
						if (currentType == TOpenSqBracket) {
							stack.push_back(TOneSymb(false, TOper_appeal_to_mass,"обращение к массиву"));
							stack.push_back(TOneSymb(true, TCloseSqBracket,"]"));
							stack.push_back(TOneSymb(false, TOper_expr1,"выражение"));
							stack.push_back(TOneSymb(true, TOpenSqBracket, "["));
						}
						break;
					}
					case TOper_switch:{
						if (currentType == TSwitch){
							stack.pop_back();
							stack.push_back(TOneSymb(true, TCloseBrace,"}"));
							stack.push_back(TOneSymb(false,TOper_case_block,"блок case"));
							stack.push_back(TOneSymb(true, TOpenBrace, "{"));
							stack.push_back(TOneSymb(true, TCloseParen, ")"));
							stack.push_back(TOneSymb(false, TOper_expr1,"выражение"));
							stack.push_back(TOneSymb(true, TOpenParen, "("));
							stack.push_back(TOneSymb(true, TSwitch, "switch"));
						} else print_error();
						break;
					}
					case TOper_case_block:{
						stack.pop_back();
						if (currentType == TCase){
							stack.push_back(TOneSymb(false, TOper_case_block,"блок case"));
							stack.push_back(TOneSymb(false, TOper_operator,"оператор"));
							stack.push_back(TOneSymb(true, TColons, ":"));
							stack.push_back(TOneSymb(true, TIntConst,"целая константа"));
							stack.push_back(TOneSymb(true, TCase, "case"));
						} else if (currentType == TDefault){
							stack.push_back(TOneSymb(false, TOper_operator,"оператор"));
							stack.push_back(TOneSymb(true, TColons, ":"));
							stack.push_back(TOneSymb(true, TDefault, "default"));
						}
						break;
					}
					case TOper_expr1:{
						if (currentType == TIdentifier || currentType == TFloatConst || currentType == TIntConst ||
							currentType == TOpenParen || currentType == TPluesPlus || currentType == TPlus ||
							currentType == TMinus || currentType == TMinusMinus){
							stack.pop_back();
							stack.push_back(TOneSymb(false, TOper_expr11,"выражение"));
							stack.push_back(TOneSymb(false, TOper_expr2,"выражение"));
						} else print_error();
						break;
					}
					case TOper_expr11:{
						stack.pop_back();
						if (currentType == TEqual || currentType == TNotEqual){
							stack.push_back(TOneSymb(false, TOper_expr1, "выражение"));
							stack.push_back(TOneSymb(true, currentType,"== или !="));
						}
						break;
					}
					case TOper_expr2:{
						if (currentType == TIdentifier || currentType == TFloatConst || currentType == TIntConst ||
							currentType == TOpenParen || currentType == TPluesPlus || currentType == TPlus ||
							currentType == TMinus || currentType == TMinusMinus){
							stack.pop_back();
							stack.push_back(TOneSymb(false, TOper_expr22, "выражение"));
							stack.push_back(TOneSymb(false, TOper_expr3, "выражение"));
						}
						else print_error();
						break;
					}
					case TOper_expr22:{
						stack.pop_back();
						if (currentType == TMore || currentType == TMoreOrEqual || currentType == TLess || currentType == TLessOrEqual){
							stack.push_back(TOneSymb(false, TOper_expr2, "выражение"));
							stack.push_back(TOneSymb(true, currentType, "<,>,<=,>="));
						}
						break;
					}
					case TOper_expr3:{
						if (currentType == TIdentifier || currentType == TFloatConst || currentType == TIntConst ||
							currentType == TOpenParen || currentType == TPluesPlus || currentType == TPlus ||
							currentType == TMinus || currentType == TMinusMinus){
							stack.pop_back();
							stack.push_back(TOneSymb(false, TOper_expr33, "выражение"));
							stack.push_back(TOneSymb(false, TOper_expr4, "выражение"));
						} else print_error();
						break;
					}
					case TOper_expr33:{
						stack.pop_back();
						if (currentType == TPlus || currentType == TMinus){
							stack.push_back(TOneSymb(false, TOper_expr3, "выражение"));
							stack.push_back(TOneSymb(true, currentType, "+,-"));
						}
						break;
					}
					case TOper_expr4:{
						if (currentType == TIdentifier || currentType == TFloatConst || currentType == TIntConst ||
							currentType == TOpenParen || currentType == TPluesPlus || currentType == TPlus ||
							currentType == TMinus || currentType == TMinusMinus){
							stack.pop_back();
							stack.push_back(TOneSymb(false, TOper_expr44, "выражение"));
							stack.push_back(TOneSymb(false, TOper_expr5, "выражение"));
						}else print_error();
					    break;
					}

					case TOper_expr44:{
						stack.pop_back();
						if (currentType == TDiv || currentType == TMod || currentType == TMultiple){
							stack.push_back(TOneSymb(false, TOper_expr1, "выражение"));
							stack.push_back(TOneSymb(true, currentType, "*,/,%"));
						}
						break;
					}
					case TOper_expr5:{
						if (currentType == TIdentifier || currentType == TFloatConst || currentType == TIntConst ||
							currentType == TOpenParen){
							stack.pop_back();
							stack.push_back(TOneSymb(false, TOper_expr6, "выражение"));
						}
						else if (currentType == TPlus || currentType == TMinus || currentType == TMinusMinus || 
							currentType == TPluesPlus){
							stack.pop_back();
							stack.push_back(TOneSymb(false, TOper_expr6, "выражение"));
							stack.push_back(TOneSymb(true, currentType, "+,-,++,--"));
						} else print_error();
						break;
					}
					case TOper_expr6:{
						if (currentType == TIdentifier || currentType == TFloatConst || currentType == TIntConst ||
							currentType == TOpenParen || currentType == TPluesPlus || currentType == TPlus ||
							currentType == TMinus || currentType == TMinusMinus){
							stack.pop_back();
							stack.push_back(TOneSymb(false, TOper_expr66, "выражение"));
							stack.push_back(TOneSymb(false, TOper_expr7, "выражение"));
						}else print_error();
						break;
					}
					case TOper_expr66:{
						stack.pop_back();
						if (currentType == TPluesPlus || currentType == TMinusMinus){
							stack.push_back(TOneSymb(true, currentType, "++, --"));
					    }
						break;
					}
					case TOper_expr7:{
						if (currentType == TIdentifier){
							stack.pop_back();
							stack.push_back(TOneSymb(false, TOper_operand));
						} else if (currentType == TFloatConst || currentType == TIntConst){
							stack.pop_back();
							stack.push_back(TOneSymb(true, currentType,"константа"));
						} else if (currentType == TOpenParen){
							stack.pop_back();
							stack.push_back(TOneSymb(true, TCloseParen, ")"));
							stack.push_back(TOneSymb(false, TOper_expr1, "выражение"));
							stack.push_back(TOneSymb(true, TOpenParen,"("));
						} else print_error();
						break;
					}
					}
				}
			}
		}
};

