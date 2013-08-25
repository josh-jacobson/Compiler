/**
 
 EECS 211 Program 8
 symbols.h
 
 Author:  	Josh Jacobson
 Contact:	joshj@u.northwestern.edu
 Due Date:	June 4, 2012
 
 */

#ifndef symbols_h
#define symbols_h

class SYMBOL {
    
public: 
    SYMBOL(char* name);         // Constructor
//    ~SYMBOL();                  // Destructor
    virtual void print();               // Print the name of the symbol
    int isThisMyName(char* n);  // Return 1 if the argument is the same as the symbol argument, 0 otherwise
    void copyMyName(char** n);  // Allocate space in the given location and copy the symbol name

protected:
    char* symbolname;
};



// Derived classes:

class VARIABLE: public SYMBOL {
protected:
    int value;
    int undefined;
public:
    VARIABLE(char *n);
    void print();       // Overridden function!
    void setValue(int newvalue);
    int getValue();
    int amIUndefined();
};




class CONSTANT: public SYMBOL {
protected:
    int constantvalue;
public:
    CONSTANT(char* name, int value);
    void print();
    int getValue();
};



class OP: public SYMBOL {
protected:
    int operator_number;
    int operator_precedence;
public:
    OP(char* op, int opnumber, int precedence);
    void print();
    int getOpNumber();
    int getPrecedence();
};


class JMP: public SYMBOL {
protected:
    int forwardReference;
    int backwardReference;
public:
    JMP(char* name, int forwardref, int backwardref);
    void print();
    int getForwardReferenceNumber();
    int getBackwardReferenceNumber();
    void setForwardReferenceNumber(int forwardref);
};

#endif
