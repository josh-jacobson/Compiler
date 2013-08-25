/**
 
 EECS 211 Program 8
 definitions.h
 
 Author:  	Josh Jacobson
 Contact:	joshj@u.northwestern.edu
 Due Date:	June 4, 2012
 
 */


#ifndef definitions_h
#define definitions_h


#define SYM_LIST_FULL 21
#define DUPLICATE_SYMBOL 22
#define SYMBOL_NOT_FOUND 23
#define END_OF_FILE 51
#define FILE_NOT_FOUND 24
#define BAD_TOKEN 25
#define MAX_LINE_LENGTH 256

#define NUMBER_OF_SPECIAL_TOKENS 18
#define VARTYPE 1
#define CONSTTYPE 2 
#define OPTYPE 3
#define PUNCTYPE 4
#define JMPTYPE 5

#define ASSIGNMENT 201
#define ADD 231
#define SUBTRACT 232
#define MULTIPLY 233
#define DIVIDE 234
#define LESS 235
#define GREATER 236
#define COMPARE_EQUAL 237 
#define TEST_IF 241
#define LOOP_WHILE 242
#define VAR 251
#define OUTPUT 271
#define SEMICOLON 301
#define LEFTPAREN 302
#define RIGHTPAREN 303
#define LEFTBRACKET 304
#define RIGHTBRACKET 305
#define ELSE_SYMBOL 306

#define MAXOPNUMBER 299

#define UNRECOGNIZED_SYMBOL 1
#define UNDEFINED_COMMAND 99

#define PARENBOOST 25
#define NUMBER_OF_OPERATORS 12
#define ASSIGNPREC 1
#define ADDPREC 5
#define SUBPREC 5
#define MULTPREC 6
#define DIVPREC 6
#define LESSPREC 4
#define GREATERPREC 4
#define EQUALPREC 4
#define IFPREC 1
#define WHILEPREC 1
#define VARPREC 1
#define COUTPREC 1

#define UNBALANCED_PARANTHESES 26
#define SYMBOL_LIST_FULL 29
#define SYMBOL_NOT_FOUND 30

#define STOPATFIRSTIFJMP 1
#define DONTSTOPATFIRSTIFJMP 0
#define DONTPROCESSJUMPS 0
#define PROCESSJUMPS 1
#define JUMP 243

#endif
