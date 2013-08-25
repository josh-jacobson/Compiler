/**
 
 EECS 211 Program 8
 symbol_list.h
 
 Author:  	Josh Jacobson
 Contact:	joshj@u.northwestern.edu
 Due Date:	June 4, 2012
 
 */


#ifndef symbol_list_h
#define symbol_list_h

#include "symbols.h"
#define LISTLEN 200
#define ALLOWDUPLICATES 1
#define DONTALLOWDUPLICATES 2


class symbolList {
public:
    symbolList(int duplicates_allowed);
    // ~symbolList();
    void print();
    int addSymbol(SYMBOL* n);
    int getSymbol(char* name, SYMBOL** s);
    int removeSymbol(char* name);
    SYMBOL* retrieveElement(int j);
    void setVariableValue(char *n, int v);
    void getVariableValue(char *n, int *v, int *u);
    
    int getNextOpenPositionNumber();
    int getCurrentPosition();
    void replaceSymbol(SYMBOL* s, int k);
protected:
    SYMBOL* symbolarray[LISTLEN];
    int number_of_symbols_in_list;
    int duplicate_symbols_allowed;
    
    
};

class symbolStack {
private:
    SYMBOL* stack[LISTLEN];
    int number_of_elements;
public:
    symbolStack();
    void print();
    int push(SYMBOL *n);
    int copyTopLevel(SYMBOL **copyto);
    int pop();
    int isempty();
    int getTopLevelValue(int *v);
    void updateWhileJmpForwardRef(int forwardref);
};



#endif
