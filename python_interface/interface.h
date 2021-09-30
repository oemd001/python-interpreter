#pragma once
#ifndef interface_h
#define interface_h
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Interface {
private:
	typedef vector<string> programType;
	programType programCode;
public:
	
	void startInterface();
	void read(string); 
	void quit(); 
	void help(string);
	void help(); 
	void show(); 
	void show(string); //<-- TODO
	void clear(); 
	void clear_try(); 
	void debug(); 

};



#endif