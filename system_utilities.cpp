/**
 
 EECS 211 Program 8
 system_utilities.cpp
 
 Author:  	Josh Jacobson
 Contact:	joshj@u.northwestern.edu
 Due Date:	June 4, 2012
 
 Note: this file includes Program 5 code written by LJH for getNextToken.
 */

#include <iostream>
#include "system_utilities.h"
#include <iostream>
#include <fstream>
using namespace std;

ifstream infile;
char line[MAX_LINE_LENGTH];	// Holds characters on one line up to limit
int  lineLength=0;		// Number of characters on the current line
int  currentPosition=1;		// Current position on the line, ie the last position

void printError(int x) {
    switch (x)
    {
        case 21:
            cout << "Symbol list full. \n";
            break;
        case 22:
            cout << "Duplicate Symbol. \n";
            break;
        case 23:
            cout << "Symbol not found. \n";
            break;
        case 24:
            cout << "File not found. \n";
            break;
        case 25:
            cout << "Bad token. \n";
            break;
        case 51:
            cout << "End of File. \n";
            break;
        case 26:
            cout << "***ERROR: Unbalanced Parentheses. \n";
            break;
        default:
            cout << "***ERROR*** \n";
    }
}

int openInputFile(char fname[]) {
    infile.open(fname,ios::in);
    if (infile.fail()) return 24;       // File not found
    else return 0;
}

int getNextToken(char **token) {
	// Tokens are of three kinds:
	//	single character:   = + - * / ; ( ) { } < >
	//	numbers:  all digits
	//	other (including variables and special keywords):
	//		starts with a letter and includes letters and digits
    
	int  tmpPos;		// Searches through the characters on the input line.
	int  endPos;		// Marks the end (last character) of the token.
	char ch;		// Utility variable
	int  tLen;		// The number of characters in the token.
    
    
	// Find non_blank character.  If we reach the end of the line, try to read
	// another line and start at the beginning again.
	tmpPos = currentPosition;
	do {
		tmpPos += 1;
		if(tmpPos>=lineLength || line[tmpPos]=='\n') {
			if(infile.eof()) return END_OF_FILE;
			memset(line,0,sizeof(line));
			infile.getline(line,sizeof(line));
            cout << line << "\n";
			lineLength = strlen(line);
			currentPosition = 0;
			tmpPos = 0;
		}
	} while(line[tmpPos]==' ');
    
    
	// Check first for single character token.
	ch = line[tmpPos];
	if  (  (ch=='=')  || (ch=='+') ||  (ch=='-')  ||
	     (ch=='*')  || (ch=='/') ||  (ch=='(')  ||
	     (ch==')')  || (ch=='{') ||  (ch=='}')  ||
	     (ch==';') || (ch=='<') || (ch=='>')
         ) { // Here for a single-character token.
        endPos = tmpPos+1;
        if (ch=='=')
        {
            ch = line[endPos];
            if (ch=='=') endPos++;
        }
    }
    
	else if( (ch>='0') && (ch<='9') ) {
		// Here if its a number.
		endPos = tmpPos+1;
		ch = line[endPos];
		while( (ch>='0') && (ch<='9') ) {
			endPos += 1;
			ch = line[endPos];
		}
	}
    
	else if( ((ch>='a') && (ch<='z'))  ||  ((ch>='A') && (ch<='Z')) ) {
		// Starts with a letter
		endPos = tmpPos+1;
		ch = line[endPos];
		while( ((ch>='a') && (ch<='z'))  ||  ((ch>='A') && (ch<='Z'))  ||  ((ch>='0') && (ch<='9')) ) {
			endPos += 1;
			ch = line[endPos];
		}
	}
    
	else {
		// Bad token.  Skip to next position in line and return error code.
		currentPosition = tmpPos+1;
		return BAD_TOKEN;
	}
    
    
	// Here if succeeded in finding a token.  tmpPos is location of first chanracter,
	//  and endPos the location AFTER of the last character.   Copy the token out and
	//  reset currentPosition for next time.
	tLen = endPos-tmpPos;				// Number of actual characters in token
	(*token) = (char *) malloc(tLen+1);		// malloc one extra for string terminator
	memcpy(*token,line+tmpPos,tLen);		// Copy the characters
	(*token)[tLen] = 0;				// Add the string terminator 0.
	currentPosition = endPos-1;
    
	return 0;		// Indicate token successfully found.
    
}



/** The following is my getNextToken function, which is not used in this program.
int getNextToken(char** tok) {
    *tok = (char *)malloc(MAX_LINE_LENGTH);
    char potential[MAX_LINE_LENGTH];
    for (int j=0; j<MAX_LINE_LENGTH; j++) potential[j] = '/0';
    int i = 0;
    int end = 0;                        // end = 1 when the end of token is reached
    
    // Find first non-blank character
    do {
        if (infile.eof()) return 51;    // Test for end of file
        infile.get(potential[0]);
    } while ((potential[0] == ' ') || (potential[i] == '\n') || (potential[0] == '\0'));

    // 1. Single character tokens:
    if (isSingleChar(potential[0]) == 1) end = 1;
    else {
        // Check for bad tokens:
        if ((potential[0] != '=')&&(isDigit(potential[0]) == 0)&&(isLetter(potential[0]) == 0)) return 25;
        //= and ==
        if (potential[0] == '=') {
            i = 1;
            infile.get(potential[i]); 
            if (potential[1] == '=') { // ==
                i++;
            }
            else infile.unget();
            end = 1;
        }
        // 2. Digits
        if (isDigit(potential[0]) == 1) {
            i=1;
            do {
                infile.get(potential[i]);
                if (!infile.eof() && (isDigit(potential[i])==1)) i++;
                else {
                    end = 1;
                    infile.unget();
                }
            } while (end==0);    
        }
        // 3. Letters
        if (isLetter(potential[0]) == 1) {
            i=1;
            do {
                infile.get(potential[i]);
                if (!infile.eof() && ((isLetter(potential[i])==1)||(isDigit(potential[i])==1)) ) i++;
                else {
                    end = 1;
                    infile.unget();
                }
            } while (end==0);
        }
    }
    memcpy(*tok,potential,max(i,1));
    return 0;
}
 */

int isSingleChar(char n) {
    // Return 1 for single character tokens, 0 otherwise
    if (n == '+') return 1;
    if (n == '-') return 1;
    if (n == '*') return 1;
    if (n == '/') return 1;
    if (n == '<') return 1;
    if (n =='>') return 1;
    if (n == ';') return 1;
    if (n == ';') return 1;
    if (n == '(') return 1;
    if (n == ')') return 1;
    if (n == '{') return 1;
    if (n == '}') return 1;
    return 0;
}


int isDigit(char n) {
    // Return 1 for digit, 0 otherwise
    if((n>='0')&&(n<='9')) return 1;
    return 0;
}


int isLetter(char n) {
    // Return 1 for letter, 0 otherwise    
    if ((n>='a')&&(n<='z')) return 1;   // lowercase letters
    if ((n>='A')&&(n<='Z')) return 1;   // uppercase letters 
    return 0;
}






class specialToken {
private:
    char* token;
    int token_name_code;
public:
    specialToken(char* tok, int name_code);
//    ~specialToken();
    int amIThisToken(char* tok);
    int getMyTokenNumber();
};

specialToken::specialToken(char* tok, int name_code) {
    int amountofspace = (strlen(tok)+1)*sizeof(char);
    token = (char *)malloc(amountofspace);
    strcpy(token,tok);
    token_name_code = name_code;
}
/**
specialToken::~specialToken() {
    free((void*) token);
    token = NULL;
}
 
 */

int specialToken::amIThisToken(char* tok) {
    // Return 1 if argument matches token string, 0 otherwise
    if (strlen(tok) != strlen(token)) return 0;      // Return 0 if lengths different
    //Lengths the same:
    if (strcmp(tok,token) != 0) return 0;
    return 1;
}

int specialToken::getMyTokenNumber() {
    return token_name_code;
}


specialToken* specialTokenList[NUMBER_OF_SPECIAL_TOKENS];


void fillSpecialTokenList() {
    specialTokenList[0] = new specialToken("=", ASSIGNMENT);
    specialTokenList[1] = new specialToken("+", ADD);
    specialTokenList[2] = new specialToken("-", SUBTRACT);
    specialTokenList[3] = new specialToken("*", MULTIPLY);
    specialTokenList[4] = new specialToken("/", DIVIDE);
    specialTokenList[5] = new specialToken("<", LESS);
    specialTokenList[6] = new specialToken(">", GREATER);
    specialTokenList[7] = new specialToken("==", COMPARE_EQUAL);
    specialTokenList[8] = new specialToken("if", TEST_IF);
    specialTokenList[9] = new specialToken("while", LOOP_WHILE);
    specialTokenList[10] = new specialToken("VAR", VAR);
    specialTokenList[11] = new specialToken("cout", OUTPUT);
    specialTokenList[12] = new specialToken(";", SEMICOLON);
    specialTokenList[13] = new specialToken("(", LEFTPAREN);
    specialTokenList[14] = new specialToken(")", RIGHTPAREN);
    specialTokenList[15] = new specialToken("{", LEFTBRACKET);
    specialTokenList[16] = new specialToken("}", RIGHTBRACKET);
    specialTokenList[17] = new specialToken("else", ELSE_SYMBOL);
}

int getTokenNumber(char* s) {
    for (int i = 0; i < NUMBER_OF_SPECIAL_TOKENS; i++) {
        int test = (*specialTokenList[i]).amIThisToken(s);
        if (test == 1) return (*specialTokenList[i]).getMyTokenNumber();
    }
    return UNDEFINED_COMMAND;
}


int symbolType(char *s) {
    // Return JMPTYPE, CONSTTYPE, VARTYPE, OPTYPE, OR PUNCTYPE depending on the string that s points to
    if ((strcmp(s,"forwardjmp") == 0) || (strcmp(s,"backwardjmp") == 0)) return JMPTYPE;
    int num = getTokenNumber(s);
    if (num != UNDEFINED_COMMAND) {
        if (num <= MAXOPNUMBER) return OPTYPE;
        else return PUNCTYPE;
    }
    else {
        if (isDigit(*s) == 1) return CONSTTYPE; // first character of s is a digit
        if (isLetter(*s) == 1) return VARTYPE;  // first character of s is a letter
    }
}


int convertStringToValue(char *s){
    int value = 0;
    int digit;
    for (int i = 0; i< strlen(s); i++) {
        digit = *(s + i) - 48;
        value = 10*value + digit;
    }
    return value;
}


// Program 6:

class operatorPrecedence {
private:
    int opnumber;
    int naturalprecedence;
public:
    operatorPrecedence(int opnumber, int precedence);
    int isThisMyOpNumber(int n);
    int getPrecedence();
};

operatorPrecedence::operatorPrecedence(int number, int precedence) {
    opnumber = number;
    naturalprecedence = precedence;
}
int operatorPrecedence::isThisMyOpNumber(int n) {
    // Return 1 if input n equals the operator number, 0 otherwise.
    if (n == opnumber) return 1;
    else return 0;
}

int operatorPrecedence::getPrecedence() {
    return naturalprecedence;
}

operatorPrecedence* precedencelist[NUMBER_OF_OPERATORS];

void fillPrecedenceList() {
    precedencelist[0] = new operatorPrecedence(ASSIGNMENT,ASSIGNPREC);
    precedencelist[1] = new operatorPrecedence(ADD, ADDPREC);
    precedencelist[2] = new operatorPrecedence(SUBTRACT, SUBPREC);
    precedencelist[3] = new operatorPrecedence(MULTIPLY, MULTPREC);
    precedencelist[4] = new operatorPrecedence(DIVIDE, DIVPREC);
    precedencelist[5] = new operatorPrecedence(LESS, LESSPREC);
    precedencelist[6] = new operatorPrecedence(GREATER, GREATERPREC);
    precedencelist[7] = new operatorPrecedence(COMPARE_EQUAL,EQUALPREC);
    precedencelist[8] = new operatorPrecedence(TEST_IF, IFPREC);
    precedencelist[9] = new operatorPrecedence(LOOP_WHILE, WHILEPREC);
    precedencelist[10] = new operatorPrecedence(VAR, VARPREC);
    precedencelist[11] = new operatorPrecedence(OUTPUT, COUTPREC);
}

int getOperatorPrecedence(int op) {
    for (int i = 0; i < NUMBER_OF_OPERATORS; i++) {
        int test = (*precedencelist[i]).isThisMyOpNumber(op);
        if (test == 1) return (*precedencelist[i]).getPrecedence();
    }
    return UNRECOGNIZED_SYMBOL;
}


int popOperatorStack(symbolStack* stk, symbolList *postfix, int prec, int whetherToStopAfterFirstIfJump, int whetherToProcessJumps) {
    int end = 0;
    SYMBOL* top;
    JMP* jump;
    CONSTANT* tempconstant;
    int toknumber;
    char *tokname;
    int precedence;
    int position;
    if (stk->isempty() == 1) end = 1;
    
    while (end == 0) {
        stk->copyTopLevel(&top);    // Copy top level of stack to top
        top->copyMyName(&tokname);
        toknumber = getTokenNumber(tokname);
        if (toknumber == LEFTBRACKET) {
            return 0;
        }
        if ((strcmp(tokname,"ifjump") == 0) || (strcmp(tokname,"whilejump") == 0) || (strcmp(tokname,"elsejump") == 0)) {
            if (whetherToProcessJumps == DONTPROCESSJUMPS) return 0;
            else {
                
                if (strcmp(tokname,"ifjump") == 0) {
                    position = postfix->getNextOpenPositionNumber();
                    tempconstant = new CONSTANT("0ifjumpaddress",postfix->getCurrentPosition());
                    postfix->replaceSymbol(tempconstant, position);
                    stk->pop();
                    if (whetherToStopAfterFirstIfJump == STOPATFIRSTIFJMP) end = 1;
                    // Stop after processing first ifjump for case of if with no else statement
                }
                
                if (strcmp(tokname,"whilejump") == 0) {
                    
                    // Create backward jump:
                    int backwardref = ((JMP *) top)->getBackwardReferenceNumber();
                    jump = new JMP("backwardjmp",0,backwardref);    // Create new unconditional jump pointing to position before while condition
                    postfix->addSymbol(jump);
                    stk->pop();
                    
                    // Now, fill in space with position to jump to if while test fails:
                    position = postfix->getNextOpenPositionNumber();
                    tempconstant = new CONSTANT("0whilejumpaddress",postfix->getCurrentPosition());
                    postfix->replaceSymbol(tempconstant, position);
                }
                
                if (strcmp(tokname,"elsejump") == 0) {
                    position = ((JMP *) top)->getForwardReferenceNumber();   // This is the address at which the temporary jump object was placed.
                    jump = new JMP("forwardjmp",postfix->getCurrentPosition(),0); // Create a new unconditional jump object that will point until after the code inside the else brackets.
                    postfix->replaceSymbol(jump, position);     // Place this new object in the sapce left for it.
                    stk->pop();
                    end = 1;
                }
                
                
            }
        }
        else { // not a jump
            precedence = ((OP *) top)->getPrecedence();
        
            if (precedence >= prec) {
                postfix->addSymbol(top);
                stk->pop();
                if (stk->isempty() == 1) end = 1;
            }
            else end = 1;
            
        }
        if (stk->isempty() == 1) end = 1;
    }
    
    return 0;
}





void checkForIfWhile(symbolStack *stk, symbolList *postfix) {
    SYMBOL* top;
    JMP* jump;
    char *tokname;
    if (stk->isempty() != 1) {
        stk->copyTopLevel(&top);    // Copy top level of stack to top
        top->copyMyName(&tokname);
        int isIf = strcmp(tokname,"if");            //isIf = 0 for if token
        int isWhile = strcmp(tokname, "while");     //isWhile = 0 for while token
        if ((isIf == 0) || (isWhile == 0)) {
            stk->pop(); // Pop operator off of operator stack
            // Add a space for a constant indicating the address to skip to if the test is false:
            postfix->addSymbol(NULL);
            

            // Add the operator
            if (isIf == 0) {    // if token
                OP* ifoperator = new OP("if",TEST_IF,IFPREC);
                jump = new JMP("ifjump",postfix->getCurrentPosition(),0);
                stk->push(jump);
                postfix->addSymbol(ifoperator);
            }
            else {              // while token
                stk->updateWhileJmpForwardRef(postfix->getCurrentPosition());
                OP* whileoperator = new OP("while",LOOP_WHILE,WHILEPREC);
                postfix->addSymbol(whileoperator);
            }
        }        
    }

    
}