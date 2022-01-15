#pragma once

#ifndef NODE_H
#define NODE_H
#include <string>
#include <vector>
#include "token.h"
using namespace std;

typedef struct node_t {
	int depth;
	string label;
	vector<Token> tokens;
	struct node_t* nodeOne, * nodeTwo, * nodeThree, * nodeFour, * nodeFive;
} NodeT;

#endif