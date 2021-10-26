#pragma once
#ifndef expevaluator_h
#define expevaluator_h

#include <iostream>
#include <string>
#include <map>
using namespace std;

class expEvaluator {
private:
	typedef map<string, string> symbolTableType;
	symbolTableType symbolTable; //not used until program 4
public:
	string infix_postfix_conv(string); //DONE!
	string calc_postfix_exp(string); //DONE
	int dict_pos(string); //DONE
	void clear_data(); //DONE
	bool isNumber(string); //DONE

	void add(string);
	void view_messages();
	void init();
};

/* requirements:

* store variables and values in the symbol table data structure (need another method for this)

* to find and retrieve a value from the symbol table data structure for a given variable 
* ** this is probably going to be built in
* 
*/


#endif