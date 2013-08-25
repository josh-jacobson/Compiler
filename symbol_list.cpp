/**

 EECS 211 Program 8
 symbol_list.cpp

 Author:  	Josh Jacobson
 Contact:	joshj@u.northwestern.edu
 Due Date:	June 4, 2012

*/

#include <iostream>
#include "symbols.h"
#include "symbol_list.h"
#include "system_utilities.h"
using namespace std;

extern symbolList variables;


symbolList::symbolList(int d) {          // Constructor
    duplicate_symbols_allowed = d;       // Argument d is either ALLOWDUPLICATES or DONTALLOWDUPLICATES
    number_of_symbols_in_list = 0;
}

/**

symbolList::~symbolList(void) {         // Destructor
   
    for (int i=0; i<number_of_symbols_in_list; i++); // delete symbolarray[i];
    

    if (duplicate_symbols_allowed==DONTALLOWDUPLICATES) {   // Duplicates not allowed
        for (int i=0; i<number_of_symbols_in_list; i++) {
            delete symbolarray[i];                          // Free memory to prevent memory leak
            symbolarray[i] = NULL;                          // Un-dangle pointer
        }
    }
    else {                              // Duplicates Allowed
        char* uniquename;               // Store unique name to check for duplicates        
        symbolarray[0]->copyMyName(&uniquename);
        int unique_symbol_indices[LISTLEN]; // Use this array to mark unique elements for deletion after scanning is complete
        unique_symbol_indices[0]=1;         // Enter 1 in corresponding spot for a unique element, 0 for a duplicate element
        
        for (int i=1; i<number_of_symbols_in_list; i++) {
            
            if (symbolarray[i]->isThisMyName(uniquename)!=1) {// Unique symbol
                symbolarray[i]->copyMyName(&uniquename);      // copy new unique name to uniquename variable
                unique_symbol_indices[i] = 1;                 // Mark new unique name for deletion
            }
            else {
                unique_symbol_indices[i] = 0;                 // Duplicate symbol -- object already marked for deletion
                symbolarray[i] = NULL;                        // Un-dangle pointer for duplicate symbol
            }
        }
        // Now delete unique symbols:
        for (int k=0; k<number_of_symbols_in_list; k++) {
            if (unique_symbol_indices[k]==1) {
                delete symbolarray[k];
                symbolarray[k] = NULL;
            }
        }
    }

}
 
    */



void symbolList::print() {
    if (number_of_symbols_in_list == 0) cout << "There are no symbols in this list.\n";
    else {
        cout << "Printing SYMBOL list: \n";
        for (int i=0; i<number_of_symbols_in_list; i++) {
            cout << i << "     ";
            symbolarray[i]->print();
            cout <<"\n";
        }
    }
}



int symbolList::addSymbol(SYMBOL* n) {
    if (number_of_symbols_in_list == LISTLEN) return 21;  // Symbol list full
    else {
        if (duplicate_symbols_allowed==DONTALLOWDUPLICATES) {   // Duplicates not allowed
            char* temp;
            n->copyMyName(&temp);   // Copy symbolname to temporary variable for use in the following scan for duplicates
            for (int i=0; i<number_of_symbols_in_list; i++) {   // Scan for duplicates
                if (symbolarray[i]->isThisMyName(temp) == 1) return 22; // If duplicate return defined constant DUPLICATE_SYMBOL
            }
        }
        // Duplicates allowed or no duplicates found, so now add symbol to list
        symbolarray[number_of_symbols_in_list] = n;
        number_of_symbols_in_list++;        // Increment symbol list count
        return 0;
        
    }
}



int symbolList::getSymbol(char* searchname, SYMBOL** s) {
    for (int i=0; i<number_of_symbols_in_list; i++) {   // Scan for symbols with symbolname=searchname
        if (symbolarray[i]->isThisMyName(searchname) == 1) {
            int amountofspace = sizeof(*symbolarray[i]);
            *s = (SYMBOL *)malloc(amountofspace);
            //s = &symbolarray[i];
            memcpy(*s,symbolarray[i],sizeof(SYMBOL));
            return 0;
        }
    }
    return 23;    // symbol not found
}



int symbolList::removeSymbol(char* searchname){
    for (int i=0; i<number_of_symbols_in_list; i++) {   // Scan for symbols with symbolname=searchname
        if (symbolarray[i]->isThisMyName(searchname) == 1) {
            delete symbolarray[i];
            for (int j=i+1; j<=number_of_symbols_in_list; j++) {   // Move all other symbols down in the array
                symbolarray[j-1] = symbolarray[j];
            }
            number_of_symbols_in_list--;
            // delete symbolarray[number_of_symbols_in_list];    // Delete last symbol in list, which is now a duplicate
            
            return 0;
        }
    }
        return 23;    // symbol not found
}
    
    
SYMBOL* symbolList::retrieveElement(int j) {
    if ((j>=0) && (j<number_of_symbols_in_list)) return symbolarray[j];
    else return NULL;
}

void symbolList::setVariableValue(char *n, int v) {
    for (int i = 0; i<number_of_symbols_in_list; i++) {
        if (symbolarray[i]->isThisMyName(n) == 1) ((VARIABLE *) symbolarray[i])->setValue(v);
    }
}

void symbolList::getVariableValue(char *n, int *v, int *u) {
    int found = 0;  // Records whether variable has been found (0/1)
    int i = 0;
    while (found == 0 && i<number_of_symbols_in_list) {
        if (symbolarray[i]->isThisMyName(n) == 1)  {
            found = 1;
            int value = ((VARIABLE *) symbolarray[i])->getValue();
            int undefined = ((VARIABLE *) symbolarray[i])->amIUndefined();
            *v = value;
            *u = undefined;
        }
        i++;
    }
    if (found == 0) *u = 1; // Set u to indicate undefined value
}

int symbolList::getNextOpenPositionNumber() {
    for (int i = number_of_symbols_in_list-1; i>=0; i--) {
        if (symbolarray[i] == NULL) return i;
    }
    return number_of_symbols_in_list;   // if no previous open spots found, return default first open position at the end of the list
}

int symbolList::getCurrentPosition() {
    return number_of_symbols_in_list;
}

void symbolList::replaceSymbol(SYMBOL* s, int k) {
    symbolarray[k] = s;
}




// symbolStack:

symbolStack::symbolStack() {
    number_of_elements = 0;
}

void symbolStack::print() {
    if (number_of_elements == 0) cout << "This stack is empty.";
    else {
        for (int i=number_of_elements-1; i>=0; i--) {
            (*stack[i]).print();
            cout << "\n";
        }
    }
}


int symbolStack::push(SYMBOL *n){
    if (number_of_elements == LISTLEN) return SYMBOL_LIST_FULL;
    else {
        stack[number_of_elements] = n;
        number_of_elements++;
        return 0;
    }
}


int symbolStack::copyTopLevel(SYMBOL **copyto){
    if (number_of_elements == 0) return SYMBOL_NOT_FOUND;
    else {
        //*copyto = new SYMBOL();
        *copyto = new SYMBOL("temporary");
        //memcpy(*copyto,stack[number_of_elements-1],sizeof(SYMBOL));
        *copyto = stack[number_of_elements-1];
        return 0;
    }
}

int symbolStack::pop(){
    if (number_of_elements == 0) return SYMBOL_NOT_FOUND;
    else {
        number_of_elements--;
        return 0;
    }
}

int symbolStack::isempty() {
    if (number_of_elements == 0) return 1;
    else return 0;
}

int symbolStack::getTopLevelValue(int *v) {
    SYMBOL* top;
    char* name;
    int value, undefined;
    copyTopLevel(&top); // Make copy of symbol on top of symbol list. Now test class type:
    top->copyMyName(&name);
    int type = symbolType(name);
    switch(type) {
            
        case VARTYPE:
            // Variable
            variables.getVariableValue(name,&value,&undefined); // Get value and undefined flag for variable
            *v = value;
            return undefined;
            break;
            
        case CONSTTYPE:
            // Constant
            *v = ((CONSTANT *) top)->getValue();
            return 0;
            break;
            
        default:
            // Not a variable or a constant
            cout << "Error: top level is not a variable or constant.";
            return 1;
            break;
    }
}

void symbolStack::updateWhileJmpForwardRef(int forwardref) {
    int found = 0;  // Records whether variable has been found (0/1)
    int i = 0;
    while (found == 0 && i<number_of_elements) {
        if (stack[i]->isThisMyName("whilejump") == 1)  {
            found = 1;
            ((JMP *) stack[i])->setForwardReferenceNumber(forwardref);
        }
        i++;
    }
}

