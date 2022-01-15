#pragma once

#ifndef TREE_H
#define TREE_H
#include <cstdio>
#include <iostream>  
#include <string> 
#include <vector> 
#include "node.h"
using namespace std;

void traversePreorder(NodeT* node, int depth = 0, string indent = "", bool last = true);

#endif
