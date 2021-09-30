#pragma once
#ifndef lexanalyzer_h
#define lexanalyzer_h
#include <iostream>
#include <vector>
#include <string>

using namespace std;

enum class categoryType {
    KEYWORD,
    IDENTIFIER,
    STRING_LITERAL,
    NUMERIC_LITERAL,
    ASSIGNMENT_OP,
    ARITH_OP,
    LOGICAL_OP,
    RELATIONAL_OP,
    LEFT_PAREN,
    RIGHT_PAREN,
    COLON,
    COMMA,
    COMMENT,
    INDENT,
    UNKNOWN
};


class LexicalAnalyzer {
private:
    typedef vector<vector<pair<string, categoryType>>> tokenType;
    typedef vector<pair<string, categoryType>> tokenLineType; 
    typedef pair<string, categoryType> pairType; 
    tokenType tokenInfo; 
public:
    void lex_generate(string);
    void lex_clear();
};

#endif