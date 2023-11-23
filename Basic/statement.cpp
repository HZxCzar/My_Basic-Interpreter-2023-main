/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"
#include "Utils/strlib.hpp"
#include "Utils/tokenScanner.hpp"
#include "exp.hpp"
#include "parser.hpp"
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <string>


/* Implementation of the Statement class */

int stringToInt(std::string str);

Statement::Statement() = default;

Statement::~Statement() = default;

//todo

REM::REM(std::string Comments):comments(Comments){}

void REM::execute(EvalState &state, Program &program)
{
    return;
}

LET::LET(Expression *rhs):instruct(rhs){}

void LET::execute(EvalState &state, Program &program)
{
    instruct->eval(state);
    //delete instruct;
}

PRINT::PRINT(Expression *rhs):instruct(rhs){}

void PRINT::execute(EvalState &state, Program &program)
{
    //std::cout<<instruct->toString();
    int ans=instruct->eval(state);
    std::cout<<ans<<'\n';
    //delete instruct;
}

INPUT::INPUT(std::string rhs):var(rhs){};

void INPUT::execute(EvalState &state, Program &program)
{
    std::cout<<" ? ";
    std::string input;
    getline(std::cin, input);
    bool flag=false;
    while(!flag)
    {
        flag=true;
        for(int i=0;i<input.size();i++)
        {
            if(i==0 && input[i]=='-')
            {
                continue;
            }
            if(!std::isdigit(input[i]))
            {
                flag=false;
                break;
            }
        }
        if(!flag)
        {
            input.clear();
            std::cout<<"INVALID NUMBER\n"<<" ? ";
            getline(std::cin, input);
        }
    }
    int value=stringToInteger(input);
    //std::cout<<" "<<value<<'\n';
    //std::cout<<var;
    state.setValue(var, value);//exit(0);
    //delete var;
}

void END::execute(EvalState &state, Program &program)
{
    program.save=-1;
}

GOTO::GOTO(int num):lineNum(num){};

void GOTO::execute(EvalState &state, Program &program)
{
    program.save=lineNum;
}

void IFTHEN::execute(EvalState &state, Program &program)
{
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    int midop;
    int flag;
    for(int i=0;i<instruct.size();i++)
    {
        if(instruct[i]=='=')
        {
            midop=i;
            flag=0;
            break;
        }
        else if(instruct[i]=='<')
        {
            midop=i;
            flag=1;
            break;
        }
        else if(instruct[i]=='>')
        {
            midop=i;
            flag=2;
            break;
        }
    }
    //std::cout<<instruct<<"\n"<<midop<<'\n';
    scanner.setInput(instruct.substr(0,midop));
    Expression* lhs=parseExp(scanner);
    //std::cout<<"????";
    int lhs_val=lhs->eval(state);
    delete lhs;
    scanner.setInput(instruct.substr(midop+1));
    Expression* rhs=parseExp(scanner);
    int rhs_val=rhs->eval(state);
    delete rhs;
    if(flag==0)
    {
        if(lhs_val==rhs_val)
        {
            program.save=num;
        }
    }
    else if(flag==1)
    {
        if(lhs_val<rhs_val)
        {
            program.save=num;
        }
    }
    else
    {
        if(lhs_val>rhs_val)
        {
            program.save=num;
        }
    }
}