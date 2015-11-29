#pragma once
/*	
	Исходный модуль сканера лексем языка по варианту 22.
	Везде допили, браток, возвращение изображений лексем самих!
*/
#include <string.h>
#include "defines.h"

class Scaner
{
	#define LetterSmall ((t[currentPosition]>='a') && (t[currentPosition]<='z'))
    #define LetterBig ((t[currentPosition]>='A') && (t[currentPosition]<='Z'))
    #define Number ((t[currentPosition]>='0') && (t[currentPosition]<='9'))

    private:
		int currentPosition;
		int line;
		int position;
		LEX lexema;
	public:
		Scaner(void)
		{
			this->currentPosition = 0;
			this->line = 0;
			this->position = 0;
		}
		~Scaner(void){};

    int getCurrentPosition() const
	{
		return this->currentPosition;
	}

	int scan(IM t)
	{
		int tResult = -1;
		while(tResult < 0){
			char c = t[currentPosition];
			switch(t[currentPosition])
			{
			case '+':
				this->incCurrentAndPos();
				strcpy_s(lexema,"+");
				if(t[currentPosition]=='+'){
					strcpy_s(lexema,"++");
					this->incCurrentAndPos();
					tResult = TPluesPlus;
				}
				else tResult = TPlus;
				break;
			case '-':
				strcpy_s(lexema,"-");
				this->incCurrentAndPos();
				if(t[currentPosition]=='-'){
					strcpy_s(lexema,"--");
					position++;
					currentPosition++;
					tResult = TMinusMinus;
				}
				else tResult = TMinus;
				break;
			case '=':
				strcpy_s(lexema,"=");
				this->incCurrentAndPos();
				if(t[currentPosition]=='='){
					strcpy_s(lexema,"==");
					this->incCurrentAndPos();
					tResult =  TEqual;
				}
				else tResult =  TAssigment;
				break;
			case '!':
				this->incCurrentAndPos();
				if(t[currentPosition]=='='){
					strcpy_s(lexema,"!=");
					this->incCurrentAndPos();
					tResult =  TNotEqual;
				}
				else tResult =  TError;
				break;
			case '<':
				strcpy_s(lexema,"<");
				this->incCurrentAndPos();
				if(t[currentPosition]=='='){
					strcpy_s(lexema,"<=");
					this->incCurrentAndPos();
					tResult =  TLessOrEqual;
				}
				else tResult =  TLess;
				break;
			case '>':
				strcpy_s(lexema,">");
				this->incCurrentAndPos();
				if(t[currentPosition]=='='){
					strcpy_s(lexema,">=");
					this->incCurrentAndPos();
					tResult =  TLessOrEqual;
				}
				else tResult =  TMore;
				break;
			case '*':
				strcpy_s(lexema,"*");
				this->incCurrentAndPos();
				tResult =  TMultiple;
				break;
			case '%':
				strcpy_s(lexema,"%");
				this->incCurrentAndPos();
				tResult =  TMod;
				break;
			case '[':
				strcpy_s(lexema,"[");
				this->incCurrentAndPos();
				tResult =  TOpenSqBracket;
				break;
			case ']':
				strcpy_s(lexema,"]");
				this->incCurrentAndPos();
				tResult =  TCloseSqBracket;
				break;
			case '{':
				strcpy_s(lexema,"{");
				this->incCurrentAndPos();
				tResult =  TOpenBrace;
				break;
			case '}':
				strcpy_s(lexema,"}");
				this->incCurrentAndPos();
				tResult =  TCloseBrace;
				break;
			case '(':
				strcpy_s(lexema,"(");
				this->incCurrentAndPos();
				tResult =  TOpenParen;
				break;
			case ')':
				strcpy_s(lexema,")");
				this->incCurrentAndPos();
				tResult =  TCloseParen;
				break;
			case '.':
				strcpy_s(lexema,".");
				this->incCurrentAndPos();
				tResult =  TPoint;
				break;
			case ',':
				strcpy_s(lexema, ",");
				this->incCurrentAndPos();
				tResult = TComma;
				break;
			case ';':
				strcpy_s(lexema,";");
				this->incCurrentAndPos();
				tResult =  TSemicolons;
				break;
			case ':':
				strcpy_s(lexema,":");
				this->incCurrentAndPos();
				tResult =  TColons;
				break;
			case '\0':
				strcpy_s(lexema,"\0");
				tResult =  TEnd;
				break;
			case '\n':
				this->incCurrentAndPos();
				line++;
				position=0;
				break;
			case '/':
			    strcpy_s(lexema,"/");
				this->incCurrentAndPos();
				if(t[currentPosition]=='/')
				{
					//коммент однострочный
					while(t[currentPosition]!='\n' && t[currentPosition]!='\0')
					{
						currentPosition++;
					}
					if(t[currentPosition]=='\n')
						currentPosition++;
					position = 0;
					line++;
				}
				//коммент многострочный
				else if(t[currentPosition]=='*')
				{
					this->incCurrentAndPos();
					bool flag = true;
					while(flag)
					{
						char c = t[currentPosition];
						switch(t[currentPosition])
						{
						case '\n':
							this->incCurrentAndPos();
							line++;
							position=0;
							break;
						case '*':
							this->incCurrentAndPos();
							if(t[currentPosition]=='/')
							{
								this->incCurrentAndPos();
								flag = false;
							}
							break;
						case '\0':
							tResult = TError;
							flag = false;
							break;
						default:
							this->incCurrentAndPos();
						}
					}
				}
				else tResult =  TDiv;
				break;
			default:{
						//константы обработали
						if(Number)
						{
							int index = 0;
							while(Number)
							{
								lexema[index] = t[currentPosition];
								index++;
								this->incCurrentAndPos();
							}
							if(t[currentPosition]=='e' || t[currentPosition]=='E')
							{
								lexema[index] = t[currentPosition];
								index++;
								this->incCurrentAndPos();
								if(t[currentPosition]=='+'||t[currentPosition]=='-'){
									lexema[index] = t[currentPosition];
									index++;
									this->incCurrentAndPos();
								}
								if(!Number){
									tResult = TError;
								}
								else
								{
									while(Number){
										lexema[index] = t[currentPosition];
										index++;
										this->incCurrentAndPos();
									}
									tResult = TFloatConst;
								}
							}
							else tResult = TIntConst;
							lexema[index] = '\0';
						}
						else if(LetterBig || LetterSmall)
						{
							int index = 0;
							while((LetterSmall || LetterBig || Number) && index < 256)
							{
								lexema[index] = t[currentPosition];
								index++;
								this->incCurrentAndPos();
							}
							//здесь то лишнее мы и отрезали
							while(LetterSmall || LetterBig || Number)
							{
								this->incCurrentAndPos();
							}
							lexema[index] = '\0';
							tResult = getIdentifierTipe(lexema);
						}
						//здесь игнорим всё, что раньше пропускали
						else this->incCurrentAndPos();
					}
			}	
		}

		return tResult;
	}

	private: void incCurrentAndPos()
			 {
				 currentPosition++;
				 position++;
			 }
	private: int getIdentifierTipe(LEX identifaer)
			 {
				 if(!strcmp(identifaer,"switch"))
					 return  TSwitch;
				 if(!strcmp(identifaer,"case"))
					 return  TCase;
				 if(!strcmp(identifaer,"break"))
					 return  TBreak;
				 if(!strcmp(identifaer,"Main"))
					 return  TMain;
				 if(!strcmp(identifaer,"void"))
					 return  TVoid;
				 if(!strcmp(identifaer,"int"))
					 return  TInt;
				 if(!strcmp(identifaer,"float"))
					 return  TFloat;
				 if(!strcmp(identifaer,"main"))
					 return  TFunctionMain;
				 if(!strcmp(identifaer,"class"))
					 return  TClass;
				 if (!strcmp(identifaer, "new"))
					 return TNew;
				 if (!strcmp(identifaer, "default"))
					 return  TDefault;
				 return TIdentifier;
			 }

	public:
		int getLine() const
		{
			return this->line;
		}
		int getPosition() const
		{
			return this->position;
		}
		char *getLexem()
		{
			return this->lexema;
		}

		int getCurrentPos(){
			return this->currentPosition;
		}

		void getData(int &curPos, int &line, int &pos){
			curPos = currentPosition;
			line = this->line;
			pos = position;
		}

		void setData(int &curPos, int &line, int &pos){
			currentPosition = curPos;
			this->line = line;
			this->position = position;
		}
};

