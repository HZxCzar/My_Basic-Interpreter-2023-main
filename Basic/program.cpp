/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include "program.hpp"
#include "Utils/strlib.hpp"
#include "Utils/tokenScanner.hpp"
#include "exp.hpp"
#include "parser.hpp"
#include "statement.hpp"



Program::Program() = default;

Program::~Program() = default;

void Program::clear() {
    opline.clear();
    for(auto &pair:parsedstatement)
    {
        delete pair.second;
    }
    parsedstatement.clear();
    line_ord.clear();
}

void Program::addSourceLine(int lineNumber, const std::string &line) {
    if(line_ord.count(lineNumber)==0)
    {
        line_ord.insert(lineNumber);
        opline[lineNumber]=line;
    }
    else
    {
        opline[lineNumber]=line;
        if(parsedstatement.count(lineNumber)>0)
        {
            delete parsedstatement[lineNumber];
            parsedstatement.erase(lineNumber);
        }
    }
}

void Program::removeSourceLine(int lineNumber) {
    
    if(line_ord.count(lineNumber)==0)
    {
        return;
    }
    else
    {
        opline.erase(lineNumber);
        if(parsedstatement.count(lineNumber)!=0)
        {
            parsedstatement.erase(lineNumber);
        }
        line_ord.erase(lineNumber);
    }
}

std::string Program::getSourceLine(int lineNumber) {
    if(line_ord.count(lineNumber))
    {
        return opline[lineNumber];
    }
    else
    {
        return (std::string)"";
    }
}

void Program::setParsedStatement(int lineNumber, Statement *&stmt) {
    if(line_ord.count(lineNumber))
    {
        if(parsedstatement.count(lineNumber))
        {
            delete parsedstatement[lineNumber];
        }
        TokenScanner scan;
        scan.ignoreWhitespace();
        scan.scanNumbers();
        scan.setInput(opline[lineNumber]);
        std::string instr=scan.nextToken();
        if(instr=="REM")
        {
            int pos=scan.getPosition();
            stmt=new REM(opline[lineNumber].substr(pos));
            //std::cout<<"!!!!"<<lineNumber;
        }
        else if(instr=="LET")
        {
            stmt=new LET(parseExp(scan));
        }
        else if(instr=="PRINT")
        {
            stmt=new PRINT(parseExp(scan));//
        }
        else if(instr=="INPUT")
        {
            instr=scan.nextToken();
            stmt=new INPUT(instr);
        }
        else if(instr=="END")
        {
            stmt=new END;
        }
        else if(instr=="GOTO")
        {
            instr=scan.nextToken();
            int pos=stringToInteger(instr);
            stmt=new GOTO(pos);
        }
        else if(instr=="IF")
        {
            int pos1=scan.getPosition();
            while(instr!="THEN")
            {
                instr=scan.nextToken();
            }
            int pos2=scan.getPosition();
            int n=stringToInteger(opline[lineNumber].substr(pos2));
            stmt=new IFTHEN(opline[lineNumber].substr(pos1,pos2-pos1-5),n);
        }
        parsedstatement[lineNumber]=stmt;
    }
    else
    {
        throw error;//////////////////~~~~~~~~~~~~~~~~~~~~~~~~~~~
    }
}

//void Program::removeSourceLine(int lineNumber) {

Statement *Program::getParsedStatement(int lineNumber) {
   if(parsedstatement.count(lineNumber))
   {
        return parsedstatement[lineNumber];
   }
   else
   {
        return NULL;
   }
}

int Program::getFirstLineNumber() {
    if(line_ord.empty())
    {
        return -1;
    }
    else
    {
        return *line_ord.begin();
    }
}

int Program::getNextLineNumber(int lineNumber) {
    auto itr=line_ord.end();
    itr--;
    if(save!=0)
    {
        int t=save;
        save=0;
        return t;
    }
    else if(lineNumber==*(itr))
    {
       // std::cout<<"HELLO";
        return -1;
    }
    else
    {
        auto it=line_ord.find(lineNumber);
        it++;
        return *it;
    }
}


//more func to add
//todo

bool Program::isarightindex(int lineNumber)
{
    if(line_ord.count(lineNumber)>0)
    {
        return true;
    }
    else 
    {
        return false;
    }
}
