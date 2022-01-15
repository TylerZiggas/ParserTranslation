#pragma once

#ifndef PARSER_H
#define PARSER_H
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include "node.h"
#include "scanner.h"
using namespace std;

class Parser: public Scanner {
private:
	ifstream file;
	string fileInput;

	Token tk;
	Token emptyToken;
	string expectedToken;

	Scanner scanner;
	int scannerStatus;

	NodeT* program();
	NodeT* block();
	NodeT* vars();
	NodeT* expr();
	NodeT* N();
	NodeT* A();
	NodeT* M();
	NodeT* R();
	NodeT* stats();
	NodeT* mStat();
	NodeT* stat();
	NodeT* in();
	NodeT* out();
	NodeT* iff();
	NodeT* loop();
	NodeT* assign();
	NodeT* RO();
	NodeT* label();
	NodeT* gotoo();


public:
	Parser(string);
	NodeT* parse();
	void nextScan();
	NodeT* createNode(string);
	void parserError();

};
#endif