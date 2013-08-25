/**
 
 EECS 211 Program 8
 system_utilites.h
 
 Author:  	Josh Jacobson
 Contact:	joshj@u.northwestern.edu
 Due Date:	June 4, 2012
 
 */
#include "symbols.h"
#include "definitions.h"
#include "symbol_list.h"

#ifndef system_utilities_h
#define system_utilities_h

void printError(int x);
int openInputFile(char fname[]);
int getNextToken(char** tok);
int isSingleChar(char n);
int isDigit(char n);
int isLetter(char n);
void fillSpecialTokenList();
int getTokenNumber(char* s);
int symbolType(char *s);
int convertStringToValue(char *s);
void fillPrecedenceList();
int getOperatorPrecedence(int op);
int popOperatorStack(symbolStack* stk, symbolList *postfix, int prec, int whetherToStopAfterFirstIfJump, int whetherToProcessJumps);
void checkForIfWhile(symbolStack *stk, symbolList *postfix);

#endif
