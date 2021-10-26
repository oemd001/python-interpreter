#include <iostream>
#include <stack>
#include <string>
#include <map>
#include <list>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>

#include <cctype>
#include <cstring>
#include <stdlib.h>
#include <regex>
#include "expevaluator.h"
using namespace std;

map<string, int> dict_map;
map<string, string> table;

//initializes the key/value pair dictionary
void expEvaluator::init() {
	dict_map.insert(make_pair<string, int>("(", -1));
	dict_map.insert(make_pair<string, int>(")", -1));
	dict_map.insert(make_pair<string, int>("*", 5));
	dict_map.insert(make_pair<string, int>("/", 5));
	dict_map.insert(make_pair<string, int>("%", 5));
	dict_map.insert(make_pair<string, int>("+", 4));
	dict_map.insert(make_pair<string, int>("-", 4));
	dict_map.insert(make_pair<string, int>("<", 3));
	dict_map.insert(make_pair<string, int>("<=", 3));
	dict_map.insert(make_pair<string, int>(">=", 3));
	dict_map.insert(make_pair<string, int>(">", 3));
	dict_map.insert(make_pair<string, int>("==", 3));
	dict_map.insert(make_pair<string, int>("!=", 3));
	dict_map.insert(make_pair<string, int>("not", 2));
	dict_map.insert(make_pair<string, int>("and", 1));
	dict_map.insert(make_pair<string, int>("or", 0));
	dict_map.insert(make_pair<string, int>(" ", -10));
}

//this function converts an infix expression to an postfix expression
string expEvaluator::infix_postfix_conv(string data) {
	/* if it's a number, add it to the string
	*  if we see the first operation, add it to the stack
	*  if the precednece between two operators are the same, add the one on the stack to the string
	*  if op A's precedence is less than op B, add op A to the stack
	*  if op A's precednece is greater than op B, add it to the string
	*  if the stack is not empty but the string reaches the end, add everything from the stack to the string
	*  if there are parenthesis, add it to the stack. if you reach the end of the parenthesis, everything in between the start paren and the end paren is added to string
	*/
	
	//initialize variables that might be needed
	string result = "";
	stack<string> stack;
	string temp_data = data;
	data = "";
	
	//this is used to add spaces in between the parentheses
	for (int i = 0; i < temp_data.length(); i++) {
		if (temp_data[i] == '(') {
			data += " ";
			data += "(";
			data += " ";
		}
		else if (temp_data[i] == ')') {
			data += " ";
			data += ")";
			data += " ";
		}
		else {
			data += temp_data[i];
		}
	}

	temp_data = data;
	data = "";

	//this "smoothens" out the spacing, so everything does not look very clamped. (x+3+2+1+3+5 becomes x + 3 + 2 + 1 + 3 + 5). The latter is a bit easier to read
	for (int i = 0; i < temp_data.length(); i++) {
		if (temp_data[i] == '<') {
			if (temp_data[i + 1] == '=') {
				data += "<=";
				i++;
			}
			else {
				data += "<";
			}
		}
		else if (temp_data[i] == '>') {
			if (temp_data[i + 1] == '=') {
				data += ">=";
				i++;
			}
			else {
				data += ">";
			}
		}
		else if (temp_data[i] == '=') {
			if (temp_data[i + 1] == '=') {
				data += "==";
				i++;
			}
		}
		else if (temp_data[i] == '!') {
			if (temp_data[i + 1] == '=') {
				data += "!=";
				i++;
			}
		}
		else if (temp_data[i] == 'a') {
			if (temp_data[i + 1] == 'n') {
				if (temp_data[i + 2] == 'd') {
					data += "and";
				}
			}
			i += 2;
			
		}
		else if (temp_data[i] == 'o') {
			if (temp_data[i + 1] == 'r') {
				data += "or";
			}
			i += 2;
		}
		else if (temp_data[i] == 'n') {
			if (temp_data[i + 1] == 'o') {
				if (temp_data[i + 2] == 't') {
					data += "not";
				}
			}
			i += 3;

		}
		else if (isdigit(temp_data[i])) {
			int j = i;
			string val = "";
			int count = 0;
			while (isdigit(temp_data[j])) {
				val += temp_data[j];
				j++;
				count++;
			}
			data += val; 
			i += count - 1; 
		}
		
		else {
			data += temp_data[i];
		}
		data += " ";

		
	}

	//return data;
	//this puts everything in a vector, it's much easier to determine what value is what
	stringstream ss(data);
	istream_iterator<string> begin(ss);
	istream_iterator<string> end;
	vector<string>list(begin, end);
	
	//push something "random" to prevent popping an empty deque 
	stack.push("#");

	
	//this is the infix to postfix conversion
	for (int i = 0; i < list.size(); i++) {
		string temp = list.at(i); 
		bool check = dict_map.find(temp) != dict_map.end();
		if (check) {
			//determine the order of precedence
			int precedence_det = dict_pos(temp);
			//this checks (first) about whether the value in question is a parentheses or not
			if (temp == "(") {
				stack.push(temp);
			}
			else if (temp == ")") {
				stack.push(temp);
				while (stack.top() != "(") {
					result += stack.top() + " ";
					stack.pop();
				}
				//this removes the last "(" from the stack
				if (stack.top() == "(") {
					stack.pop();
				}
			}
			
			//checks if the stack is empty
			else if (stack.empty() && check) {
				stack.push(temp);
			}
			//precedence check
			else if (!stack.empty() && check) {
				//A and B are of the same precedence
				if (precedence_det == dict_pos(stack.top())) {
					result += stack.top() + " ";
					stack.pop();
					stack.push(temp);
				}
				//A is greater than B
				else if (precedence_det > dict_pos(stack.top()) && precedence_det != -1) {
					stack.push(temp);
				}
				//A is less than B
				else if (precedence_det < dict_pos(stack.top()) && precedence_det != -1) {
					result += stack.top() + " ";
					stack.pop();
					if (dict_pos(temp) == dict_pos(stack.top()) && precedence_det != -1) {
						result += stack.top() + " ";
						stack.pop();
						stack.push(temp);
					}
					else {
						stack.push(temp);
					}
				}
				//if there are no operations in the stack (top)
				else {
					stack.push(temp);
				}
			}
		}
		else {
			//if it's just a number, add it to the final string
			result = result + temp + " ";
		}
	}
	
	//this just adds everything that has not been added by the loop
	while (!stack.empty()) {
		result += stack.top() + " ";
		stack.pop();
	}

	//this removes the extra (, extra ) and the item that prevents stack overflows. 
	result.erase(remove(result.begin(), result.end(), '('), result.end());
	result.erase(remove(result.begin(), result.end(), ')'), result.end());
	result.erase(remove(result.begin(), result.end(), '#'), result.end());
	return result; 
}

//this function calcualtes the postfix expression 
string expEvaluator::calc_postfix_exp(string data) {
	int result = 0; 
	
	if (data.length() == 0) {
		return "0";
	}

	//this puts everything in a vector again (for easy access and whatnot)
	stringstream ss(data);
	stack<string> stack;
	istream_iterator<string> begin(ss);
	istream_iterator<string> end;
	vector<string> list(begin, end);
	string::iterator it; 

	if (list.size() == 1) {
		return list.at(0);
	}

	int num = 0; 
	string add = "";
	int a = 0;
	int b = 0;
	string str = "";

	//this is the algorithm that calculates the postfix equation
	for (int i = 0; i < list.size(); i++) {
		string s = list.at(i);
		if (isNumber(s)) {
			stack.push(s);
		}
		else {
			//precedence does not matter here, it just goes from left to right
			//precedence calculations were done beforehand when we converted from infix to postfix
			string potential_result = stack.top();
			a = stoi(stack.top());
			stack.pop();
			if (stack.empty()) {
				if (a == 1) {
					stack.push("0");
				}
				else if (a == 0){
					stack.push("1");
				}
				else {
					return stack.top();
				}
				return stack.top();
			}
			else {
				b = stoi(stack.top());
				stack.pop();
			}
			if (dict_pos(s) != -10) {
				str = "";
				if (s == "+") {
					num = b + a;
					str = to_string(num);
					stack.push(str);
				}
				else if (s == "-") {
					num = b - a;
					str = to_string(num);
					stack.push(str);
				}
				else if (s == "*") {
					num = a * b; 
					str = to_string(num);
					stack.push(str);
				}
				else if (s == "/") {
					num = b / a;
					str = to_string(num);
					stack.push(str);
				}
				else if (s == "%") {
					num = b % a;
					str = to_string(num);
					stack.push(str);
				}
				else if (s == "<") {
					num = b < a;
					str = to_string(num);
					stack.push(str);
				}
				else if (s == "<=") {
					num = b <= a;
					str = to_string(num);
					stack.push(str);
				}
				else if (s == ">") {
					num = b > a;
					str = to_string(num);
					stack.push(str);
				}
				else if (s == ">=") {
					num = b >= a;
					str = to_string(num);
					stack.push(str);
				}
				else if (s == "==") {
					num = b == a;
					str = to_string(num);
					stack.push(str);
				}
				else if (s == "!=") {
					num = b != a;
					str = to_string(num);
					stack.push(str);
				}
				else if (s == "not") {
					//testing !1 returns 1
					int check = stoi(stack.top());
					if (stack.top() == "1") {
						stack.push("0");
					}
					else {
						stack.push("1");
					}
				}
				else if (s == "or") {
					num = a || b;
					str = to_string(num);
					stack.push(str);
				}
				else if (s == "and") {
					num = a && b;
					str = to_string(num);
					stack.push(str);
				}
			}
		}
	}

	return stack.top();
}

//this function checks whether the value that's being looked at is a number or an operator
bool expEvaluator::isNumber(string s) {
	string str = s; 
	for (int i = 0; i < str.length(); i++) {
		//if (!(str[i] >= '0' && str[i] <= '9'))
		if (!isdigit(str[i])) {
			return false;
		}
	}
	return true; 
}

//this function searches for the precedence level for your expression
int expEvaluator::dict_pos(string data) {
	int result = -10;
	//need a key--value pair. can have different keys but same values
	
	for (auto& x : dict_map) {
		if (x.first == data) {
			return x.second;
		}
	}
	return result; 
}

//this clears all the data you've entered to the symbol table
void expEvaluator::clear_data() {
	table.clear();
}

//this function adds more key/value pairs to the symbol table
void expEvaluator::add(string data) {
	stringstream ss(data);
	istream_iterator<string> begin(ss);
	istream_iterator<string> end;
	vector<string>list(begin, end);

	string s1 = list.at(0);
	string s2 = list.at(1);

	table.insert(pair<string, string>(s1, s2));
}

//This returns all the values you've inputted on the symbol table
void expEvaluator::view_messages() {
	cout << "Key(s) \t" << "Value(s)" << endl;
	map<string, string>::iterator it; 
	for (it = table.begin(); it != table.end(); it++) {
		cout << it->first
			<< " \t"
			<< it->second
			<< endl;
	}
}