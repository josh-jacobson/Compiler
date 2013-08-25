/**
 
 EECS 211 Program 8
 symbols.cpp
 
 Author:  	Josh Jacobson
 Contact:	joshj@u.northwestern.edu
 Due Date:	June 4, 2012
 
 */

#include <iostream>
#include <string.h>
#include "symbols.h"
#include <stdlib.h>
using namespace std;


SYMBOL::SYMBOL(char* name) {
    int amountofspace = (strlen(name)+1)*sizeof(char);
    symbolname = (char *)malloc(amountofspace);
    strcpy(symbolname,name);
    *(symbolname + amountofspace) = '\0';
}

/**
SYMBOL::~SYMBOL(void) {
	free((void*) symbolname);
    symbolname = NULL;
}
 */


void SYMBOL::print() {
    cout << symbolname;
}


int SYMBOL::isThisMyName(char* n){
    // Return 1 if the argument is the same as the symbol name, 0 otherwise
    if (strlen(n) != strlen(symbolname)) return 0;      // Return 0 if lengths different
    //Lengths the same:
    if (strcmp(n,symbolname) != 0) return 0;
    return 1;
}


void SYMBOL::copyMyName(char **n) {
	int len;
	len = strlen(symbolname);
	(*n) = (char *)malloc(len+1);
	strcpy(*n, symbolname);
	(*n)[len] = 0;
}



// Derived class VARIABLE:

VARIABLE::VARIABLE(char *n) : SYMBOL(n) {  // Constructor
    undefined = 1; // mark as undefined
}


void VARIABLE::print(){
    cout << "Variable: ";
    SYMBOL::print();
    if (undefined == 1) cout << "\nThis variable has never been assigned a value.\n";
    else cout << "\nThe current value of this variable is " << value << "\n";
}

void VARIABLE::setValue(int newvalue){
    value = newvalue;
    undefined = 0; // If this variable was undefined, it has now been assigned a value.
}


int VARIABLE::getValue(){
    if (undefined == 1) return -99999;
    else return value;
}

int VARIABLE::amIUndefined() {
    if (undefined==0) return 0;
    else return 1;
    //return undefined;   // return 0 if defined, 1 if undefined
}


// Derived class CONSTANT:

CONSTANT::CONSTANT(char* name, int value) : SYMBOL(name) {
    constantvalue = value;
}

void CONSTANT::print() {
    cout << "Constant: ";
    SYMBOL::print();
    cout << "\nConstant value:" <<constantvalue <<"\n";
}

int CONSTANT::getValue() {
    return constantvalue;
}



// Derived class OP:



OP::OP(char* op, int opnumber, int precedence) : SYMBOL(op) {
    operator_number = opnumber;
    operator_precedence = precedence;
}
void OP::print() {
    cout << "Operator: ";
    SYMBOL::print();
    cout << "\nOperator number: " << operator_number << "\nOperator precedence: " << operator_precedence << "\n";
}

int OP::getOpNumber() {
    return operator_number;
}

int OP::getPrecedence() {
    return operator_precedence;
}


// Derived class JMP:

JMP::JMP(char* name, int forwardref, int backwardref) : SYMBOL(name) {
    forwardReference = forwardref;
    backwardReference = backwardref;
}

int JMP::getForwardReferenceNumber() {
    return forwardReference;
}

void JMP::print() {
    cout << "Jump: ";
    SYMBOL::print();
    cout << "\nForward Reference: " << forwardReference << "\nBackward Reference: " << backwardReference << "\n";
}

int JMP::getBackwardReferenceNumber() {
    return backwardReference;
}

void JMP::setForwardReferenceNumber(int forwardref) {
    forwardReference = forwardref;
}