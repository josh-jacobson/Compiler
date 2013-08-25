/**
 
 EECS 211 Program 8
 main.cpp
 
 Author:  	Josh Jacobson
 Contact:	joshj@u.northwestern.edu
 Due Date:	June 4, 2012
 
 */

#include "system_utilities.h"
#include "symbols.h"
#include "symbol_list.h"
#include <iostream>
using namespace std;

symbolList postfix(ALLOWDUPLICATES);
symbolStack operators;

symbolList variables(DONTALLOWDUPLICATES);
symbolStack evaluation;

int main() {
    
    fillPrecedenceList();
    int PB=0;
    int nextTokenRead = 0;  // Used to indicate if the next token has been read already.
    
    int err = openInputFile("p8input.txt");
    if (err!=0) {
        printError(err);
        return err;
    }    
    fillSpecialTokenList();
    
    
    
    // Infix-to-postfix translation:
    char* myToken;      // Current Token
    char* nextToken;    // Used to store next token if it has already been read
    int type;
    int value;
    int precedence;
    int totalprecedence;
    int tokennumber;
    int postfixposition;
    int holeposition;
    JMP* jump;
    
    do {
        if (nextTokenRead == 1) {
            myToken = nextToken;
            err = 0;
        }
        else err = getNextToken(&myToken);
        if (err !=0) {
            popOperatorStack(&operators, &postfix,PB, STOPATFIRSTIFJMP, PROCESSJUMPS);
            printError(err);  
        }
        else {
            nextTokenRead = 0;  // Reset to 0 in order to process new token
            type = symbolType(myToken);
            
            if (type == VARTYPE) {
                VARIABLE* token = new VARIABLE(myToken);
                postfix.addSymbol(token);     // Add variable to postfix symbol list
            }
            
            if (type == CONSTTYPE) {
                value = convertStringToValue(myToken);
                CONSTANT* token = new CONSTANT("0CONST", value);
                postfix.addSymbol(token);     // Add constant to postfix symbol list
            }
            
            if (type == OPTYPE) {
                int opnumber = getTokenNumber(myToken);
                if (opnumber == LOOP_WHILE) {
                    postfixposition = postfix.getCurrentPosition();
                    OP* whileoperator;
                    whileoperator = new OP("while",LOOP_WHILE,WHILEPREC);
                    
                    jump = new JMP("whilejump",69,postfixposition);          //  Backward reference = current position
                    operators.push(jump);
                    popOperatorStack(&operators, &postfix, totalprecedence, STOPATFIRSTIFJMP, DONTPROCESSJUMPS);
                    operators.push(whileoperator);
                }
                else {
                    precedence = getOperatorPrecedence(opnumber);
                    totalprecedence = precedence+PB;
                    popOperatorStack(&operators, &postfix, totalprecedence, STOPATFIRSTIFJMP, DONTPROCESSJUMPS);
                    OP* token = new OP(myToken,opnumber,totalprecedence);
                    operators.push(token);     // Add object to operator stack
                }
                
            }
            
            if (type == PUNCTYPE) {
                tokennumber = getTokenNumber(myToken);
                
                switch (tokennumber)
                {
                    case SEMICOLON:
                        err = getNextToken(&nextToken);
                        if (err !=0) { // If end of file reached:
                            printError(err);
                            popOperatorStack(&operators, &postfix, 0, STOPATFIRSTIFJMP, PROCESSJUMPS);
                        }
                        else {
                            nextTokenRead = 1;  // Indicate that the next token has been read
                            // if next token is else, don't stop at first if jump
                            if (strcmp("else",nextToken) == 0) popOperatorStack(&operators, &postfix, 0, DONTSTOPATFIRSTIFJMP, PROCESSJUMPS);
                            // Otherwise, don't stop at first if jump
                            else popOperatorStack(&operators, &postfix, 0, STOPATFIRSTIFJMP, PROCESSJUMPS);
                        }
                        
                        if (PB != 0) {
                            printError(UNBALANCED_PARANTHESES);
                            PB = 0;
                        }
                        break;
                    case LEFTPAREN:
                        PB = PB + PARENBOOST;
                        break;
                        
                    case RIGHTPAREN:
                        popOperatorStack(&operators, &postfix, PB, STOPATFIRSTIFJMP, DONTPROCESSJUMPS);
                        if (PB < PARENBOOST) printError(UNBALANCED_PARANTHESES);
                        else PB = PB - PARENBOOST;
                        postfixposition = postfix.getCurrentPosition();
                        if (PB == 0) checkForIfWhile(&operators, &postfix);
                        break;
                        
                    case LEFTBRACKET:
                    {
                        OP* TOKEN = new OP(myToken,tokennumber,PB);
                        operators.push(TOKEN);     // Add object to operator stack
                        break;
                    }
                    case RIGHTBRACKET:
                        operators.pop(); // Pop left bracket from operator stack
                        err = getNextToken(&nextToken);
                        if (err !=0) { // If bad token or end of file reached:
                            printError(err);
                            if (err == 51) popOperatorStack(&operators, &postfix, 0, STOPATFIRSTIFJMP, PROCESSJUMPS);
                        }
                        else {
                            if (strcmp("else",nextToken) == 0) {
                                // if next token is else: 
                                
                                holeposition = postfix.getCurrentPosition(); // record current position
                                jump = new JMP("replace me!",69,0);
                                postfix.addSymbol(jump);  // leave a space in the postfix to be filled in for case when if test succeeds and else case needs to be skipped.  Fill in after parsing code inside else brackets
                                
                                popOperatorStack(&operators, &postfix, 0, STOPATFIRSTIFJMP, PROCESSJUMPS); // pop to first if jump
                                
                                
                                
                                //Push an else jump to the operator stack, referencing the jump object to be updated.
                                
                                jump = new JMP("elsejump",holeposition,0);
                                operators.push(jump);
                                
                            }
                            // Otherwise, pop operator stack until the next left bracket or stack is empty
                            else {
                                nextTokenRead = 1;  // Indicate that the next token has been read
                                popOperatorStack(&operators, &postfix, 0, DONTSTOPATFIRSTIFJMP, PROCESSJUMPS);
                            }
                        }
                        
                        
                        break;
                        
                    default:
                        cout << "***PUNCTUATION ERROR*** \n";
                }
            }
            
        }
    } while (err != 51);
    
    // Print postfix translation:
    cout << "\n\n\nThe following symbol list is the postfix translation of the input file, p8input.txt.\n\n";
    postfix.print(); 
    
    
    
    
    
    // Now execute the postfix:
    cout << "\n\n\nExecuting postfix: \n\n";
    
    int index = 0;
    SYMBOL* element;
    int end, topvalue, secondvalue, undefined, topundefined, secondundefined, newindex;
    SYMBOL* temp;
    char* tempname;
    CONSTANT* myconstant;
    
    do {
        
        // Get new element:
        element = postfix.retrieveElement(index);
        if (element == NULL) end = 1;
        else {
            
            
            
            // Get type:
            tempname = NULL;
            element->copyMyName(&tempname);
            int type = symbolType(tempname);
            
            switch(type) {
                case(JMPTYPE):
                    if (strcmp(tempname,"forwardjmp") == 0) index = ((JMP*) element)->getForwardReferenceNumber();
                    else index = ((JMP*) element)->getBackwardReferenceNumber();
                    index--; // To compensate for index++ at end of loop
                    break;
                    
                case(VARTYPE):
                case(CONSTTYPE):
                    evaluation.push(element); // push variable or constant onto evaluation stack
                    break;
                case(OPTYPE):
                    int opnumber = ((OP *) element)->getOpNumber(); // get operation number of element
                    
                    switch(opnumber) {
                        case TEST_IF:
                        case LOOP_WHILE:
                            evaluation.getTopLevelValue(&newindex);
                            evaluation.pop();
                            evaluation.getTopLevelValue(&value);
                            evaluation.pop();
                            if (value == 0) {
                                index = newindex; // If test stored is false, jump to location specified in postfix
                                index--;  // To compensate for index++ at end of loop
                            }
                            break;
                        case VAR:
                            evaluation.copyTopLevel(&temp);
                            variables.addSymbol(temp);
                            evaluation.pop();
                            break;
                        case ASSIGNMENT:
                            undefined = evaluation.getTopLevelValue(&value);    // Get value of variable or constant on top of evaluation stack
                            evaluation.pop();                                   
                            evaluation.copyTopLevel(&temp);                     // Get the variable in the second position on the stack
                            evaluation.pop();
                            temp->copyMyName(&tempname);
                            if (undefined == 1) cout << "Error: Undefined\n";
                            else variables.setVariableValue(tempname,value);
                            break;
                        case OUTPUT:
                            evaluation.copyTopLevel(&temp);
                            evaluation.pop();
                            temp->copyMyName(&tempname);
                            variables.getVariableValue(tempname,&value,&undefined);
                            cout << "\nName: " << tempname <<"\nValue: ";
                            if (undefined == 1) cout << "Undefined";
                            else cout << value;
                            cout << "\n";
                            break;
                        default:
                            // Arithmetic and comparison operators:
                            topundefined = evaluation.getTopLevelValue(&topvalue);
                            evaluation.pop();
                            secondundefined = evaluation.getTopLevelValue(&secondvalue);
                            evaluation.pop();
                            
                            if ((topundefined == 1) || (secondundefined == 1)) {
                                value = 0;
                                cout << "\nArithmetic Error: Undefined variables\n";
                            }
                            else {
                                switch(opnumber) {
                                    case ADD:
                                        value = secondvalue + topvalue;
                                        break;
                                    case SUBTRACT:
                                        value = secondvalue - topvalue;
                                        break;
                                    case MULTIPLY:
                                        value = secondvalue * topvalue;
                                        break;
                                    case DIVIDE:
                                        if (topvalue == 0) {
                                            cout << "\nArithmetic Error: division by 0. Using 0 as result.\n";
                                            value = 0;
                                        }
                                        else value = secondvalue / topvalue;
                                        break;
                                    case LESS:
                                        value = (secondvalue < topvalue);
                                        break;
                                    case GREATER:
                                        value = (secondvalue > topvalue);
                                        break;
                                    case COMPARE_EQUAL:
                                        value = (secondvalue == topvalue);
                                        break;
                                }
                            }
                            
                            myconstant = new CONSTANT("0CONST",value);
                            evaluation.push(myconstant);
                            break;
                            
                    }
                    break;
            }
            index++;
            
        }
        
    } while (end != 1);
    
    
    cout << "\n\nThank you for compiling with us today. Have a lovely day, and we hope to see you again soon!\n\n";
    
	return 0;
}