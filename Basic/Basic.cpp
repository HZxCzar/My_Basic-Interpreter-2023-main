/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"
#include "Utils/error.hpp"
#include "Utils/tokenScanner.hpp"
#include "Utils/strlib.hpp"
#include "statement.hpp"


/* Function prototypes */

void processLine(std::string line, Program &program, EvalState &state);
//int step=0;
/* Main program */

int main() {
    EvalState state;
    Program program;
    //cout << "Stub implementation of BASIC" << endl;
    while (true) {
        try {
            std::string input;
            getline(std::cin, input);
            if (input.empty())
                return 0;
            processLine(input, program, state);
        } catch (ErrorException &ex) {
            std::cout << ex.getMessage() << std::endl;
        }
    }
    return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(std::string line, Program &program, EvalState &state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);

    std::string token=scanner.nextToken();
    TokenType type=scanner.getTokenType(token);
    if(type==NUMBER)
    {
        int lineNumber=stringToInteger(token);
        int pos=scanner.getPosition();
        if(scanner.hasMoreTokens())
        {
            program.addSourceLine(lineNumber,line.substr(pos));
        }
        else
        {
            program.removeSourceLine(lineNumber);
        }
    }
    else if(type==WORD)
    {
        if(token=="LET")
        {
            Expression* temp=parseExp(scanner);
            LET elem(temp);
            elem.execute(state,program);
        }
        else if(token=="PRINT")
        {
            //std::cout<<"???";
            Expression* temp=parseExp(scanner);
            PRINT elem(temp);
            elem.execute(state,program);
        }
        else if(token=="INPUT")
        {
            token=scanner.nextToken();
            //std::cout<<token;
            INPUT elem(token);
            elem.execute(state,program);
            //std::cout<<"???";
        }
        else if(token=="RUN")
        {
            int lineNumber=program.getFirstLineNumber();
            //step++;
            while(lineNumber!=-1 && program.isarightindex(lineNumber))
            {
                Statement *elem;
                program.setParsedStatement(lineNumber,elem);
                elem->execute(state, program);
                lineNumber=program.getNextLineNumber(lineNumber);//if(step==2)exit(0);
            }
            if(!program.isarightindex(lineNumber) && lineNumber!=-1)
            {
                std::cout<<"LINE NUMBER ERROR\n";
            }
            //exit(0);
        }
        else if(token=="LIST")
        {
            int lineNumber=program.getFirstLineNumber();
            //int pos=0;
            while(lineNumber!=-1)
            {
                std::cout<<lineNumber<<program.getSourceLine(lineNumber)<<'\n';
                lineNumber=program.getNextLineNumber(lineNumber);
            }
            //exit(0);
        }
        else if(token=="CLEAR")
        {
            program.clear();
            state.Clear();
        }
        else if(token=="QUIT")
        {
            program.clear();
            state.Clear();
            exit(0);
        }
        else if(token=="HELP")
        {
            std::cout<<"自己写的问个毛线？\n";
        }
        else {
        std::cout<<"ERROR";
        }
    }
}

