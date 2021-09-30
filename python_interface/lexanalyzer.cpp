#include "lexanalyzer.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <iomanip>

using namespace std;

void LexicalAnalyzer::lex_clear() {
	tokenInfo.clear();
}

void LexicalAnalyzer::lex_generate(string input) {
	tokenInfo.clear();

	int count = 0;

	for (int i = 0; i < input.length(); i++) {
		tokenLineType tokensInLine;
		string temp = "";

		if (isdigit(input[i])) {
			while (isdigit(input[i])) {
				temp = temp + input[i];
				i++;
			}

			pairType number(temp, categoryType::NUMERIC_LITERAL);
			tokensInLine.push_back(number);
		}

		else if (isalpha(input[i])) {
			while (isalpha(input[i])) {
				temp = temp + input[i];
				i++;
			}
			if (temp == "and" || temp == "not" || temp == "or") {
				pairType logical_operator(temp, categoryType::LOGICAL_OP);
				tokensInLine.push_back(logical_operator);
			}
			else if (temp == "print" || temp == "if" || temp == "elif" || temp == "while" || temp == "else" || temp == "int" || temp == "input") {
				pairType  keyword(temp, categoryType::KEYWORD);
				tokensInLine.push_back(keyword);
			}
			else {
				pairType identifier(temp, categoryType::IDENTIFIER);
				tokensInLine.push_back(identifier);
			}
		}

		else if (input[i] == '(') {
			pairType parantheses_left("(", categoryType::LEFT_PAREN);
			tokensInLine.push_back(parantheses_left);
		}
		else if (input[i] == ')') {
			pairType parantheses_right(")", categoryType::RIGHT_PAREN);
			tokensInLine.push_back(parantheses_right);
		}
		else if (input[i] == ',') {
			pairType comma(",", categoryType::COMMA);
			tokensInLine.push_back(comma);
		}
		else if (input[i] == '#') {
			pairType comment("#", categoryType::COMMENT);
			tokensInLine.push_back(comment);
		}
		else if (input[i] == '=') {
			if (input[i + 1] == '=') {
				pairType relational_operator("==", categoryType::RELATIONAL_OP);
				tokensInLine.push_back(relational_operator);
				i++;
			}
			else {
				pairType equal_assignment("=", categoryType::ASSIGNMENT_OP);
				tokensInLine.push_back(equal_assignment);
			}
		}

		else if (input[i] == '<' || input[i] == '>' || input[i] == '<=' || input[i] == '>=' || input[i] == '!') {
			if (input[i + 1] == '<' && input[i] == '<') {
				pairType bitshift_operator("<<", categoryType::RELATIONAL_OP);
				tokensInLine.push_back(bitshift_operator);
				i++;
			}
			else {
				string temp_3 = "";
				temp_3 = temp_3 + input[i];
				pairType logical(temp_3 , categoryType::LOGICAL_OP);
				tokensInLine.push_back(logical);
				i++;
			}
		}

		else if (input[i] == '\'') {
			temp = temp + '\'';
			i++;
			while (input[i] != '\'') {
				temp = temp + input[i];
				i++;
			}
			if (input[i] == '\'') {
				temp = temp + '\'';
				pairType string_literal(temp, categoryType::STRING_LITERAL);
				tokensInLine.push_back(string_literal);
			}
		}

		else if (input[i] == '\"') {
			temp = temp + input[i];
			i++;
			while (input[i] != '\"' && input[i] != NULL) {
				temp = temp + input[i];
				i++;
			}
			cout << temp;
			if (input[i] == '\"') {
				temp = temp + '\"';
				pairType string_literal(temp, categoryType::STRING_LITERAL);
				tokensInLine.push_back(string_literal);
			}
		}

		else if (input[i] == '+') {
			pairType arith("+", categoryType::ARITH_OP);
			tokensInLine.push_back(arith);
		}
		else if (input[i] == '/') {
			pairType arith("/", categoryType::ARITH_OP);
			tokensInLine.push_back(arith);
		}
		else if (input[i] == '*') {
			pairType arith("*", categoryType::ARITH_OP);
			tokensInLine.push_back(arith);
		}
		else if (input[i] == '-') {
			pairType arith("-", categoryType::ARITH_OP);
			tokensInLine.push_back(arith);
		}
		else if (input[i] == '\t') {
			pairType tab("(indent)", categoryType::INDENT);
			tokensInLine.push_back(tab);
		}
		else if (input[i] == ' ') {
			//do nothing
		}
		else {
			pairType unknown("unknown", categoryType::UNKNOWN);
			tokensInLine.push_back(unknown);
		}
		

		for (int i = 0; i < tokensInLine.size(); i++) {
			string str_temp = tokensInLine[i].first;
			cout << "Token[" << count << "]:   " << str_temp << " ";
			if (tokensInLine[i].second == categoryType::KEYWORD) {
				cout << setw(18) << " - KEYWORD";
			}
			else if (tokensInLine[i].second == categoryType::IDENTIFIER) {
				cout << setw(18) << "\t - IDENTIFIER";
			}
			else if (tokensInLine[i].second == categoryType::STRING_LITERAL) {
				cout << setw(18) << "\t - STRING_LITERAL";
			}
			else if (tokensInLine[i].second == categoryType::NUMERIC_LITERAL) {
				cout << setw(18) << "\t - NUMERIC_LITERAL";
			}
			else if (tokensInLine[i].second == categoryType::ASSIGNMENT_OP) {
				cout << setw(18) << "\t - ASSIGNMENT_OP";
			}
			else if (tokensInLine[i].second == categoryType::ARITH_OP) {
				cout << setw(18) << "\t - ARITH_OP";
			}
			else if (tokensInLine[i].second == categoryType::LOGICAL_OP) {
				cout << setw(18) << "\t - LOGICAL_OP";
			}
			else if (tokensInLine[i].second == categoryType::RELATIONAL_OP) {
				cout << setw(18) << "\t - RELATIONAL_OP";
			}
			else if (tokensInLine[i].second == categoryType::LEFT_PAREN) {
				cout << setw(18) << "\t - LEFT_PAREN";
			}
			else if (tokensInLine[i].second == categoryType::RIGHT_PAREN) {
				cout << setw(18) << "\t - RIGHT_PAREN";
			}
			else if (tokensInLine[i].second == categoryType::COLON) {
				cout << setw(18) << "\t - COLON";
			}
			else if (tokensInLine[i].second == categoryType::COMMA) {
				cout << setw(18) << "\t - COMMA";
			}
			else if (tokensInLine[i].second == categoryType::COMMENT) {
				cout << setw(18) << "\t - COMMENT";
			}
			else if (tokensInLine[i].second == categoryType::INDENT) {
				cout << setw(18) << "\t - INDENT";
			}
			else {
				cout << setw(18) << "\t - UNKNOWN";
			}
			count++;
			cout << endl;
		}
	}
		
}