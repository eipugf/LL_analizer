#pragma once
/*	
	Исходный модуль сканера лексем языка по варианту 22.
	Везде допили, браток, возвращение изображений лексем самих ёпта!
*/
#define MaxText 10000
#define MaxLex 257
typedef char IM[MaxText];
typedef char LEX[MaxLex];
#include <string.h>

//возвращаемые коды
#define TIntConst 1
#define TFloatConst 2
#define TPlus 3
#define TPluesPlus 4
#define TMinus 5
#define TMinusMinus 6
#define	TAssigment 7
#define TEqual 8
#define TMoreOrEqual 9
#define TLessOrEqual 10
#define TNotEqual 11
#define TMultiple 12
#define TMod 13
#define TDiv 14
#define TIdentifier 15
#define TSwitch 16
#define TCase 17
#define TBreak 18
#define TMain 19
#define TVoid 20
#define TInt 21
#define TFloat 22
#define TFunctionMain 23
#define TClass 24
#define TNew 25
#define TComma 26
#define TPoint 27
#define TOpenParen 28
#define TCloseParen 29
#define TColons 30
#define TOpenSqBracket 31
#define TCloseSqBracket 32
#define TMore 33
#define TLess 34
#define TOpenBrace 35
#define TCloseBrace 36

#define TEnd	1000
#define TError	2000

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
				strcpy(lexema,"+");
				if(t[currentPosition]=='+'){
					strcpy(lexema,"++");
					this->incCurrentAndPos();
					tResult = TPluesPlus;
				}
				else tResult = TPlus;
				break;
			case '-':
				strcpy(lexema,"-");
				this->incCurrentAndPos();
				if(t[currentPosition]=='-'){
					strcpy(lexema,"--");
					position++;
					currentPosition++;
					tResult = TMinusMinus;
				}
				else tResult = TMinus;
				break;
			case '=':
				strcpy(lexema,"=");
				this->incCurrentAndPos();
				if(t[currentPosition]=='='){
					strcpy(lexema,"==");
					this->incCurrentAndPos();
					tResult =  TEqual;
				}
				else tResult =  TAssigment;
				break;
			case '!':
				this->incCurrentAndPos();
				if(t[currentPosition]=='='){
					strcpy(lexema,"!=");
					this->incCurrentAndPos();
					tResult =  TNotEqual;
				}
				else tResult =  TError;
				break;
			case '<':
				strcpy(lexema,"<");
				this->incCurrentAndPos();
				if(t[currentPosition]=='='){
					strcpy(lexema,"<=");
					this->incCurrentAndPos();
					tResult =  TLessOrEqual;
				}
				else tResult =  TLess;
				break;
			case '>':
				strcpy(lexema,">");
				this->incCurrentAndPos();
				if(t[currentPosition]=='='){
					strcpy(lexema,">=");
					this->incCurrentAndPos();
					tResult =  TLessOrEqual;
				}
				else tResult =  TMore;
				break;
			case '*':
				strcpy(lexema,"*");
				this->incCurrentAndPos();
				tResult =  TMultiple;
				break;
			case '%':
				strcpy(lexema,"%");
				this->incCurrentAndPos();
				tResult =  TMod;
				break;
			case '[':
				strcpy(lexema,"[");
				this->incCurrentAndPos();
				tResult =  TOpenSqBracket;
				break;
			case ']':
				strcpy(lexema,"]");
				this->incCurrentAndPos();
				tResult =  TCloseSqBracket;
				break;
			case '{':
				strcpy(lexema,"{");
				this->incCurrentAndPos();
				tResult =  TOpenBrace;
				break;
			case '}':
				strcpy(lexema,"}");
				this->incCurrentAndPos();
				tResult =  TCloseBrace;
				break;
			case '(':
				strcpy(lexema,"(");
				this->incCurrentAndPos();
				tResult =  TOpenParen;
				break;
			case ')':
				strcpy(lexema,")");
				this->incCurrentAndPos();
				tResult =  TCloseParen;
				break;
			case '.':
				strcpy(lexema,".");
				this->incCurrentAndPos();
				tResult =  TPoint;
				break;
			case ';':
				strcpy(lexema,";");
				this->incCurrentAndPos();
				tResult =  TComma;
				break;
			case ':':
				strcpy(lexema,":");
				this->incCurrentAndPos();
				tResult =  TColons;
				break;
			case '\0':
				strcpy(lexema,"\0");
				tResult =  TEnd;
				break;
			case '\n':
				this->incCurrentAndPos();
				line++;
				position=0;
				break;
			case '/':
			    strcpy(lexema,"/");
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
							else tResult = TInt;
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
				 if(!strcmp(identifaer,"new"))
					 return  TNew;
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
};

