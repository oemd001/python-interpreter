#include <iostream>
#include "interface.h"
#include "lexanalyzer.h"
#include "expevaluator.h"
#include <vector>
#include <string>
#include <regex>
#include <sstream>

//this was used in read(file.py)
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>

using namespace std;

static vector<string> program_vector;
expEvaluator obj;

void Interface::startInterface() {
	// program 3 req
	expEvaluator expEvaluation;

	//greeting message, this will only appear once
	cout << "PySub Interpreter v. 1.0 on Windows (September 2021)" << endl;
	cout << "Enter program lines or read<filename>.py> on the command line interface" << endl;
	cout << "Type 'help' for more information or 'quit' to exit" << endl;
	

	string input = "";
	while (input != "quit") {
		cout << ">>> ";
		getline(cin, input);
		//differentiate between "help" and "help()". The key difference here is using the parentheses
		if (input.find("help(") != string::npos) {
			//this parses word in between the parentheses. If there is no parentheses, this would be invalid
			//help(() will return "command not found"
			unsigned first = input.find("(");
			unsigned last = input.find(")");
			string args = input.substr(first + 1, last - first - 1);
			help(args);
		}
		else if (input.find("show(") != string::npos) {
			//this parses word in between the parentheses. If there is no parentheses, this would be invalid
			//help(() will return "command not found"
			unsigned first = input.find("(");
			unsigned last = input.find(")");
			string args = input.substr(first + 1, last - first - 1);
			show(args);
		}
		else if (input.find("read") != string::npos) {
			//unmatched parentheses will throw a "file not found"
			unsigned first = input.find("(");
			unsigned last = input.find(")");
			string args = input.substr(first + 1, last - first - 1);
			read(args);
		}
		else if (input.find("quit") != string::npos) {
			quit();
		}
		else if (input.find("show") != string::npos) {
			show();
		}
		else if (input.find("clear") != string::npos) {
			clear();
		}
		else if (input.find("help") != string::npos) {
			help();
		}
		else if (input.find("cls") != string::npos) {
			clear_try();
		}
		else if (input.find("add") != string::npos) {
			unsigned first = input.find("(");
			unsigned last = input.find(")");
			string args = input.substr(first + 1, last - first - 1);
			add(args);
		
		}
		else if (input.find("messages") != string::npos) {
			messages();
		}
		else {
			string str = input;
			cout << endl;
			expression(str);
			cout << endl;
		}

	}
}

//MUST be .py, can't read just any other filetype
//usage: read(<filename>.py)
//__cplusplus --> I have 199711L (C++ 98 or C++03)

void Interface::read(string file) {
	//this checks if the file exists
	ifstream f(file.c_str());
	const string file_extension = ".py";
	if (!f.good()) {
		cout << "This file doesn't exist, or you forgot to put the '.py' file extension" << endl;
	}

	//this checks if the inputted file has a .py extension
	else if (file.substr(file.find_last_of(".")) != file_extension) {
		cerr << "Please input a file that has a .py file extension!" << endl;
	}
	//this adds the file in question to the global vector variable
	else {
		//this clears program_vector and fills it with everything in the .py file
		program_vector.clear();
		program_vector.shrink_to_fit();

		ifstream input;
		input.open(file);

		string word = "";

		while (getline(input, word)) {
			if (word.size() > 0) {
				program_vector.push_back(word);
			}
		}
		cout << endl;

	}
}

void Interface::add(string input) {
	stringstream ss(input);
	istream_iterator<string> begin(ss);
	istream_iterator<string> end;
	vector<string>list(begin, end);

	if (list.size() < 2 ) {
		cout << "Please enter two values seperated by a space" << endl;
		cout << "Example:" << endl;
		cout << "You U" << endl;
	}
	else {
		obj.add(input);
	}
}

void Interface::quit() {
	exit(0);
}

void Interface::show(string input) {
	LexicalAnalyzer generate;
	if (input == "tokens" && program_vector.size() > 0) {
		cout << "***** TOKEN INFORMATION *****" << endl;
		for (int i = 0; i < program_vector.size(); i++) {
			cout << "Line # " << i << ":" << endl;
			string temp = program_vector.at(i);
			generate.LexicalAnalyzer::lex_generate(temp);
			cout << "----------------------------------" << endl;
		}
	}
	else if (input != "tokens") {
		cout << "To use this command, type: \"show(tokens) \"" << endl << endl;
	}

	else {
		cout << "There are no tokens to show" << endl << endl;
	}
}

void Interface::show() {
	if (program_vector.size() == 0) {
		cout << "Nothing to show!" << endl;
	}
	else {
		for (int i = 0; i < program_vector.size(); i++) {
			cout << "[" << i << "]" << " " << program_vector.at(i) << endl;
		}
	}
}

void Interface::messages() {
	obj.view_messages();
}

void Interface::clear() {
	program_vector.clear();
	program_vector.shrink_to_fit();
	obj.clear_data();
	cout << "Contents cleared" << endl;
}

//This is the continuous help interface.
void Interface::help() {
	cout << "Welcome to the help utility!" << endl;
	cout << "**To exit and return to the interpreter, type 'exit'" << endl;
	cout << "**To get a description of any command, just type the command at the help> prompt" << endl;

	string input = "";

	//continuous loop until input is equal to "exit"
	//A global hashmap might be smarter as it could save on memory and decrease runtime speeds. 
	//TODO: optimize this loop and change this in to a hashmap
	while (input != "exit") {
		cout << "help> ";
		cin >> input;
		if (input.find("quit") != string::npos) {
			cout << "This commands exits the command line interpreter" << endl;
			cout << "To exit the help utility and return to the interpreter, enter 'exit'" << endl << endl;
			cout << "Usage:" << endl;
			cout << "     " << "quit" << endl;
		}
		else if (input.find("help") != string::npos) {
			cout << "This brings up this help utility" << endl;
			cout << "An alternative usage would be typing in help(<command>) outside the help interface" << endl << endl;
			cout << "To bring up a list of commands, type 'commands' within this help utility" << endl << endl;
			cout << "Usage:" << endl;
			cout << "     " << "help" << endl;
			cout << "     " << "help(<command>)" << endl;
		}
		else if (input.find("read") != string::npos) {
			cout << "This command enables PySub to read a given python file" << endl;
			cout << "NOTE: This will clear and override your existing code" << endl << endl;
			cout << "Usage: " << endl;
			cout << "     " << "read(<filename/path to filename>.py)" << endl;
		}
		else if (input.find("show") != string::npos) {
			cout << "This command shows the lines of the program you've typed in to the interpreter" << endl << endl;
			cout << "Usage:" << endl;
			cout << "     " << "show" << endl << endl;

			cout << "An alternate usage of this command would be \"show(tokens)\"" << endl;
			cout << "This command will show the different tokens of this particular python program you've either inputted or typed in" << endl;
			cout << "Note: This command will return nothing if nothing is either inputted or typed in to this console." << endl << endl;
			cout << "Usage:" << endl;
			cout << "     " << "show(tokens)" << endl;
		}
		else if (input.find("clear") != string::npos) {
			cout << "This command clears all of the existing lines of program you've typed in to the interpreter (This includes the token viewer as well)" << endl << endl;
			cout << "Usage:" << endl;
			cout << "     " << "clear" << endl;
		}
		else if (input.find("commands") != string::npos) {
			cout << "Below is a list of commands. Enter any command at the prompt to get more help" << endl << endl;
			cout << "clear \t help \t show(tokens) \t quit \nread \t cls \t show" << endl << endl;
			cout << "NOTE: All commands can also be entered as functions:" << endl << endl;

			cout << "clear()" << "    ";
			cout << "help()" << "    ";
			cout << "show(tokens)" << "    ";
			cout << "quit() " << endl;
			cout << "read()" << "    ";
			cout << "cls()" << "    ";
			cout << "show() " << "    ";
			cout << "messages() " << endl;
			cout << "add() " << "    ";
			cout << "\"Expressions\" ";
		}
		else if (input.find("cls") != string::npos) {
			cout << "This command clears the contents of a crowded console." << endl;
			cout << "To use this command, exit out of the help utility and type in 'cls'" << endl << endl;
			cout << "Usage:" << endl;
			cout << "     " << "cls" << endl;
		}
		else if (input.find("exit") != string::npos) {
			break;
		}
		else if (input.find("messages()") != string::npos) {
			cout << "This command allows you to view all the existing values you put on the symbol table" << endl << endl;
			cout << "Usage: " << endl;
			cout << "     " << "messages()" << endl;
		}
		else if (input.find("add()") != string::npos) {
			cout << "This command allows you to add custom key/value combinations to the symboltable" << endl;
			cout << "In order to use this command effectively, the key/value combination MUST BE seperated by a space" << endl;
			cout << "Usage: " << endl;
			cout << "     " << "add(key value)" << endl;
		}
		else if (input.find("Expressions") != string::npos) {
			cout << "To activate the expression calculator functionalty, simply enter an expression anytime on the command window" << endl << endl;
			cout << "Usage:" << endl;
			cout << "     " << "(3 + 2)" << endl;
		}
		else {
			cout << "Please enter a valid command!" << endl << endl;
			cout << "If you're looking to enter an expression, do something like: (3 + 2) and click enter" << endl;
		}
	}
}

//same TODO as help() interface
void Interface::help(string input) {
	if (input.find("quit") != string::npos) {
		cout << "This commands exits the command line interpreter" << endl;
		cout << "To exit the help utility and return to the interpreter, enter 'exit'" << endl << endl;
		cout << "Usage:" << endl;
		cout << "     " << "quit" << endl;
	}
	else if (input.find("help") != string::npos) {
		cout << "This brings up the help utility" << endl;
		cout << "An alternative usage would be typing in help(<command>) outside the help interface" << endl << endl;
		cout << "To bring up a list of commands, type 'commands' within this help utility" << endl << endl;
		cout << "Usage:" << endl;
		cout << "     " << "help" << endl;
		cout << "     " << "help(<command>)" << endl;
	}
	else if (input.find("read") != string::npos) {
		cout << "This command enables PySub to read a given python file" << endl;
		cout << "NOTE: This will clear and override your existing code" << endl << endl;
		cout << "Usage: " << endl;
		cout << "     " << "read(<filename/path to filename>.py)" << endl;
	}
	else if (input.find("show") != string::npos) {
		cout << "This command shows the lines of the program you've typed in to the interpreter" << endl << endl;
		cout << "Usage:" << endl;
		cout << "     " << "show" << endl << endl;

		cout << "An alternate usage of this command would be \"show(tokens)\"" << endl;
		cout << "This command will show the different tokens of this particular python program you've either inputted or typed in" << endl;
		cout << "Note: This command will return nothing if nothing is either inputted or typed in to this console." << endl << endl;
		cout << "Usage:" << endl;
		cout << "     " << "show(tokens)" << endl;
	}
	else if (input.find("clear") != string::npos) {
		cout << "This command clears all of the existing lines of program you've typed in to the interpreter (This includes the token viewer as well)" << endl << endl;
		cout << "Usage:" << endl;
		cout << "     " << "clear" << endl;
	}
	else if (input.find("commands") != string::npos) {
		cout << "Below is a list of commands. Enter any command at the prompt to get more help" << endl << endl;
		cout << "clear \t help \t show(tokens) \t quit \nread \t cls \t show" << endl << endl;
		cout << "NOTE: All commands can also be entered as functions:" << endl << endl;

		cout << "clear()" << "    ";
		cout << "help()" << "    ";
		cout << "show(tokens)" << "    ";
		cout << "quit() " << endl;
		cout << "read()" << "    ";
		cout << "cls()" << "    ";
		cout << "show() " << "    ";
		cout << "messages() " << endl;
		cout << "add() " << "    ";
		cout << "\"Expressions\" ";
	}
	else if (input.find("cls") != string::npos) {
		cout << "This command clears the contents of a crowded console." << endl;
		cout << "To use this command, exit out of the help utility and type in 'cls'" << endl << endl;
		cout << "Usage:" << endl;
		cout << "     " << "cls" << endl;
	}
	else if (input.find("messages()") != string::npos) {
		cout << "This command allows you to view all the existing values you put on the symbol table" << endl << endl;
		cout << "Usage: " << endl;
		cout << "     " << "messages()" << endl;
	}
	else if (input.find("add()") != string::npos) {
		cout << "This command allows you to add custom key/value combinations to the symboltable" << endl;
		cout << "In order to use this command effectively, the key/value combination MUST BE seperated by a space" << endl;
		cout << "Usage: " << endl;
		cout << "     " << "add(key value)" << endl;
	}
	else if (input.find("Expressions") != string::npos) {
		cout << "To activate the expression calculator functionalty, simply enter an expression anytime on the command window" << endl << endl;
		cout << "Usage:" << endl;
		cout << "     " << "(3 + 2)" << endl;
	}
	else {
		cout << "Please enter a valid command!" << endl << endl;
		cout << "If you're looking to enter an expression, do something like: (3 + 2) and click enter" << endl;
	}
}

//the try/catch is an attempt to make this portable between UNIX/Windows/Mac. 
//This function was a result of a very VERY cluttered terminal
void Interface::clear_try() {
	try {
		system("cls");
	}
	catch (string nothing) {
		system("clear");
	}
}

void Interface::expression(string input) {
	expEvaluator start;
	start.expEvaluator::init();
	//string input = "1 < 5";
	//string input = "19 - 4 + 22 / 11 * 5 % 3 - 1"; 
	//string input = "(3 + 5)";
	//string input = "22 - (30 / 15) + (4 * 3 - 2) / 5";
	//string input = "22 - ( 30 / 15 ) + ( 4 * 3 - 2 ) / 5"; 
	//string input = "(35 / 7 * 2) < (44 % 10 + 2) or (18 + 4 / 2) == (4 * 5)";
	//string input = "(7 + 3) <= 25 and ((4 * 5) == (14 + 3) or not(100 >= (40 * 2)))";
	//string input = "1 and 0";
	//string input = "not(1)";

	//string input = "19-4+22/11*5%3-1";
	//string input = "(3+5)";
	//string input = "22-(30/15)+(4*3-2)/5";
	//string input = "(35 / 7 * 2) < (44 % 10 + 2) or (18 + 4 / 2) == (4 * 5)";
	//string input = "(7+3)<=25and((4*5)==(14+3)or not(100>=(40*2)))";
	//string input = "(7+3) <= 25 and ((4*5)==(14+3)or not(100>=(40*2)))";
	string result = start.expEvaluator::infix_postfix_conv(input);
	cout << "Infix expression: " << input << endl;
	cout << "Postfix expression: " << result << endl;
	cout << "Evaluated answer: " << start.expEvaluator::calc_postfix_exp(result) << endl;
	//cout << result;
}

